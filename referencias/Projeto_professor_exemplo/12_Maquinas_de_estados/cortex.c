
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SM_UPDATE_MS 1000

static void sm_update_trigger(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

//void HAL_SYSTICK_Callback(void) => pode ser usado no HAL STM32
void SysTick_Handler(void)
{
	static uint16_t sm_update_ms = 0;

	if(++sm_update_ms >= SM_UPDATE_MS)
	{
		sm_update_ms = 0;
		sm_update_trigger();
	}

	// outros eventos temporizados
}

void PendSV_Handler(void)
{
	// chame o tick da sua SM aqui ! Por exemplo:
	// sm_func(&sm);
}

void main(void)
{
	// configure as prioridades, sendo que PSV > SYSTICK
	HAL_NVIC_SetPriority(PendSV_IRQn, 3, 0);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);	  

	while(1)
	{
		_WFI();
	}
}
