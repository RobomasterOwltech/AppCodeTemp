#include <stm32f3xx_hal.h>
#include <stm32f3xx_it.h>
#include <string.h>

#include "cmsis_os.h"
#include "usart.h"

uint8_t UART1_rxBuffer[12];
UART_HandleTypeDef uart;
osMessageQId remoteQueue;
osMessageQDef(remoteQueue, 16, unsigned int);

#pragma pack(push, 1)
struct control_data {
    uint8_t joystickA;
    uint8_t joystickB;
    uint8_t knobA;
    uint8_t knobB;
    uint8_t switchA;
    uint8_t switchB;
    uint8_t switchC;
    uint8_t switchD;
} control_data;
#pragma pack(pop)

void UART_Init(void) {}

void UART_Task(void* argument) {
    HAL_UART_Receive_IT(&uart, UART1_rxBuffer, 12);
    remoteQueue = osMessageCreate(osMessageQ(remoteQueue), NULL);
    for (;;) {
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart) {
    HAL_UART_Receive_IT(uart, UART1_rxBuffer, 12);
    memcpy(&control_data, &UART1_rxBuffer, sizeof(control_data));
    osMessagePut(remoteQueue, (unsigned int)&control_data, osWaitForever);
    HAL_UART_Transmit(uart, (uint8_t*)"\n\nSent from ISR\n\n", 17, 500);
}
