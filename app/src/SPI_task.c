

#ifndef SPI_task_C
#define SPI_task_C

#ifdef __cplusplus
extern "C" {
#endif

#include "SPI_task.h"

#include <cmsis_os.h>
#include <stdio.h>

#include "main.h"

// unsigned char buffer_spi[40];  // esta en bytes este buffer solo sera para el spi ya tenemos uno en el task.h
//  es un array para guardar los datos del acelerometro y giroscopio en memoria

SPI_Config SPI1_Config = {.spiHandle = &hspi2, .csPort = SPI1_CS_GPIO_Port, .csPin = SPI1_CS_Pin};

// funcion para leer un registro de 16 bits
uint16_t todos_addres() {
    for (int i = 0; i < 128; i++) {
        uint16_t data = IMU_ReadResgister16(i);
        if (data != 0) {
            // printf("0x %02X: 0x%02X\n", i, data);
        }
    }
    return 0;
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
// proceso de empaquetados de datos con los datos del acelerometro y giroscopio que recibimos
// creacion strucct
typedef struct {
    float vel_x, vel_y, vel_z;  // velocidad variables
    float ace_x, ace_y, ace_z;  // aceleracion variables
    float gir_x, gir_y, gir_z;  // giroscopio variable
} IMU_Data_t;

// Definir la cola
osMessageQId imuQueue;
#define QUEUE_SIZE 10

// Definir la estructura de la cola
osMessageQDef(imuQueue, QUEUE_SIZE, IMU_Data_t);

// Pool de memoria para los datos del IMU
IMU_Data_t imuDataPool[QUEUE_SIZE];
uint8_t poolIndex = 0;

// Función para inicializar la cola
void initQueue(void) {
    imuQueue = osMessageCreate(osMessageQ(imuQueue), NULL);
    if (imuQueue == NULL) {
        // Manejar el error si la cola no se pudo crear
    }
}

// funcion para inicializar el SPI
void ProcesarDatos(void) {
    // Leer datos del acelerómetro
    int16_t ax = IMU_ReadResgister16(0x3B);  // Eje X
    int16_t ay = IMU_ReadResgister16(0x3D);  // Eje Y
    int16_t az = IMU_ReadResgister16(0x3F);  // Eje Z

    // Leer datos del giroscopio
    int16_t gx = IMU_ReadResgister16(0x43);  // Eje X
    int16_t gy = IMU_ReadResgister16(0x45);  // Eje Y
    int16_t gz = IMU_ReadResgister16(0x47);  // Eje Z

    // convertir los datos a unidade fisicas
    float x1g = ax * (9.81f / 16384.0f);
    float y1g = ay * (9.81f / 16384.0f);
    float z1g = az * (9.81f / 16384.0f);
    // quedan pendiente de asignacion para su uso
    float x2g = gx * (9.81f / 16384.0f);
    float y2g = gy * (9.81f / 16384.0f);
    float z2g = gz * (9.81f / 16384.0f);

    // Almacenar los datos en el struct
    IMU_Data_t* imuData = &imuDataPool[poolIndex];
    imuData->ace_x = x1g;
    imuData->ace_y = y1g;
    imuData->ace_z = z1g;
    imuData->gir_x = x2g;
    imuData->gir_y = y2g;
    imuData->gir_z = z2g;

    // Enviar los datos a la cola
    osStatus status = osMessagePut(imuQueue, (uint32_t)imuData, 0);
    if (status != osOK) {
        // Manejar el error si no se pudo enviar el mensaje
    }

    // Actualizar el índice del pool de memoria
    poolIndex = (poolIndex + 1) % QUEUE_SIZE;
}
void Task_ProcessIMUData(void const* argument) {
    while (1) {
        // Esperar a recibir un mensaje de la cola
        osEvent event = osMessageGet(imuQueue, osWaitForever);
        if (event.status == osEventMessage) {
            // Obtener el puntero al struct
            IMU_Data_t* imuData = (IMU_Data_t*)event.value.v;

            // Procesar los datos recibidos
            // printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", imuData->ace_x, imuData->ace_y, imuData->ace_z);
            // printf("Gyro X: %f, Gyro Y: %f, Gyro Z: %f\n", imuData->gir_x, imuData->gir_y, imuData->gir_z);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif /* SPI_task.C */
