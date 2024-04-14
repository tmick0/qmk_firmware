// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _SCROLL,
    _NUM,
    _NAV,
    _FN,
    _MOUSE,
};

enum custom_keycodes {
    DRAG_SCROLL = SAFE_RANGE,
};

#define M1(x) LSFT_T(x)
#define M2(x) LCTL_T(x)
#define M3(x) LGUI_T(x)
#define M4(x) LALT_T(x)

bool default_scrolling = false;
bool set_scrolling = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BASE] = LAYOUT_split_3x5_3(
       KC_Q,       KC_W,         KC_E,        KC_R, LT(_SCROLL, KC_T),                     LT(_SCROLL, KC_Y),        KC_U,        KC_I,        KC_O,        KC_P,
   M1(KC_A),   M2(KC_S),     M3(KC_D),    M4(KC_F), KC_G,                                               KC_H,    M4(KC_J),    M3(KC_K),    M2(KC_L), M1(KC_SCLN),
   M2(KC_Z),   M3(KC_X),     M4(KC_C),        KC_V, KC_B,                                               KC_N,        KC_M,     KC_COMM,      KC_DOT,     KC_SLSH,
                                           XXXXXXX, KC_LBRC, LT(_NUM, KC_SPC),     LT(_NAV, KC_ENT), KC_BSPC,     XXXXXXX
),

[_SCROLL] = LAYOUT_split_3x5_3(
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
                                           XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX,     XXXXXXX
),

[_NUM] = LAYOUT_split_3x5_3(
       KC_1,        KC_2,        KC_3,        KC_4,    KC_5,                                            KC_6,        KC_7,        KC_8,        KC_9,        KC_0,
M1(KC_EXLM),   M2(KC_AT), M3(KC_HASH),  M4(KC_DLR), KC_PERC,                                         KC_CIRC, M4(KC_AMPR), M3(KC_ASTR), M2(KC_LPRN), M1(KC_RPRN),
      KC_F1,       KC_F2,       KC_F3,       KC_F4,   KC_F5,                                          KC_GRV,     KC_QUOT,     KC_MINS,     KC_EQL,     KC_BSLS,
                                           XXXXXXX, KC_RBRC, XXXXXXX,                       MO(_FN),  KC_DEL,     XXXXXXX
),

[_NAV] = LAYOUT_split_3x5_3(
     KC_ESC,     KC_PGUP,       KC_UP,     KC_PGDN, KC_VOLU,                                         XXXXXXX,     XXXXXXX,      KC_INS,     XXXXXXX,     KC_PSCR,
    XXXXXXX,     KC_LEFT,     KC_DOWN,     KC_RGHT, KC_VOLD,                                         XXXXXXX, M4(XXXXXXX), M3(XXXXXXX), M2(XXXXXXX), M1(XXXXXXX),
    XXXXXXX,     KC_HOME,      KC_ESC,      KC_END, KC_MUTE,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
                                           XXXXXXX, XXXXXXX,  KC_TAB,                       _______, XXXXXXX,      KC_DEL
),

[_FN] = LAYOUT_split_3x5_3(
      KC_F1,       KC_F2,       KC_F3,       KC_F4,   KC_F5,                                           KC_F6,       KC_F7,       KC_F8,       KC_F9,      KC_F10,
     KC_F11,      KC_F12,     XXXXXXX,     XXXXXXX, XXXXXXX,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX,                                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
                                           XXXXXXX, XXXXXXX, XXXXXXX,                       _______, XXXXXXX,     XXXXXXX
),

[_MOUSE] = LAYOUT_split_3x5_3(
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, DRAG_SCROLL,                                 DRAG_SCROLL,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     KC_BTN3, KC_BTN4,                                         KC_BTN4,     KC_BTN3,     XXXXXXX,     XXXXXXX,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, KC_BTN5,                                         KC_BTN5,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,
                                           XXXXXXX, KC_BTN2, KC_BTN1,                       KC_BTN1, KC_BTN2,     XXXXXXX
),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(_NUM, KC_SPC):
    case LT(_NAV, KC_ENT):
      return LT_TAPPING_TERM;
    default:
      return TAPPING_TERM;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DRAG_SCROLL && record->event.pressed) {
        set_scrolling = !set_scrolling;
        if (set_scrolling) {
          pimoroni_trackball_set_rgbw(0xb0, 0x50, 0xb0, 0x00);
          pimoroni_trackball_set_cpi(SCROLL_CPI);
        } else {
          pimoroni_trackball_set_rgbw(0x80, 0x00, 0xa0, 0x00);
          pimoroni_trackball_set_cpi(MOUSE_CPI);
        }
        return true;
    }
  
    const uint16_t sh_modtaps[] = {M1(KC_EXLM), M2(KC_AT), M3(KC_HASH), M4(KC_DLR), M4(KC_AMPR), M3(KC_ASTR), M2(KC_LPRN), M1(KC_RPRN)};
    const uint16_t tap_values[] = {KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN};
    if (record->tap.count && record->event.pressed) {
      for (int i = 0; i < sizeof(sh_modtaps) / sizeof(*sh_modtaps); ++i) {
        if (keycode == sh_modtaps[i]) {
          tap_code16(tap_values[i]);
          return false;
        }
      }
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _MOUSE:
        set_scrolling = default_scrolling;
        if (set_scrolling) {
          pimoroni_trackball_set_rgbw(0xb0, 0x50, 0xb0, 0x00);
          pimoroni_trackball_set_cpi(SCROLL_CPI);
        } else {
          pimoroni_trackball_set_rgbw(0x80, 0x00, 0xa0, 0x00);
          pimoroni_trackball_set_cpi(MOUSE_CPI);
        }
        break;
    case _NUM:
        default_scrolling = false;
        pimoroni_trackball_set_rgbw(0xd0, 0xf0, 0x40, 0x00);
        break;
    case _NAV:
        default_scrolling = false;
        pimoroni_trackball_set_rgbw(0x00, 0x80, 0xd0, 0x00);
        break;
    case _FN:
        default_scrolling = false;
        pimoroni_trackball_set_rgbw(0xd0, 0x20, 0x50, 0x00);
        break;
    case _SCROLL:
      default_scrolling = true;
      break;
    case _BASE:
    default:
        default_scrolling = false;
        pimoroni_trackball_set_rgbw(0x00, 0x00, 0x00, 0x90);
        break;
    }
  return state;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = -mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE);
    set_auto_mouse_enable(true);
}
