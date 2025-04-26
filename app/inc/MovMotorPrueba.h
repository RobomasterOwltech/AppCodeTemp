#include "CanBusTask.h"


void StartCANTxTask(void* argument) {
    uint8_t speed = 0; // Velocidad inicial
    CAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8] = {0}; // Datos CAN (8 bytes)
    uint32_t txMailbox;

    // Configurar el encabezado del mensaje CAN
    txHeader.StdId = CONTROL_ID_1FF; // ID del motor
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.IDE = CAN_ID_STD;
    txHeader.DLC = RM_DLC; // Longitud de datos (8 bytes)

    for (;;) {
        // Incrementar la velocidad
        speed += 10; // Incremento de 10 unidades por segundo
        if (speed > 255) speed = 0; // Reiniciar si supera el máximo

        // Configurar los datos del mensaje
        txData[0] = speed; // Velocidad en el primer byte
        for (int i = 1; i < 8; i++) {
            txData[i] = 0; // Rellenar con ceros
        }

        // Enviar el mensaje CAN
        if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, txData, &txMailbox) != HAL_OK) {
            // Manejar error de transmisión
        }

        // Esperar 1 segundo
        osDelay(1000);
    }
}