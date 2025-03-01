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
// ChassisControlMessage* canMsgSend;
// ChassisControlMessage* canMsgRecd;

struct ChassisControlMessage canMsgSend, canMsgRecd;

uint32_t TxMailbox;

osSemaphoreId xCANsem;  // Semaphore ID
osSemaphoreDef(xCANsem);

// Queues:}
    // TODO: set size other than 16?
osPoolId  can_rx_mpool;
osPoolDef(can_rx_mpool, 16, ChassisControlMessage);                    // Define memory pool
osMessageQId  outputQueue; 
osMessageQDef(outputQueue, 16, &ChassisControlMessage);              // Define message queue

osPoolId  can_tx_mpool;
osPoolDef(can_tx_mpool, 16, ChassisControlMessage);                    // Define memory pool
osMessageQId  inputQueue; 
osMessageQDef(inputQueue, 16, &ChassisControlMessage);    

void StartCANTxTask(void* argument) {
    while (1) {
        // TODO: Implement TX logic
    }
}

void StartCANRxTask(void* argument) {
    
    
    if (osSemaphoreWait(xCANsem, CAN_TIMEOUT_MS) == osOK) {
        // Send the message to the corresponding node or subsystem
        // TODO: Implement the logic to retrieved data from the remote controller
        // THIS IS TRANSMISSION

        ChassisControlMessage  *rptr;
        osEvent  evt;
        evt = osMessageGet(inputQueue, osWaitForever);  // wait for message
        if (evt.status == osEventMessage) {
            rptr = evt.value.p;
            printf ("\nVoltage: %.2f V\n", rptr->voltage);
            printf ("Current: %.2f A\n", rptr->current);
            printf ("Number of cycles: %d\n", rptr->counter);
            osPoolFree(mpool, rptr);                  // free memory allocated for message
        }
        canMsgSend = osPoolAlloc(can_mpool);                     // Allocate memory for the message
        canMsgSend->vMotor_FL = 80; // Allocate memory for the message
        canMsgSend->vMotor_FR = 22;                        // Set the message content
        canMsgSend->vMotor_BL = 17;
        canMsgSend->vMotor_BR = 120;
        osMessagePut(MsgBox, (uint32_t)mptr, osWaitForever);  // Send Message

        HAL_CAN_AddTxMessage(&hcan, &TxHeaderCan, TxData, &TxMailbox);
        //remember to use from isr functions 
        // TODO: is the delay really necessary? shouldn't the semaphore wait that time?
        osDelay(100);
    }
    
}
void CAN1_RX0_IRQHandler(void)
{
    // Check if a message is pending in FIFO0
    if (__HAL_CAN_GET_IT_SOURCE(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING))
    {
        CAN_RxHeaderTypeDef RxHeader;
        uint8_t RxData[8];  // Adjust size based on your data length

        if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            // Process the received message
        }
    }
}

// TODO: Define size
uint8_t tempBuffer[8];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    // TODO: define if this callback is the same for all CAN handlers
    //  If so, accommodate for the second one
    if (hcan->State == HAL_CAN_STATE_READY) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, RxHeaderCan, tempBuffer) != HAL_OK) {
            // Handle error



            /* Proposal:
             * 1. Print an error message through the USART2 peripheral (connects to the ST-Link hence the computer)
             * 2. Turn on a LED, preferably the red one (LD2) to indicate an error
             */
        }
        // Transform recieved message and push it to the other tasks
            // TODO: When inserting a new element into the queue, is done by value, so everything is copied
        memcpy(&myStruct, data, sizeof(myStruct));
        
        osSemaphoreRelease(xCANsem);
    }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->State == HAL_CAN_STATE_READY) {
        osSemaphoreRelease(xCANsem);
    }
}

// TODO: use preprocesor directives to enable/disable the second CAN bus
static void canBusTask(void* argument) {
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
        HAL_CAN_TxMailbox0CompleteCallback();
        StartCANRxTask();
    }
}

#ifdef __cplusplus
}
#endif

#endif /* TemplateCFile_C */