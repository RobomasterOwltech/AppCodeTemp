#include "main.h"
#include "robotconfig.h"
#include "SPI_task.h"


SPI_Config SPI1_Config = {
    .spiHandle = &hspi1,
    .csPort = SPI1_CS_PORT,
    .csPin = SPI1_CS_PIN
};

// Inicia SPI
void SPI_Init(SPI_Config* config) {
    HAL_GPIO_WritePin(config->csPort, config->csPin, GPIO_PIN_SET); 
}

// envia y recibe
uint8_t SPI_Transfer(SPI_Config* config, uint8_t data) {
    uint8_t receivedData;
    HAL_GPIO_WritePin(config->csPort, config->csPin, GPIO_PIN_RESET); 
    HAL_SPI_TransmitReceive(config->spiHandle, &data, &receivedData, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(config->csPort, config->csPin, GPIO_PIN_SET);  
    return receivedData;
}
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->State == HAL_I2C_STATE_READY)
    {
        osSemaphoreRelease(I2C_semaphore);
    }
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->State == HAL_I2C_STATE_READY)
    {
        osSemaphoreRelease(I2C_semaphore);
    }
}
static void Task1(void *argument)
{
    while(1)
    {
        HAL_I2C_Master_Transmit_IT(&hi2c1, I2C_addr, write_data_array, sizeof(write_data_array));
        osSemaphoreAcquire(I2C_semaphore, 100);
        HAL_I2C_Master_Receive_IT(&hi2c1, I2C_addr, read_data_array, sizeof(read_data_array));
        osSemaphoreAcquire(I2C_semaphore, 100);
    }
}


int main(void) {
    HAL_Init();
    tim(); 
    MX_GPIO_Init();
    MX_SPI1_Init();

    
    SPI_Init(&SPI1_Config);
    //esta parte para mandar a llamar al semaforo que esta guardado
    I2C_semaphore = osSemaphoreNew(1, 0, NULL);

    osKernelInitialize();
    osThreadNew(Task1, NULL, NULL);
    osKernelStart();

   //aqui la repeticion de spi
    while (1) {
    
        uint8_t command = 0x01; 
        uint8_t response = SPI_Transfer(&SPI1_Config, command);

        HAL_Delay(1000);
    }
}
