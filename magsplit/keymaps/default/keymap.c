// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "module_link.h"
#include "module_oled.h"

//layer declations
enum {
    _BASE,
    _SYMBOL,
    _FN
};

// Tap Dance definitions (populate as new tap dances are added)
tap_dance_action_t tap_dance_actions[] = {
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_ortho_staggered_5x12(
        QK_GESC,       KC_1,    KC_2,    KC_3,     KC_4,     KC_5,     KC_6,    KC_7,    KC_8,        KC_9,    KC_0,          KC_DEL,
        KC_TAB,        KC_Q,    KC_W,    KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,        KC_O,    KC_P,          XXXXXXX,
        MO(_SYMBOL),   KC_A,    KC_S,    KC_D,     KC_F,     KC_G,     KC_H,    KC_J,    KC_K,        KC_L,    KC_SEMICOLON,  KC_ENTER,
        KC_LSFT,       KC_Z,    KC_X,    KC_C,     KC_V,     KC_B,     KC_N,    KC_M,    KC_COMMA,    KC_DOT,  KC_SLASH,      KC_LSFT,

                                         KC_LCTL,  KC_SPC,   LT(_FN, KC_LGUI),          KC_LSFT, KC_BSPC, MO(_FN)
    ),
    [_SYMBOL] = LAYOUT_ortho_staggered_5x12(
        KC_GRV,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MINUS,  KC_EQUAL,  XXXXXXX,    XXXXXXX,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LBRC,   KC_RBRC,   XXXXXXX,    KC_BACKSLASH,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   KC_QUOTE,  XXXXXXX,    XXXXXXX,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,   KC_LSFT,    XXXXXXX,

                                               XXXXXXX,  KC_LALT,  XXXXXXX,  KC_LSFT,  KC_BSPC,  XXXXXXX
    ),
    [_FN] = LAYOUT_ortho_staggered_5x12(
        KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,    KC_F12,
        XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_HOME,  KC_UP,    KC_END,     XXXXXXX,   XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LEFT,  KC_DOWN,  KC_RIGHT,   XXXXXXX,   XXXXXXX,
        KC_LSFT, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_VOLD,  KC_MPLY,  KC_VOLU,    XXXXXXX,   XXXXXXX,

                                               KC_LCTL,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_BSPC,  XXXXXXX
    )
};      


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SPC:
            return TAPPING_TERM + 400;
        case LSFT_T(KC_ENTER):
            return 100;    
        case LT(_FN, KC_LGUI):
            return 150;  // hold for FN layer, tap for GUI
        default:
            return TAPPING_TERM;
    }
}

void keyboard_post_init_user(void) {
    moduleLinkInit();
}

void housekeeping_task_user(void) {
    moduleLinkTask();
}

bool oled_task_user(void) {
    // the display is only fitted to the left half
    if (!is_keyboard_left()) {
        return false;
    }
    const char *layerName = "";
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            layerName = "BASE";
            break;
        case _SYMBOL:
            layerName = "SYMBOL";
            break;
        case _FN:
            layerName = "FN";
            break;
    }
    moduleOledRender(layerName);
    return false;
}