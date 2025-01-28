#include <stdio.h>
#include <sys/param.h>
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"

#include "i2s.h"
#include "projectConfig.h"

// tag
static const char TAG[] = "I2S";

i2s_chan_handle_t tx_handle;


QueueHandle_t xQueueData; // fila de dados e eventos para transferir entre as tarefas de leituras do youtube e envio de dados
TaskHandle_t xTaskReadDataHandle; // handler para task de leitura de dados do youtube
TaskHandle_t xTaskTransmitDataHandle; // handler para task de envio de dados a caixa de som


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
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(44100), // 44100 amostras por segundo
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

static void vTaskWriteData(void * pvParameters);
static void vTaskFetchData(void * pvParameters);

void i2s_setup(void)
{
    // alocar o canal i2s para receber dados
    i2s_new_channel(&chan_cfg, NULL, &tx_handle);

    // inicializar o canal i2s no modo padrao
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
}

void i2s_start(void)
{
    BaseType_t xReturnedTask[2];

    i2s_setup();

    xQueueData = xQueueCreate(DMA_BUF_NUM, BUF_SIZE*sizeof(char)); 
    if(xQueueData == NULL){ // testar se a criacao da fila falhou
        ESP_LOGE(TAG, "Falha em criar fila de dados");
        while(1);
    }
    
    xReturnedTask[0] = xTaskCreatePinnedToCore(
        vTaskWriteData, 
        "taskREAD", 
        configMINIMAL_STACK_SIZE+2048, 
        NULL, 
        configMAX_PRIORITIES-1, 
        &xTaskReadDataHandle, 
        APP_CPU_NUM
    );
    
    
    xReturnedTask[1] = xTaskCreatePinnedToCore(
        vTaskFetchData, 
        "taskTX", 
        configMINIMAL_STACK_SIZE+2048, 
        NULL, 
        configMAX_PRIORITIES-1, 
        &xTaskTransmitDataHandle, 
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
    uint8_t w_buf[BUF_SIZE]; // buffer para entrada de dados

    /* Assign w_buf */
    for (int i = 0; i < BUF_SIZE; i += 8) {
        w_buf[i]     = 0x12;
        w_buf[i + 1] = 0x34;
        w_buf[i + 2] = 0x56;
        w_buf[i + 3] = 0x78;
        w_buf[i + 4] = 0x9A;
        w_buf[i + 5] = 0xBC;
        w_buf[i + 6] = 0xDE;
        w_buf[i + 7] = 0xF0;
    }
 
    // iniciar o canal i2s
    i2s_channel_enable(tx_handle);
 
    while (1) {
        // esperar para que o buffer de entrada (w_buf) seja totalmente preenchido
        if (i2s_channel_write(tx_handle, (void*) w_buf, bytes_to_write, &bytes_written, portMAX_DELAY) == ESP_OK) {
            xQueueSend(xQueueData, &w_buf, portMAX_DELAY); // enfileirar dados lidos para a tarefa de envio
        } else {
            ESP_LOGE(TAG, "Erro durante a leitura");
            break;
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

}