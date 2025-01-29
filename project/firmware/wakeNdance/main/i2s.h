#ifndef __I2S_PERSONAL_LIB__
#define __I2S_PERSONAL_LIB__

#include "driver/i2s_std.h"
#include "driver/gpio.h"

#define DMA_BUF_NUM 16 // quantidade de buffers do DMA
#define DMA_BUF_SIZE 1024 // tamanho [em amostras] dos buffers do DMA

void i2s_setup(void);
void i2s_start(void);

#endif //__I2S_PERSONAL_LIB__