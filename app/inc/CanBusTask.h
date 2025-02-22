
/**
 * CANTask.c
 *
 * Created on: January 12, 2024
 *     Author: Erick Daniel Ortiz Cervantes
 *  Reference: https://www.youtube.com/watch?v=KHNRftBa1Vc
 */

#pragma once

#include "CommunicationStructs.h"
#include "RobomasterCanProtocol.h"
#include "cmsis_os.h"
#include "main.h"
#include "semphr.h"

// =======================================================
// Robomaster protocol
// =======================================================
#define STACK_SIZE 128

// TODO: define which one should stayed defined, and it's value
// Supposedly the bitrate is 1Mbps
// Tho, the supported baud rate is 1000 kbps
#define CAN_BITRATE
#define CAN_BAUDRATE
// Each motor sends it's info 1 KHz

// On GNU arm you can't use __attribute__((__packed))
// See https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Packed-Structures.html

#pragma pack(1)
typedef struct {
    uint8_t SOF : 1;
    uint16_t ID : 11;
    uint8_t RTR : 1;
    uint8_t Reserved : 1;
    uint8_t DLC : 4;
} beginCANFrame;

#pragma pack(1)
typedef struct {
    // TODO: Check what is CRC_TypeDef
    uint16_t CRC_ : 15;
    uint8_t CRCDeliter : 1;
    uint8_t Ack : 1;
    uint8_t AckDelimiter : 1;
    uint8_t EOF : 7;
} endCANFrame;

static uint8_t CANframeLength = sizeof(beginCANFrame) + (RM_DLC) * 8 + sizeof(endCANFrame);

// =======================================================

// =======================================================
// Communication structs
// =======================================================

ChassisControlMessage TxData;
ChassisControlMessage RxData;

uint32_t TxMailbox;

SemaphoreHandle_t xCANSemaphore;

void StartCANTxTask(void* argument);
void StartCANRxTask(void* argument);

osThreadDef(StartCANTxTask, osPriorityNormal, 3, 0, 128 * 4);
osThreadDef(StartCARRxTask, osPriorityNormal, 3, 0, 128 * 4);

void StartCANTxTask(void* argument) {
    while (1) {
        // TODO: Implement TX logic
    }
}

void StartCANRxTask(void* argument) {
    while (1) {
        if (osSemaphoreAcquire(xCANSemaphore, osWaitForever) == osOK) {
            // Send the message to the corresponding node or subsystem
            // TODO: Implement the logic to retrieved data from the remote controller
            TxData.vMotor_FL = RxData.vMotor_FL;
            TxData.vMotor_FR = RxData.vMotor_FR;
            TxData.vMotor_BL = RxData.vMotor_BL;
            TxData.vMotor_BR = RxData.vMotor_BR;
            osSemaphoreAcquire(xCANSemaphore, osWaitForever);
            HAL_CAN_AddTxMessage(&hcan, &TxHeaderCan, TxData, &TxMailbox);

            osDelay(100);
        }
    }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->State == HAL_CAN_STATE_READY) {
        osSemaphoreRelease(xCANSemaphore);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->State == HAL_CAN_STATE_READY) {
        if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeaderCan, (uint8_t*)&RxData) != HAL_OK) {
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

// =======================================================
// STREAM BUFFER --- nOT USED FOR NOW
// =======================================================

/*
static uint8_t rxData[RM_DLC];
static uint8_t txData[RM_DLC];

static StreamBufferHandle_t rxStream = NULL;

static bool rxInProgress = false;
static uint_fast16_t rxLen = 0;

// There is no DMA for can
// static DMA_HandleTypeDef usart2DmaRx;

// Queues for receiving data
osMessageQId queue_id;               // Define the queue handle
osMessageQDef(queue, 10, uint32_t);  // Define the queue with 10 elements of uint32_t type

// Create the queue
queue_id = osMessageCreate(osMessageQ(queue), NULL);
*/

// ==================
/*
file:///Users/jperezch/Downloads/um2217-description-of-stm32h7-hal-and-lowlayer-drivers-stmicroelectronics.pdf


  ==============================================================================
                      ##### Control functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) HAL_CAN_Start                    : Start the CAN module
      (+) HAL_CAN_Stop                     : Stop the CAN module
      (+) HAL_CAN_RequestSleep             : Request sleep mode entry.
      (+) HAL_CAN_WakeUp                   : Wake up from sleep mode.
      (+) HAL_CAN_IsSleepActive            : Check is sleep mode is active.
      (+) HAL_CAN_AddTxMessage             : Add a message to the Tx mailboxes
                                             and activate the corresponding
                                             transmission request
      (+) HAL_CAN_AbortTxRequest           : Abort transmission request
      (+) HAL_CAN_GetTxMailboxesFreeLevel  : Return Tx mailboxes free level
      (+) HAL_CAN_IsTxMessagePending       : Check if a transmission request is
                                             pending on the selected Tx mailbox
      (+) HAL_CAN_GetRxMessage             : Get a CAN frame from the Rx FIFO
      (+) HAL_CAN_GetRxFifoFillLevel       : Return Rx FIFO fill level

*/