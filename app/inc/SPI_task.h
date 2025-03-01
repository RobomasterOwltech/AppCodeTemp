#include "cmsis_os.h"
#include "spi.h"
#ifndef __SPI_task.h__
    #define __SPI_task .h__
    #ifdef __cplusplus
extern "C" {
    #endif

extern SPI_HandleTypeDef hspi2;  // esta en el archivo de spi

typedef struct {
    SPI_HandleTypeDef* spiHandle;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
} SPI_Config;
uint8_t buffer[40];
float vel_x, vel_y, vel_z;     // velocidad variables
float acce_x, acce_y, acce_z;  // aceleracion variables
float gir_x, gir_y, gir_z;     // giroscopio variable

/*---------------------------------------------------------------*/
/*la cosa  del semaforo que debo implementar*/
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c);

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c);

static void Task1(void const* argument);

//*-----------------------------------------------------------------------*/

// Funciones para manejar SPI y datos del robot
void SPI_Init(SPI_HandleTypeDef* hspi);  // Inicializar SPI
void SPI_ReadData(SPI_HandleTypeDef* hspi, uint8_t* buffer,
                  uint16_t len);  // Leer datos SPI (le puse el nombre de read data)
void SPI_WriteSensor(SPI_HandleTypeDef* hspi, uint8_t* data, uint16_t len);  // Escribir datos SPI

// Declaracion de funciones de SPI
int16_t INU_ReadResgister16(uint8_t reg);

// Calcular velocidad y aceleración

// a lo que invesige el metodo de conseguir los datos
void ReadData(float* x1, float* y1, float* z1, float* x2, float* y2, float* z2) {
    // correccion de las cosas del buffer
    uint8_t command = 0x02;
    uint8_t buffer[40];

    // Enviar comando
    uint8_t txData = 0x02;
    uint8_t rxData;
    HAL_SPI_TransmitReceive(&hspi2, &txData, &rxData, 1, HAL_MAX_DELAY);

    // Leer 40 bytes de datos
    for (int i = 0; i < 40; i++) {
        txData = 0x00;  // Enviar datos vacíos para recibir respuesta
        HAL_SPI_TransmitReceive(&hspi2, &txData, &buffer[i], 1, HAL_MAX_DELAY);
    }

    // Convertir los datos en valores float
    float* x1 = (float*)&buffer[0];
    float* y1 = (float*)&buffer[4];
    float* z1 = (float*)&buffer[8];
    float* x2 = (float*)&buffer[12];
    float* y2 = (float*)&buffer[16];
    float* z2 = (float*)&buffer[20];

    #ifdef __cplusplus
}
    #endif

#endif /* __SPI_task.h__ */