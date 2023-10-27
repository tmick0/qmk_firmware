// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _NUM,
    _NAV,
    _FN
};

#define M1(x) LSFT_T(x)
#define M2(x) LCTL_T(x)
#define M3(x) LGUI_T(x)
#define M4(x) LALT_T(x)
#define M5(x) x

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT(
  XXXXXXX,  XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,   XXXXXXX,                 XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,     XXXXXXX, XXXXXXX,
  XXXXXXX,     KC_Q,          KC_W,        KC_E,     KC_R,      KC_T,                    KC_Y,     KC_U,     KC_I,     KC_O,        KC_P, XXXXXXX,
  XXXXXXX, M1(KC_A),      M2(KC_S),    M3(KC_D), M4(KC_F),  M5(KC_G),                M5(KC_H), M4(KC_J), M3(KC_K), M2(KC_L), M1(KC_SCLN), XXXXXXX,
  XXXXXXX, M1(KC_Z),          KC_X,        KC_C,     KC_V,      KC_B,                    KC_N,     KC_M,  KC_COMM,   KC_DOT,     KC_SLSH, XXXXXXX,
           M2(KC_LBRC), M3(KC_RBRC), M4(KC_BSLS), MO(_NUM),    KC_SPC,                  KC_ENT, MO(_NAV),  KC_GRV,  KC_QUOT,     KC_BSPC
),

[_NUM] = LAYOUT(
  _______,     _______,     _______,      _______,     _______,     _______,                 _______,      _______,     _______,     _______,     _______, _______,
  _______,        KC_1,        KC_2,         KC_3,        KC_4,        KC_5,                    KC_6,         KC_7,        KC_8,        KC_9,        KC_0, _______,
  _______, M1(KC_EXLM),    M2(KC_AT), M3(KC_HASH),  M4(KC_DLR), M5(KC_PERC),             M5(KC_CIRC),  M4(KC_AMPR), M3(KC_ASTR), M2(KC_LPRN), M1(KC_RPRN), _______,
  _______,     XXXXXXX,      XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,                 XXXXXXX,      XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX, _______,
               XXXXXXX,      XXXXXXX,     XXXXXXX,     XXXXXXX,      KC_TAB,                 XXXXXXX,      MO(_FN),     KC_MINS,      KC_EQL,      KC_DEL
),

[_NAV] = LAYOUT(
  _______,     _______,     _______,     _______,     _______,     _______,                _______,     _______,      _______,     _______,     _______, _______,
  _______,     KC_PGUP,       KC_UP,     KC_PGDN,     XXXXXXX,     KC_VOLU,                XXXXXXX,     XXXXXXX,      KC_INS,      XXXXXXX,     KC_PSCR, _______,
  _______,     KC_LEFT,     KC_DOWN,     KC_RGHT,     XXXXXXX,     KC_VOLD,            M5(XXXXXXX),  M4(XXXXXXX), M3(XXXXXXX), M2(XXXXXXX), M1(XXXXXXX), _______,
  _______,     KC_HOME,      KC_ESC,      KC_END,     XXXXXXX,     KC_MUTE,                XXXXXXX,     XXXXXXX,      XXXXXXX,     XXXXXXX,     XXXXXXX, _______,
               XXXXXXX,     XXXXXXX,     XXXXXXX,     MO(_FN),      KC_TAB,                XXXXXXX,     XXXXXXX,      XXXXXXX,     XXXXXXX,     XXXXXXX
),

[_FN] = LAYOUT(
  _______,     _______,     _______,     _______,     _______,     _______,                 _______,     _______,     _______,     _______,    _______, _______,
  _______,       KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,                   KC_F6,       KC_F7,       KC_F8,       KC_F9,     KC_F10, _______,
  _______, M1(_______), M2(_______), M3(_______), M4(_______), M5(_______),             M5(_______), M4(_______), M3(_______),  M2(KC_F11), M1(KC_F11), _______,
  _______,     _______,     _______,     _______,     _______,     _______,                 _______,     _______,     RGB_HUI,     RGB_SAI,    RGB_VAI, _______,
               _______,     _______,     _______,     _______,     _______,                 _______,     _______,     RGB_SPI,     RGB_MOD,    RGB_TOG
),
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  const int r[] = {0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 1, 2, 3, 6, 7, 8};
  const int c[] = {0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < sizeof(r) / sizeof(*r); ++i) {
    rgb_matrix_set_color(g_led_config.matrix_co[r[i]][c[i]], 0, 0, 0);
  }
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint16_t sh_modtaps[] = {M1(KC_EXLM), M2(KC_AT), M3(KC_HASH), M4(KC_DLR), M5(KC_PERC), M5(KC_CIRC), M4(KC_AMPR), M3(KC_ASTR), M2(KC_LPRN), M1(KC_RPRN)};
    const uint16_t tap_values[] = {KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN};
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
