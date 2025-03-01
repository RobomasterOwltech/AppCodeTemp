#include "SPI_task.h"

#include <cmsis_os.h>

#include "main.h"
#ifndef __SPI_task.c__
    #define __SPI_task .c__
    #ifdef __cplusplus
extern "C" {
    #endif

buffer_spi[5];  // esta en bytes
// es un array para guardar los datos del acelerometro y giroscopio en memoria

SPI_Config SPI1_Config = {.spiHandle = &hspi2, .csPort = SPI1_CS_GPIO_Port, .csPin = SPI1_CS_Pin};

// funcion para leer un registro de 16 bits
uint16_t todos_addres() {
    for (int i = 0; i < 128; i++) {
        uint16_t data = IMU_ReadRegister8(i);
        if (data != 0) {
            printf("0x%02X: 0x%02X\n", i, data);
        }
    }
}

// las cosas del semaforo
osSemaphoreId I2C_semaphore;    // define el semaforo
osSemaphoreDef(I2C_semaphore);  // define la estructura del semaforo

void initSemaphore(void) {
    I2C_semaphore = osSemaphoreCreate(osSemaphore(I2C_semaphore), 1);  // Crear semáforo con 1 permiso
    if (I2C_semaphore == NULL) {
    }
}

I2C_HandleTypeDef hi2c1;

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    if (hi2c->State == HAL_I2C_STATE_READY) {
        osSemaphoreRelease(I2C_semaphore);
    }
}

void ProcesarDatos() {
    // leer datos del acelerometro
    int16_t ax = IMU_ReadRegister16(0x3B);  // Eje X
    int16_t ay = IMU_ReadRegister16(0x3D);  // Eje y
    int16_t az = IMU_ReadRegister16(0x3F);  // Eje z

    // leer datos del giroscopio
    int16_t gx = IMU_ReadRegister16(0x43);  // Eje X
    int16_t gy = IMU_ReadRegister16(0x45);  // Eje y
    int16_t gz = IMU_ReadRegister16(0x47);  // Eje z

    // convertir los datos a unidade fisicas
    float x1g = ax * (9.81f / 16384.0f);
    float y1g = ay * (9.81f / 16384.0f);
    float z1g = az * (9.81f / 16384.0f);
    // quedan pendiente de asignacion para su uso
    float x2g = gx * (9.81f / 16384.0f);
    float y2g = gy * (9.81f / 16384.0f);
    float z2g = gz * (9.81f / 16384.0f);
}

    #ifdef __cplusplus
}
    #endif

#endif /* __SPI_task.h__ */
