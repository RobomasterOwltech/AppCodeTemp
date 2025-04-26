#include "main.h"
    #include "cmsis_os.h"
    #include "robotConfig.h" // Asegúrate de incluir el archivo donde se define htim3

    extern TIM_HandleTypeDef htim3; // Declaración del temporizador existente

    void StartPWMTxTask(void* argument) {
        uint32_t dutyCycle = 0; // Ciclo de trabajo inicial (0%)

        // Iniciar el PWM en el canal 1 del temporizador 3
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

        for (;;) {
            // Incrementar el ciclo de trabajo
            dutyCycle += 100; // Incremento de 10% (100/1000)
            if (dutyCycle > 1000) dutyCycle = 0; // Reiniciar si supera el 100%

            // Actualizar el ciclo de trabajo
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dutyCycle);

            // Esperar 1 segundo
            osDelay(1000);
        }
    }