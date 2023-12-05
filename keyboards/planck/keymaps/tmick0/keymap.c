// Copyright 2023 Travis Mick (@tmick0)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _NUM,
    _NAV
};

#define M1(x) LSFT_T(x)
#define M2(x) LCTL_T(x)
#define M3(x) LGUI_T(x)
#define M4(x) LALT_T(x)

#define NUM_SPC LT(_NUM, KC_SPC)
#define NAV_ENT LT(_NAV, KC_ENT)

#define L_ENC 3
#define R_ENC 7

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT_ortho_4x12(
       KC_Q,        KC_W,        KC_E,        KC_R,    KC_T, XXXXXXX,    XXXXXXX,    KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,
   M1(KC_A),    M2(KC_S),    M3(KC_D),    M4(KC_F),    KC_G, XXXXXXX,    XXXXXXX,    KC_H,    M4(KC_J),    M3(KC_K),    M2(KC_L), M1(KC_SCLN),
   M2(KC_Z),        KC_X,        KC_C,        KC_V,    KC_B, XXXXXXX,    XXXXXXX,    KC_N,        KC_M,     KC_COMM,      KC_DOT,     KC_SLSH,
    XXXXXXX, M3(KC_LBRC), M4(KC_RBRC),     KC_BSLS, NUM_SPC, XXXXXXX,    XXXXXXX, NAV_ENT,     KC_MINS,     KC_QUOT,     KC_BSPC,     KC_MUTE
),

[_NUM] = LAYOUT_ortho_4x12(
       KC_1,        KC_2,        KC_3,        KC_4,    KC_5, XXXXXXX,    XXXXXXX,     KC_6,        KC_7,        KC_8,        KC_9,        KC_0,
M1(KC_EXLM),   M2(KC_AT), M3(KC_HASH),  M4(KC_DLR), KC_PERC, XXXXXXX,    XXXXXXX,  KC_CIRC, M4(KC_AMPR), M3(KC_ASTR), M2(KC_LPRN), M1(KC_RPRN),
      KC_F1,       KC_F2,       KC_F3,       KC_F4,   KC_F5, XXXXXXX,    XXXXXXX,    KC_F6,       KC_F7,       KC_F8,       KC_F9,      KC_F10,
    XXXXXXX,      KC_F11,      KC_F12,     XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX,      KC_GRV,      KC_EQL,      KC_DEL,     XXXXXXX
),

[_NAV] = LAYOUT_ortho_4x12(
     KC_ESC,     KC_PGUP,       KC_UP,     KC_PGDN,  KC_VOLU, XXXXXXX,   XXXXXXX, XXXXXXX,     XXXXXXX,      KC_INS,     XXXXXXX,     KC_PSCR,
    XXXXXXX,     KC_LEFT,     KC_DOWN,     KC_RGHT,  KC_VOLD, XXXXXXX,   XXXXXXX, XXXXXXX, M4(XXXXXXX), M3(XXXXXXX), M2(XXXXXXX), M1(XXXXXXX),
    XXXXXXX,     KC_HOME,      KC_ESC,      KC_END,  KC_MUTE, XXXXXXX,   XXXXXXX, RGB_SPI,     RGB_HUI,     RGB_SAI,     RGB_VAI,     XXXXXXX,
    XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,   KC_TAB, XXXXXXX,   XXXXXXX, XXXXXXX,     RGB_TOG,     RGB_MOD,      KC_DEL,     XXXXXXX
),

};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case NUM_SPC:
    case NAV_ENT:
      return LT_TAPPING_TERM;
    default:
      return TAPPING_TERM;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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

bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (index) {
    case L_ENC:
        tap_code(clockwise ? KC_MS_WH_DOWN : KC_MS_WH_UP);
        break;
    case R_ENC:
        tap_code(clockwise ? KC_VOLD : KC_VOLU);
        break;
  }
  return false;
}
