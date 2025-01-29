#include <string.h>

#include "esp_log.h"

#include "spiffs_dma.h"

static const char TAG[] = "FileSystem";
static esp_err_t result;

size_t total = 0, used = 0;

esp_vfs_spiffs_conf_t config =
{
    .base_path = MUSIC_FOLDER,      ///> folder that will be accessed
    .partition_label = NULL,    ///> partition label if there were more than 1
    .max_files = 5,             ///> max number of opened files
    .format_if_mount_failed = TRUE,
};

void fileSystem_setup(void)
{
    result = esp_vfs_spiffs_register(&config);
    if(result != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(result));
        return;
    }
}

void fileSystem_updateStatistics(void)
{
    result = esp_spiffs_info(config.partition_label, &total, &used);
    if(result != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get partition info (%s)", esp_err_to_name(result));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size:\n\ttotal: %d,\n\tused: %d", total, used);
    }
}

size_t fileSystem_readFile(char* filename, uint8_t * buffer)
{
    size_t bytes_read = 0;
    FILE * f = NULL;
    char filePath[100] = MUSIC_FOLDER;

    strcat(filePath, "/");
    strcat(filePath, filename);

    f = fopen(filePath, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return 0;
    }

    bytes_read = fread(buffer, 1, 1024, f);

    fclose(f);

    return bytes_read;
}