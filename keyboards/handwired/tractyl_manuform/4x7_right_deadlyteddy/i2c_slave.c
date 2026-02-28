#include "i2c_slave.h"


uint8_t i2c_slave_reg[I2C_SLAVE_REG_COUNT] = {0};


void i2c_slave_init(uint8_t address) {
    (void)address;
}