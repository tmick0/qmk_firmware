// Copyright 2022 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x3665
#define DEVICE_VER      0x0002
#define MANUFACTURER    tmick0
#define PRODUCT         egg58

#define MATRIX_ROWS 10
#define MATRIX_COLS 6

#define MATRIX_ROW_PINS { C6, D7, E6, B4, B5 }
#define MATRIX_COL_PINS { F6, F7, B1, B3, B2, B6 }
#define DIODE_DIRECTION COL2ROW

#define TAPPING_TERM 100
#define DEBOUNCE 5
#define TAP_CODE_DELAY 10

#define USE_I2C

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define MASTER_LEFT
#define SPLIT_USB_DETECT
#define SPLIT_MAX_CONNECTION_ERRORS 0

#define RGB_DI_PIN D3
#define RGBLIGHT_SPLIT
#define RGBLED_NUM 58
#define RGBLED_SPLIT { 29, 29 }
#define RGBLIGHT_LIMIT_VAL 160
#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF
#define RGBLIGHT_SLEEP
#define RGB_DISABLE_WHEN_USB_SUSPENDED
