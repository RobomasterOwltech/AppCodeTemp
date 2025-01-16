
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

#include "structs.h"

#define PENDING_MESSAGES 1
#define NO_PENDING_MESSAGES 0

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

MotorControlMessage TxData;
MotorControlMessage RxData;

uint32_t TxMailbox;

uint8_t hasPendingMessages = 0;

osThreadId_t CANTaskHandle;
const osThreadAttr_t CANTask_attributes = {
  .name = "CANTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

static void MX_CAN_Init(void);
void StartCANTask(void *argument);

void StartCANTask(void *argument) {
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  while (1) {
    if (hasPendingMessages) {
      // Send the message to the corresponding node or subsystem
      // TODO: Implement the logic to retrived data from the remote controller
      TxData.vMotor1 = RxData.vMotor1;
      TxData.vMotor2 = RxData.vMotor2;
      TxData.vMotor3 = RxData.vMotor3;
      TxData.vMotor4 = RxData.vMotor4;
      HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);

      // Reset the flag
      hasPendingMessages = NO_PENDING_MESSAGES;

      // Report current data from the CAN bus
      // TODO: Implement the logic to retrieve data from the CAN bus nodes (chassis and gymbal)
      TxData.vMotor1 = RxData.vMotor1;
      TxData.vMotor2 = RxData.vMotor2;
      TxData.vMotor3 = RxData.vMotor3;
      TxData.vMotor4 = RxData.vMotor4;
      HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    }
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    // Handle error
    /* Proposal:
     * 1. Print an error message through the USART2 peripheral (connects to the ST-Link hence the computer)
     * 2. Turn on a LED, preferably the red one (LD2) to indicate an error
     */
  }

  // Set a flag to indicate that a message has been received
  hasPendingMessages = PENDING_MESSAGES;
}
