/*
 * BaseFileC.h
 *
 *  Created on: May 24, 2023
 *      Author: @yourName
 */

#ifndef CANBUSTASK_C
#define CANBUSTASK_C

#include "CanBusTask.h"
#ifdef __cplusplus
extern "C" {
#endif

// YOUR CODE
// We need at least the following principles:
// * StartReceiveInt: Sets up an interrupt-based reception for USART2 and configures the necessary variables used by the
// ISR for the transfer.
// * StartCAN4Traffic:  Starts a continuous stream of data transmitted from UART4 to be received by USART2 (provided the
// jumpers are correctly set).
// * canActionTask: This function initializes USART2 and associated hardware, starts a reception, and waits for
// completion (with a deadline of 100 ms). The complete message is either printed or a timeout occurs and an error is
// printed.
// * CAN2_IRQHandler: An ISR is issued when an interrupt occurs for the USART2 peripheral

ChassisControlMessage* canMsgSend;
ChassisControlMessage* canMsgRecd;

// struct ChassisControlMessage canMsgSend, canMsgRecd;

uint32_t TxMailbox;

osSemaphoreId xCANsem;  // Semaphore ID
osSemaphoreDef(xCANsem);

// Queues:}
// TODO: set size other than 16?

osPoolDef(can_rx_mpool, 16, ChassisControlMessage);  // Define memory pool
osPoolId can_rx_mpool;
osMessageQDef(outputQueue, 16, ChassisControlMessage);  // Define message queue
osMessageQId outputQueue;

osPoolId can_tx_mpool;
osPoolDef(can_tx_mpool, 16, ChassisControlMessage);  // Define memory pool
osMessageQId inputQueue;
osMessageQDef(inputQueue, 16, ChassisControlMessage);

uint8_t tempBuffer[8];  // Adjust size based on your data length
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    // This callback is only for the designated notification
    // Is executed from all the handlers, so we do need to identify which
    // is calling the interruption
    //  If so, accommodate for the second one

    if (hcan->State == HAL_CAN_STATE_READY) {
        // Read message, and update the current state struct
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeaderCan, tempBuffer) != HAL_OK) {
            // Handle error
            /* Proposal:
             * 1. Print an error message through the USART2 peripheral (connects to the ST-Link hence the computer)
             * 2. Turn on a LED, preferably the red one (LD2) to indicate an error
             */
        }
        // Transform received message and update the value at canMsgSend
        // This won't be published to the other tasks until all motors have updated

        // TODO: When inserting a new element into the queue, is done by value, so everything is copied
        memcpy(canMsgSend, tempBuffer, sizeof(ChassisControlMessage));

        osSemaphoreRelease(xCANsem);
    }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->State == HAL_CAN_STATE_READY) {
        osSemaphoreRelease(xCANsem);
    }
}

// TODO: use preprocesor directives to enable/disable the second CAN bus
void CANBusThread(void const* argument) {
    uint8_t tmp_tx_data[5];
    xCANsem = xSemaphoreCreateBinary();

    can_rx_mpool = osPoolCreate(osPool(can_rx_mpool));
    outputQueue = osMessageCreate(osMessageQ(outputQueue), NULL);  // create msg queue

    can_tx_mpool = osPoolCreate(osPool(can_tx_mpool));
    outputQueue = osMessageCreate(osMessageQ(outputQueue), NULL);  // create msg queue

    // We are missing the queue to listen and publish to the other tasks

    if (xCANsem == NULL) {
        /* Handle semaphore initialization error */
    }

    while (1) {
        // Have both callbacks all the time

        // Have an if for when a new message needs to be pushed
        osEvent evt;
        evt = osMessageGet(inputQueue, osWaitForever);  // wait for message
        if (evt.status == osEventMessage) {
            canMsgRecd = evt.value.p;

            // Structure from ChassisControlMessage to RM Protocol

            osPoolFree(can_tx_mpool, canMsgRecd);  // free memory allocated for message

            // Send RM CAN message
            HAL_CAN_AddTxMessage(&hcan, &TxHeaderCan, tmp_tx_data, &TxMailbox);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif /* CANBUSTASK_C */