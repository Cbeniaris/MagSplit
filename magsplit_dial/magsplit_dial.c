#include QMK_KEYBOARD_H
#include "module_client.h"

#define LAYER_COUNT 4

// the encoder switch is the fifth direct input, it always cycles layers and is never forwarded
#define LAYER_SWITCH_ROW 0
#define LAYER_SWITCH_COL 4

// one LED per layer, driven high to light
static const pin_t layerLeds[LAYER_COUNT] = {F6, F7, E6, D7};

static void updateLayerLeds(uint8_t activeLayer) {
    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        gpio_write_pin(layerLeds[i], i == activeLayer);
    }
}

void keyboard_pre_init_kb(void) {
    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        gpio_set_pin_output(layerLeds[i]);
        gpio_write_pin_low(layerLeds[i]);
    }
    keyboard_pre_init_user();
}

void keyboard_post_init_kb(void) {
    moduleClientInit();
    updateLayerLeds(get_highest_layer(layer_state));
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    moduleClientTask();
    housekeeping_task_user();
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
    updateLayerLeds(get_highest_layer(state));
    return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }
    // encoder switch, next layer on press and wrap from the last layer back to the first
    if (record->event.type == KEY_EVENT && record->event.key.row == LAYER_SWITCH_ROW && record->event.key.col == LAYER_SWITCH_COL) {
        if (record->event.pressed) {
            layer_move((get_highest_layer(layer_state) + 1) % LAYER_COUNT);
        }
        return false;
    }
    // every other key and encoder turn is sent to the keyboard instead of the host
    moduleClientSendKey(keycode, record->event.pressed);
    return false;
}