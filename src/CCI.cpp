#include "stm32f2xx.h"

// Assume device address is 7-bit
#define I2C_SLAVE_ADDRESS 0x50

void I2C_Start_Transmission(I2C_TypeDef *I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress) {
    // Generate the start condition
    I2Cx->CR1 |= I2C_CR1_START;

    // Wait for the start condition to be sent
    while (!(I2Cx->SR1 & I2C_SR1_SB));

    // Send the slave address and the read/write bit
    I2Cx->DR = (slaveAddress << 1) | transmissionDirection;

    // Wait for the address to be sent
    while (!(I2Cx->SR1 & I2C_SR1_ADDR));

    // Clear the ADDR flag by reading SR1 and SR2
    volatile uint32_t temp = I2Cx->SR2;
    (void)temp;
}

void I2C_Write_Data(I2C_TypeDef *I2Cx, uint8_t data) {
    // Send the data
    I2Cx->DR = data;

    // Wait for the byte to be transmitted
    while (!(I2Cx->SR1 & I2C_SR1_TXE));
}

void write_command(uint16_t command) {
    // Start I2C transmission in write mode
    I2C_Start_Transmission(I2C1, 0, I2C_SLAVE_ADDRESS);

    // Send the command high byte
    I2C_Write_Data(I2C1, (command >> 8) & 0xFF);

    // Send the command low byte
    I2C_Write_Data(I2C1, command & 0xFF);

    // Generate the stop condition
    I2C1->CR1 |= I2C_CR1_STOP;
}

int main() {
    // Initialize I2C1 (not shown, depends on your specific setup)
    // Initialize GPIOs for I2C1 (not shown, depends on your specific setup)

    // Example command to write
    write_command(0xABCD);

    while(1) {
    }
    return 0;
}
