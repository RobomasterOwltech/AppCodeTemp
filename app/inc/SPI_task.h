#include "cmsis_os.h"
#include "spi.h"


typedef struct {
    SPI_HandleTypeDef* spiHandle; 
    GPIO_TypeDef* csPort;         
    uint16_t csPin;  
} SPI_Config;
uint8_t buffer[40];
float vel_x,  vel_y,  vel_z; //velocidad variables
float acce_x, acce_y, acce_z; //aceleracion variables
float gir_x, gir_y,  gir_z; //giroscopio variable


/*---------------------------------------------------------------*/
/*la cosa  del semaforo que debo implementar*/
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);

static void Task1(void *argument);

//*-----------------------------------------------------------------------*/


 //Funciones para manejar SPI y datos del robot
void SPI_Init(SPI_HandleTypeDef *hspi);  // Inicializar SPI
void SPI_ReadData(SPI_HandleTypeDef *hspi, uint8_t *buffer, uint16_t len); // Leer datos SPI (le puse el nombre de read data)
void SPI_WriteSensor(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t len);  // Escribir datos SPI


//Declaracion de funciones de SPI
int16_t INU_ReadResgister16(uint8_t reg);

 // Calcular velocidad y aceleración


//a lo que invesige el metodo de conseguir los datos 
void ReadData(float *x1, float *y1, float *z1, 
    float *x2, float *y2, float *z2) {
//si falla lo de 0x02 probar poner command que esta declarado arriba

uint8_t command = 0x02;
SPI_Transfer(&hspi2, 0x02); //aqui dice ques no esta definido
for (int i = 0; i < 40; i++) {
buffer[i] = SPI_Transfer(&hspi2, 0x00);
}
*x1 = *((float*)&buffer[0]);
*y1 = *((float*)&buffer[4]);
*z1 = *((float*)&buffer[8]);
*x2 = *((float*)&buffer[12]);
*y2 = *((float*)&buffer[16]);
*z2 = *((float*)&buffer[20]);
}