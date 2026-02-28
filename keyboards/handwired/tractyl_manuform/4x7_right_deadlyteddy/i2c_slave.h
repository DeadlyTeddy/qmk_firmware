#pragma once

#include "i2c_master.h"

#define I2C_SLAVE_REG_COUNT 128

extern uint8_t i2c_slave_reg[I2C_SLAVE_REG_COUNT];

void i2c_slave_init(uint8_t address);