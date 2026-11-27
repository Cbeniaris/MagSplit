#include QMK_KEYBOARD_H

// layer declarations
enum {
    _LAYER1,
    _LAYER2,
    _LAYER3,
    _LAYER4
};

// LAYOUT order: key 1 (NW), key 2 (NE), key 3 (SW), key 4 (SE), encoder switch
// the encoder switch always cycles layers in firmware, so its slot here is ignored
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LAYER1] = LAYOUT(
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,
            XXXXXXX
    ),
    [_LAYER2] = LAYOUT(
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,
            XXXXXXX
    ),
    [_LAYER3] = LAYOUT(
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,
            XXXXXXX
    ),
    [_LAYER4] = LAYOUT(
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,
            XXXXXXX
    )
};

// encoder turns per layer, counterclockwise then clockwise
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_LAYER1] = {ENCODER_CCW_CW(MS_WHLL, MS_WHLR)},
    [_LAYER2] = {ENCODER_CCW_CW(XXXXXXX, XXXXXXX)},
    [_LAYER3] = {ENCODER_CCW_CW(XXXXXXX, XXXXXXX)},
    [_LAYER4] = {ENCODER_CCW_CW(XXXXXXX, XXXXXXX)}
};