// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "tamago60.h"

enum layer_names {
    _BASE,
    _FN
};

static uint8_t layer = _BASE;
static uint8_t scroll_ticks = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,  \
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS, \
        KC_LCTL, KC_LGUI, KC_LALT, MO(_FN),     KC_SPC,       KC_BTN1, KC_BTN2,     KC_ENT,       MO(_FN), KC_LBRC, KC_RBRC, KC_BSPC
    ),
    [_FN] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  \
        KC_TRNS, KC_PGUP, KC_UP,   KC_PGDN, KC_TRNS, KC_VOLU,                   KC_TRNS, KC_TRNS, KC_INS,  KC_TRNS, KC_PSCR, KC_F12,  \
        KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_VOLD,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_HOME, KC_TRNS, KC_END,  KC_TRNS, KC_MUTE,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,      KC_BTN3, KC_BTN4,     KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    layer = get_highest_layer(state);
    return state;
}

static inline int8_t sign(const int8_t x) {
    return ((x == 0) ? 0 : (x < 0) ? -1 : 1);
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (layer >= _FN) {
        // Translate move to scroll when Fn is active
        mouse_report.h = sign(mouse_report.x);
        mouse_report.v = sign(mouse_report.y);
        mouse_report.x = 0;
        mouse_report.y = 0;

        // Only send once every SCROLL_WAIT_PERIOD ticks to slow scroll down
        if (mouse_report.h || mouse_report.v) {
            if (scroll_ticks != 0) {
                mouse_report.h = 0;
                mouse_report.v = 0;
            }
            scroll_ticks = (scroll_ticks + 1) % SCROLL_WAIT_PERIOD;
        }
    }

    return mouse_report;
}
