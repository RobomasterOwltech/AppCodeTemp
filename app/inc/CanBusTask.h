/*
 * CanBusTask.h
 *
 *  Created on: Nov 12, 2024
 *      Author: @JorgePErc
 */

#ifndef CanBusTask_H
    #define CanBusTask_H

    #ifdef __cplusplus
extern "C" {
    #endif

    #include <FreeRTOS.h>
    #include <stream_buffer.h>
    #include <task.h>

    #include "RobomasterCanProtocol.h"
    #include "robotPins.h"

// Make an if to know if
// CAN_HandleTypeDef* hcan;
// already exists

    /* Task defines Protocol
     */
    #define STACK_SIZE 128

    // Supossedly the bitrate is 1Mbps
    // Tho, the supported baud rate is 1000 kbps
    #define CAN_BITRATE
    // Each motor sends it's info 1 KHz
    #define CAN

// See https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Packed-Structures.html

typedef struct __attribute__((__packed)) {
    uint8_t SOF : 1;
    uint16_t ID : 11;
    uint8_t RTR : 1;
    uint8_t Reserved : 1;
    uint8_t DLC : 4;
} beginCANFrame;

typedef struct __attribute__((__packed)) {
    // TODO: Check what is CRC_TypeDef
    uint16_t CRC : 15;
    uint8_t CRCDeliter : 1;
    uint8_t Ack : 1;
    uint8_t AckDelimiter : 1;
    uint8_t EOF : 7;
} endCANFrame;

static uint8_t frameLength = sizeof(beginCANFrame) + (RM_DLC) * 8 + sizeof(endCANFrame);

static CAN_TxHeaderTypeDef tx_message;

CAN_RxHeaderTypeDef* RxHeader;
CAN_TxHeaderTypeDef* TxHeader;

static uint8_t rxData[RM_DLC];
static uint8_t txData[RM_DLC];

static StreamBufferHandle_t rxStream = NULL;

static bool rxInProgress = false;
static uint_fast16_t rxLen = 0;

static DMA_HandleTypeDef usart2DmaRx;

// Queues for receiving data
osMessageQId queue_id;               // Define the queue handle
osMessageQDef(queue, 10, uint32_t);  // Define the queue with 10 elements of uint32_t type

// Create the queue
queue_id = osMessageCreate(osMessageQ(queue), NULL);

    #ifdef __cplusplus
}
    #endif

#endif /* CanBusTask_H */

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