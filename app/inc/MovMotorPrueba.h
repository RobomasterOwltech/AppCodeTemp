#ifndef MOVMOTORPRUEBA_H
#define MOVMOTORPRUEBA_H

#include "main.h"
#include "cmsis_os.h"
#include "tim.h"

extern TIM_HandleTypeDef htim3; // Declaración del timer existente

/* Tarea PWM para control del motor */
void StartPWMTxTask(void const * argument) {
    uint32_t dutyCycle = 0; // Ciclo de trabajo inicial (0%)

    // Iniciar PWM en el canal 1 del timer 3
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
        // Manejar error si el PWM no inicia correctamente
        Error_Handler();
    }

    for(;;) {
        // Incrementar el ciclo de trabajo
        dutyCycle += 100; // Incrementar en 10% (100/1000)
        if (dutyCycle > 1000) {
            dutyCycle = 0; // Reiniciar si excede el 100%
        }

        // Actualizar el ciclo de trabajo
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dutyCycle);

        // Esperar 1 segundo
        osDelay(1000);
    }
}

#endif /* MOVMOTORPRUEBA_H */