
/**
 * CANTask.c
 *
 * Created on: January 12, 2024
 *     Author: Erick Daniel Ortiz Cervantes
 *  Reference: https://www.youtube.com/watch?v=KHNRftBa1Vc
 */

#ifndef CANBUSTASK_H
#define CANBUSTASK_H

#include "CanBusTask.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "CommunicationStructs.h"
#include "RobomasterCanProtocol.h"
#include "can.h"
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

#define CAN_TIMEOUT_MS 5
// https://arm-software.github.io/CMSIS_5/RTOS/html/group__CMSIS__RTOS__SemaphoreMgmt.html

// Each motor sends it's info 1 KHz

// Can filter example
// https://schulz-m.github.io/2017/03/23/stm32-can-id-filter/
// On GNU arm you can't use __attribute__((__packed))
// See https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Packed-Structures.html

#pragma pack(push, 1)
typedef struct {
    uint8_t SOF : 1;
    uint16_t ID : 11;
    uint8_t RTR : 1;
    uint8_t Reserved : 1;
    uint8_t DLC : 4;
} beginCANFrame;

typedef struct {
    // TODO: Check what is CRC_TypeDef
    uint16_t CRC_ : 15;
    uint8_t CRCDeliter : 1;
    uint8_t Ack : 1;
    uint8_t AckDelimiter : 1;
    uint8_t EOF : 7;
} endCANFrame;
#pragma pack(pop)

// static uint8_t CANframeLength = sizeof(beginCANFrame) + (RM_DLC) * 8 + sizeof(endCANFrame);

// =======================================================

// =======================================================
// Communication structs
// =======================================================
void CANBusThread(void const* argument);

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan);

// =======================================================
// STREAM BUFFER --- nOT USED FOR NOW
// =======================================================

/*
CAN Interrupts
    CAN_IT_TX_MAILBOX_EMPTY     || Transmit mailbox empty interrupt
    CAN_IT_RX_FIFO0_MSG_PENDING || FIFO 0 message pending interrupt
    CAN_IT_RX_FIFO0_FULL        || FIFO 0 full interrupt
    CAN_IT_RX_FIFO0_OVERRUN     || FIFO 0 overrun interrupt
    CAN_IT_RX_FIFO1_MSG_PENDING || FIFO 1 message pending interrupt
    CAN_IT_RX_FIFO1_FULL        || FIFO 1 full interrupt
    CAN_IT_RX_FIFO1_OVERRUN     || FIFO 1 overrun interrupt
    CAN_IT_WAKEUP               || Wake-up interrupt
    CAN_IT_SLEEP_ACK            || Sleep acknowledge interrupt
    CAN_IT_ERROR_WARNING        || Error warning interrupt
    CAN_IT_ERROR_PASSIVE        || Error passive interrupt
    CAN_IT_BUSOFF               || Bus-off interrupt
    CAN_IT_LAST_ERROR_CODE      || Last error code interrupt
    CAN_IT_ERROR                || Error Interrupt

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

#ifdef __cplusplus
}
#endif

#endif /* CANBUSTASK_H */