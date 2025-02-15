#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <Nucleo_F767ZI_GPIO.h>
#include <SEGGER_SYSVIEW.h>
#include <Nucleo_F767ZI_Init.h>
#include <stm32f7xx_hal.h>
#include <UartQuickDirtyInit.h>
#include "Uart4Setup.h"

// Exportar los setups necesarios para que funcione



void polledUartReceive ( void* NotUsed ); // void not used?
void uartPrintOutTask( void* NotUsed); // void not used? 

static QueueHandle_t uart2_BytesReceived = NULL;

int main(void)
{
	HWInit();
	SetupUart4ExternalSim(9600);
	SEGGER_SYSVIEW_Conf();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	assert_param(xTaskCreate(polledUartReceive, "polledUartRx", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL) == pdPASS);
	assert_param(xTaskCreate(uartPrintOutTask, "uartPrintTask", STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL) == pdPASS);

	uart2_BytesReceived = xQueueCreate(10, sizeof(char));


// porque esta comentado?
//	for(int i = 0; i < 10; i++)
//	{
//		UART4->TDR = i;
//		while(!(UART4->ISR & USART_ISR_TXE));
//	}
	//start the scheduler - shouldn't return unless there's a problem
	vTaskStartScheduler();

	while(1)
	{
	}
}

void uartPrintOutTask( void* NotUsed)
{
	char nextByte;

	while(1)
	{
		xQueueReceive(uart2_BytesReceived, &nextByte, portMAX_DELAY);
		SEGGER_SYSVIEW_PrintfHost("%c", nextByte); //Donde lo esta imprimiendo?
	}
}

// Como funciona esto?
void polledUartReceive( void* NotUsed )
{
	uint8_t nextByte;
	STM_UartInit(USART2, 9600, NULL, NULL);
	while(1)
	{
		while(!(USART2->ISR & USART_ISR_RXNE_Msk));
		nextByte = USART2->RDR;
		xQueueSend(uart2_BytesReceived, &nextByte, 0);
	}
}

