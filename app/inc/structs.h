
/**
 * CANTask.c
 * 
 * Created on: January 15, 2024
 *     Author: Erick Daniel Ortiz Cervantes
 */

#pragma once

#include "main.h"

/**
 * @brief Represents a subsystem within the CAN bus
 */
typedef enum { GIMBALL, CHASSIS } Node;

/**
 * @brief Represents a CAN bus message
 * 
 * @param node: Node or subsystem where the message is sent to
 * @param vMotorx: Value to set to the speed controller
 */
typedef struct {
    Node node;
    uint8_t vMotor1;
    uint8_t vMotor2;
    uint8_t vMotor3;
    uint8_t vMotor4;
} MotorControlMessage;
