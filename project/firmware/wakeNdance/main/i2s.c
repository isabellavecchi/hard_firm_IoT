#include <stdio.h>
#include <sys/param.h>
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "i2s.h"
#include "projectConfig.h"
#include "tasks_common.h"
#include "spiffs_dma.h"

// tag
static const char TAG[] = "I2S";

i2s_chan_handle_t tx_handle;

uint8_t w_buf[BUF_SIZE]; // buffer para entrada de dados

// configuracao do canal i2s 
i2s_chan_config_t chan_cfg = {
    .id = I2S_NUM_0, // escolha do canal automatica (0 ou 1)
    .role = I2S_ROLE_MASTER, // o esp32 atua como o controlador
    .dma_desc_num = DMA_BUF_NUM, // quantidade de buffers do DMA
    .dma_frame_num = DMA_BUF_SIZE, // tamanho dos buffers do DMA
    .auto_clear = false, // limpar automaticamente o buffer TX (desnecessario)
};

// configuracoes de clock, slot e gpio para o i2s no modo normal (standard)
i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(I2S_FREQ), // 44100 amostras por segundo
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED, // master clock (opcional)
        .bclk = I2S_BCLK, // bit clock
        .ws = I2S_GPIO_UNUSED, // word slot
        .dout = I2S_DATA, // data out
        .din = I2S_GPIO_UNUSED, // data in (necessario apenas para recepcao de dados)
        .invert_flags = { // nao inverter bits
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
    },
};

// Semaphores for synchronization
SemaphoreHandle_t xSemaphoreDataReady; // Signaled when new data is available
SemaphoreHandle_t xSemaphoreSpaceAvailable; // Signaled when buffer is ready for new data

TaskHandle_t xTaskWriteDataHandle; // handler para task de leitura de dados do youtube
TaskHandle_t xTaskFetchDataHandle; // handler para task de envio de dados a caixa de som

static void vTaskWriteData(void * pvParameters);
static void vTaskFetchData(void * pvParameters);

void i2s_setup(void)
{
    // alocar o canal i2s para receber dados
    i2s_new_channel(&chan_cfg, NULL, &tx_handle);

    // inicializar o canal i2s no modo padrao
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
}

// void i2s_preloadData(void)
// {
//     /* (Optional) Preload the data before enabling the TX channel, so that the valid data can be transmitted immediately */
//     while (w_bytes == EXAMPLE_BUFF_SIZE) {
//         /* Here we load the target buffer repeatedly, until all the DMA buffers are preloaded */
//         ESP_ERROR_CHECK(i2s_channel_preload_data(tx_chan, w_buf, EXAMPLE_BUFF_SIZE, &w_bytes));
//     }
// }

void i2s_start(void)
{
    BaseType_t xReturnedTask[2];

    i2s_setup();

    xSemaphoreDataReady = xSemaphoreCreateBinary(); 
    if (xSemaphoreDataReady == NULL) {
        ESP_LOGE(TAG, "Failed to create data ready semaphore");
        return;
    }

    xSemaphoreSpaceAvailable = xSemaphoreCreateBinary();
    if (xSemaphoreSpaceAvailable == NULL) {
        ESP_LOGE(TAG, "Failed to create space available semaphore");
        vSemaphoreDelete(xSemaphoreDataReady); 
        return;
    }

    // **Initialize semaphores:**
    // - xSemaphoreDataReady: Initially not signaled (no data available)
    // - xSemaphoreSpaceAvailable: Initially signaled (buffer is available)
    xSemaphoreGive(xSemaphoreSpaceAvailable); 

    
    xReturnedTask[0] = xTaskCreatePinnedToCore(
        vTaskWriteData, 
        "taskWriteSound", 
        I2S_TASK_STACK_SIZE, 
        NULL, 
        I2S_TASK_PRIORITY, 
        &xTaskWriteDataHandle, 
        I2S_TASK_CORE_ID
    );
    
    
    xReturnedTask[1] = xTaskCreatePinnedToCore(
        vTaskFetchData, 
        "taskTX", 
        configMINIMAL_STACK_SIZE+2048, 
        NULL, 
        configMAX_PRIORITIES-1, 
        &xTaskFetchDataHandle, 
        PRO_CPU_NUM
    );
    
    // testar se a criacao das tarefas falhou
    if(xReturnedTask[0] == pdFAIL || xReturnedTask[1] == pdFAIL){ 
        ESP_LOGE(TAG, "Falha em criar tarefas");
        while(1);
    }
}


static void vTaskWriteData(void * pvParameters)
{
    size_t bytes_to_write = BUF_SIZE; // quantidades de bytes para ler
    size_t bytes_written; // quantidade de bytes lidos

    // iniciar o canal i2s
    i2s_channel_enable(tx_handle);
 
    while (1)
    {
        // Wait for data to be available
        xSemaphoreTake(xSemaphoreDataReady, portMAX_DELAY);

        // esperar para que o buffer de entrada (w_buf) seja totalmente preenchido
        if (i2s_channel_write(tx_handle, (void*) w_buf, bytes_to_write, &bytes_written, portMAX_DELAY) == ESP_OK)
        {
            // Signal that the buffer is available for new data
            xSemaphoreGive(xSemaphoreSpaceAvailable);
        }
        else
        {
            xSemaphoreGive(xSemaphoreDataReady);
            ESP_LOGE(TAG, "Erro durante a leitura");
        }
        vTaskDelay(1);
    }
    // no caso de erro, deve-se parar o canal i2s
    i2s_channel_disable(tx_handle);
    // liberar os recursos alocados
    i2s_del_channel(tx_handle);
 
    vTaskDelete(NULL);
}

static void vTaskFetchData(void * pvParameters)
{
    while (1)
    {
        // Wait for space to be available
        xSemaphoreTake(xSemaphoreSpaceAvailable, portMAX_DELAY); 

        // Read data from SPIFFS
        size_t bytes_read = fileSystem_readFile("A6 - Telegram Sam.flac", w_buf); 
        if (bytes_read > 0)
        {
            // Signal that new data is available
            xSemaphoreGive(xSemaphoreDataReady);
        }
        else
        {
            ESP_LOGE(TAG, "Error reading from SPIFFS");
            // Handle error (e.g., break the loop)
            xSemaphoreGive(xSemaphoreSpaceAvailable);
        }
    }
}