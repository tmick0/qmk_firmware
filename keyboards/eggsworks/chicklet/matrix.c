// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "matrix.h"
#include "pca9555.h"
#include "debounce.h"

// I2C addresses for each half's pca9555
#define LHS (0b01000000)
#define RHS (0b01000010)

// Bitshift and mask for reading the columns
#define HALF (MATRIX_COLS >> 1)
#define MASK ((1 << HALF) - 1)

// State
static matrix_row_t matrix[MATRIX_ROWS] = {0};
static matrix_row_t debounced[MATRIX_ROWS] = {0};
static bool status = true;

static void init_gpio(const uint8_t addr) {
    pca9555_init(addr);

    // Each pca9555 has cols on port0 and rows on port1
    // Configure them as input and output respectively
    pca9555_set_config(addr, PCA9555_PORT0, ALL_INPUT);
    pca9555_set_config(addr, PCA9555_PORT1, ALL_OUTPUT);
    // pca9555_set_output(addr, PCA9555_PORT1, 0);
}

void matrix_init(void) {
    init_gpio(LHS);
    init_gpio(RHS);
    debounce_init(MATRIX_ROWS);
}

uint8_t matrix_scan(void) {
    bool change = false;

    // If the split connection was lost, reinitialize the right side
    if (!status) {
        init_gpio(RHS);
    }

    // Scan both lhs and rhs in parallel
    for (uint8_t row = 0; row < MATRIX_ROWS / 2; ++row) {
        // Set row high
        pca9555_set_output(LHS, PCA9555_PORT1, 1 << row);
        pca9555_set_output(RHS, PCA9555_PORT1, 1 << row);

        // todo: delay here?

        // Read columns
        uint8_t cols_lhs, cols_rhs;
        pca9555_readPins(LHS, PCA9555_PORT0, &cols_lhs);
        status = pca9555_readPins(RHS, PCA9555_PORT0, &cols_rhs);

        // Update matrix
        const matrix_row_t prev = matrix[row];
        matrix[row] = ((cols_rhs & MASK) << HALF) | (cols_lhs & MASK);
        change |= (prev != matrix[row]);
    }

    return debounce(matrix, debounced, MATRIX_ROWS, change);
}