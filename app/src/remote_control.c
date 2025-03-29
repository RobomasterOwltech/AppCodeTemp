#include <stm32f3xx_hal.h>
#include <stm32f3xx_it.h>
#include <string.h>

#include "CommunicationStructs.h"
#include "cmsis_os.h"
#include "usart.h"

control_data control1;
UART_HandleTypeDef huart1;
uint8_t UART1_rxBuffer;

osMessageQDef(remoteQueue, 16, unsigned int);

void UART_Init(void) {}

void sbus_translate(control_data* _control, uint8_t* _buffer) {
    _control->joystickA = (_buffer[0] >> 1) & 0x7F;                                 // 7bits
    _control->joystickB = ((_buffer[0] & 0x01) << 6) | ((_buffer[1] >> 2) & 0x3F);  // 7bits
    _control->switchA = (_buffer[1] >> 1) & 0x01;                                   // 2bits
    _control->switchB = _buffer[1] & 0x01;                                          // 2bits
    _control->switchC = (_buffer[2] >> 6) & 0x03;                                   // 4bits
    _control->switchD = (_buffer[2] >> 4) & 0x03;                                   // 4bits
    _control->knobA = _buffer[3];                                                   // 8bits
    _control->knobB = _buffer[4];                                                   // 8bits
}

void UART_Task(void* argument) {
    HAL_UART_Receive_IT(&huart1, &UART1_rxBuffer, 12);
    remoteQueue = osMessageCreate(osMessageQ(remoteQueue), NULL);
    for (;;) {
        sbus_translate(&control1, &UART1_rxBuffer);
        osMessagePut(remoteQueue, (unsigned int)&control1, osWaitForever);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart) { HAL_UART_Receive_IT(&huart1, &UART1_rxBuffer, 12); }
