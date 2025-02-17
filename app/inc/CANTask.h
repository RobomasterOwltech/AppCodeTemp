
/**
 * CANTask.c
 * 
 * Created on: January 12, 2024
 *     Author: Erick Daniel Ortiz Cervantes
 *  Reference: https://www.youtube.com/watch?v=KHNRftBa1Vc
 */

#pragma once

#include "main.h"
#include "cmsis_os.h"
#include "semphr.h"

#include "structs.h"

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

MotorControlMessage TxData;
MotorControlMessage RxData;

uint32_t TxMailbox;

SemaphoreHandle_t xCANSemaphore;

osThreadId_t CANTxTaskHandle;
const osThreadAttr_t CANTask_attributes = {
  .name = "CANTxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t CANRxTaskHandle;
const osThreadAttr_t CANTask_attributes = {
  .name = "CANRxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static void MX_CAN_Init(void);
void StartCANTxTask(void *argument);
void StartCANRxTask(void *argument);

void StartCANTxTask(void *argument) {
  while (1) {
    // TODO: Implement TX logic
  }
}

void StartCANRxTask(void *argument) {
  while (1) {
    if (osSemaphoreAcquire(xCANSemaphore, osWaitForever) == osOK) {
      // Send the message to the corresponding node or subsystem
      // TODO: Implement the logic to retrived data from the remote controller
      TxData.vMotor1 = RxData.vMotor1;
      TxData.vMotor2 = RxData.vMotor2;
      TxData.vMotor3 = RxData.vMotor3;
      TxData.vMotor4 = RxData.vMotor4;
      osSemaphoreAcquire(xCANSemaphore, osWaitForever);
      HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);

      osDelay(100);
    }
  }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
  if (hcan->State == HAL_CAN_STATE_READY) {
    osSemaphoreRelease(xCANSemaphore);
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (hcan->State == HAL_CAN_STATE_READY) {
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, (uint8_t*) &RxData) != HAL_OK) {
      // Handle error
      /* Proposal:
      * 1. Print an error message through the USART2 peripheral (connects to the ST-Link hence the computer)
      * 2. Turn on a LED, preferably the red one (LD2) to indicate an error
      */
    }

    osSemaphoreRelease(xCANSemaphore);
  }
}

void InitCANSemaphore(void) {
  xCANSemaphore = xSemaphoreCreateBinary();

  if (xCANSemaphore == NULL) {
    /* Handle error */
  }
}
