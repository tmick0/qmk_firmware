// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include "matrix.h"
#include "pca9555.h"
#include "i2c_master.h"

// I2C addresses for each half's pca9555
#define LHS (0b0100000)
#define RHS (0b0100001)

// State
static bool rhs_present = false;
static uint8_t rhs_err_count = 0;

// Macros for I2C communication
#define SLAVE_TO_ADDR(n) (n << 1)
#define TIMEOUT 100

// Commands for pca9555 input polarity inversion
enum {
    CMD_INVERSION_0 = 4,
    CMD_INVERSION_1
};

static bool pca9555_set_polarity(uint8_t slave_addr, pca9555_port_t port, uint8_t conf) {
    uint8_t addr = SLAVE_TO_ADDR(slave_addr);
    uint8_t cmd  = port ? CMD_INVERSION_1 : CMD_INVERSION_0;
    i2c_status_t ret = i2c_writeReg(addr, cmd, &conf, sizeof(conf), TIMEOUT);
    return ret == I2C_STATUS_SUCCESS;
}

static bool init_gpio(const uint8_t addr) {
    pca9555_init(addr);
    return pca9555_set_config(addr, PCA9555_PORT1, ALL_INPUT)
        && pca9555_set_config(addr, PCA9555_PORT0, ALL_OUTPUT)
        && pca9555_set_output(addr, PCA9555_PORT0, ALL_HIGH)
        && pca9555_set_polarity(addr, PCA9555_PORT1, ALL_HIGH);
}

void matrix_init_custom(void) {
    init_gpio(LHS);
    rhs_present = init_gpio(RHS);
}

uint8_t matrix_scan_custom(matrix_row_t matrix[]) {
    bool change = false;
    if (!rhs_present && ++rhs_err_count == 0) {
        if (init_gpio(RHS)) {
            rhs_present = true;
            rhs_err_count = 0;
        }
    }
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        const matrix_row_t prev = matrix[row];
        memset(&matrix[row], 0x00, sizeof(matrix_row_t));
        if (rhs_present || row < MATRIX_ROWS / 2) {
            const uint8_t addr = row >= MATRIX_ROWS / 2 ? RHS : LHS;
            const uint8_t bit = row >= MATRIX_ROWS / 2 ? row - MATRIX_ROWS / 2 : row;
            const bool success = pca9555_set_output(addr, PCA9555_PORT0, ALL_HIGH & ~(1 << bit))
                              && pca9555_readPins(addr, PCA9555_PORT1, &matrix[row]);
            if (!success && row >= MATRIX_ROWS / 2) {
                rhs_present = false;
            }
        }
        change |= (prev != matrix[row]);
    }
    return change;
}