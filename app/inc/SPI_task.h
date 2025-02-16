#include "cmsis_os.h"
#include "robotconfig/inc/spi.h"
//esta llamada no existe pero cuando me mezcle por la branch puede que no haya problema


float vel_x, float vel_y, float vel_z;
float acce_x,float acce_y,float acce_z;


//---------------------------------------------------------------
//la cosa  del semaforo que debo implementar
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);

static void Task1(void *argument);

//-------------------------------------------------------------------------


 //Funciones para manejar SPI y datos del robot
void SPI_Init(SPI_HandleTypeDef *hspi);  // Inicializar SPI
void SPI_ReadData(SPI_HandleTypeDef *hspi, uint8_t *buffer, uint16_t len); // Leer datos SPI (le puse el nombre de read data)
void SPI_WriteSensor(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t len);  // Escribir datos SPI


//Declaracion de funciones de SPI
int16_t INU_ReadResgister16(uinit8_t reg);

void ProcesarDatos(){

//leer datos del acelerometro
int16_t ax= IMU_ReadREegister16(0x3B); //Eje X
int16_t ay= IMU_ReadREegister16(0x3D); //Eje y
int16_t az= IMU_ReadREegister16(0x3F); //Eje z

//leer datos del giroscopio
int16_t gx= IMU_ReadREegister16(0x43); //Eje X
int16_t gy= IMU_ReadREegister16(0x45); //Eje y
int16_t gz= IMU_ReadREegister16(0x47); //Eje z

//convertir los datos a unidade fisicas
float x1g= ax * (9.81f / 16384.0f);
float y1g= ay * (9.81f / 16384.0f);
float z1g= az * (9.81f / 16384.0f);

float x2g= gx * (9.81f / 16384.0f);
float y2g= gy * (9.81f / 16384.0f);
float z2g= gz * (9.81f / 16384.0f);


}; // Calcular velocidad y aceleración


//a lo que invesige el metodo de conseguir los datos 
void ReadData(float *x1, float *y1, float *z1, 
    float *x2, float *y2, float *z2) {
uint8_t buffer[40];
uint8_t command = 0x02;
SPI_Transfer(&robotConfig, command);
for (int i = 0; i < 40; i++) {
buffer[i] = SPI_Transfer(&SPI1_Config, 0x00);
}
*x1 = *((float*)&buffer[0]);
*y1 = *((float*)&buffer[4]);
*z1 = *((float*)&buffer[8]);
*x2 = *((float*)&buffer[12]);
*y2 = *((float*)&buffer[16]);
*z2 = *((float*)&buffer[20]);
}