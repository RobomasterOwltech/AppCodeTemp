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
    _control->joystickAx = ((_buffer[1] >> 3) | (_buffer[2] << 5)) & 0xff;
    _control->joystickAy = ((_buffer[2] >> 6) | (_buffer[3] << 2) | (_buffer[4] << 10)) & 0xff;
    _control->joystickBx = ((_buffer[4] >> 1) | (_buffer[5] << 7)) & 0x0ff;
    _control->joystickBy = ((_buffer[5] >> 1) | (_buffer[6] << 7)) & 0x0ff;
    _control->knobA = ((_buffer[6] >> 7) | (_buffer[7] << 1)) & 0xff;
    _control->knobB = ((_buffer[7]) | (_buffer[8])) & 0xff;
    _control->switchA = (_buffer[9] & 0x07) >> 2;
    _control->switchB = (_buffer[10] & 0x20) >> 5;
    _control->switchC = ((_buffer[12] >> 3) & 0x03);
    _control->switchD = ((_buffer[13] & 0x08) >> 3);
}

void UART_Task(void* argument) {
    HAL_UART_Receive_IT(&huart1, &UART1_rxBuffer, 12);
    osPoolDef(remote_pool, 16, control_data);
    remoteQueue = osMessageCreate(osMessageQ(remoteQueue), NULL);
    for (;;) {
        sbus_translate(&control1, &UART1_rxBuffer);
        osMessagePut(remoteQueue, (unsigned int)&control1, osWaitForever);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart) { HAL_UART_Receive_IT(&huart1, &UART1_rxBuffer, 12); }
