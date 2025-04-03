
/**
 * CANTask.c
 *
 * Created on: January 15, 2024
 *     Author: Erick Daniel Ortiz Cervantes
 */

#pragma once

#include "cmsis_os.h"
#include "main.h"

/**
 * @brief Represents a CAN bus message splitted into the velocity
 * in in each motor
 *
 * @param vMotorx: Value to set to the speed controller
 */
typedef struct {
    uint8_t vMotor_FL;
    uint8_t vMotor_FR;
    uint8_t vMotor_BL;
    uint8_t vMotor_BR;
} ChassisControlMessage;

/**
 * @brief Represents a CAN bus message
 *
 * @param node: Node or subsystem where the message is sent to
 * @param vMotorx: Value to set to the speed controller
 */
typedef struct {
    uint8_t vMotor_yaw;
    uint8_t vMotor_pitch;
} GimballControlMessage;

/**
 * @brief Represents a Control Data Message
 */
#pragma pack(push, 1)
typedef struct control_data {
    uint8_t joystickAx;
    uint8_t joystickAy;
    uint8_t joystickBx;
    uint8_t joystickBy;
    uint8_t knobA;
    uint8_t knobB;
    uint8_t switchA;
    uint8_t switchB;
    uint8_t switchC;
    uint8_t switchD;
} control_data;
#pragma pack(pop)

// Message queues from protocols
extern osPoolId can_rx_mpool;
extern osPoolId can_tx_mpool;
extern osPoolId remote_pool;

extern osMessageQId outputQueueChassis;
extern osMessageQId inputQueueChassis;

extern osMessageQId remoteQueue;
