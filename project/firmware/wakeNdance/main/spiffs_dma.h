#ifndef __SPIFFS_DMA_LIB__
#define __SPIFFS_DMA_LIB__

#include "esp_spiffs.h"
#include "projectConfig.h"

#define MUSIC_FOLDER    "/music"

size_t fileSystem_readFile(char* filename, uint8_t * buffer);

#endif //__SPIFFS_DMA_LIB__