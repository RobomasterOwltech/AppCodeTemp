#include "main.h"
#include "robotconfig.h"


typedef struct {
    SPI_HandleTypeDef* spiHandle; 
    GPIO_TypeDef* csPort;         
    uint16_t csPin; //esto directamente nose, pero venia en un ejemplo que vi 
} SPI_Config;

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

int main(void) {
    HAL_Init();
    tim(); //voy a creer que es lo del relog
    MX_GPIO_Init();
    MX_SPI1_Init();

    
    SPI_Init(&SPI1_Config);

    while (1) {
    
        uint8_t command = 0x01; 
        uint8_t response = SPI_Transfer(&SPI1_Config, command);

        HAL_Delay(1000); // awanta 1 segundo
    }
}
