#include QMK_KEYBOARD_H
#include <string.h>
#include "module_link.h"
#include "module_oled.h"

#define MODULE_OLED_COLUMNS 21
#define TITLE_TEXT "MagSplit"
#define TITLE_LINE 0
#define LAYER_LINE 1
#define LOGO_COLUMN 0
#define LOGO_FIRST_LINE 4
#define LOGO_PAGES 4
#define LOGO_WIDTH 32
#define NAME_COLUMN 6
#define NAME_LINE 5
#define NAME_WIDTH (MODULE_OLED_COLUMNS - NAME_COLUMN)
#define LOGO_BLINK_MS 500

// 32x32 link icon, one byte per 8 vertical pixels, four pages of 32 bytes
static const char PROGMEM logoBitmap[LOGO_PAGES * LOGO_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x3C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x3C, 0x78, 0xF8, 0xF0, 0xC0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x38, 0x38, 0x1C, 0x1C, 0x1C, 0x39, 0x39, 0x78, 0xF0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x7F, 0x3F, 0x1F, 0x00,
    0xF0, 0xF8, 0xFC, 0x1E, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x8F, 0x9E, 0x9C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x0E, 0x0F, 0x07, 0x03, 0x31, 0x70, 0xE0, 0xE0, 0xC0, 0x80,
    0x03, 0x07, 0x0F, 0x1E, 0x3C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x1E, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x0F, 0x3C, 0x38, 0x30, 0x00, 0x00, 0x01, 0x01, 0x01
};

static const char PROGMEM blankPage[LOGO_WIDTH] = {0};

static ModuleLinkState lastState = MODULE_LINK_DETACHED;

static void writeLine(uint8_t line, uint8_t column, uint8_t width, const char *text, bool centered) {
    char buffer[MODULE_OLED_COLUMNS + 1];
    uint8_t length = strlen(text);
    if (length > width) {
        length = width;
    }
    uint8_t start = centered ? (width - length) / 2 : 0;
    // pad the whole field so a shorter text overwrites the previous one
    memset(buffer, ' ', width);
    memcpy(buffer + start, text, length);
    buffer[width] = '\0';
    oled_set_cursor(column, line);
    oled_write(buffer, false);
}

static void drawLogo(bool visible) {
    for (uint8_t page = 0; page < LOGO_PAGES; page++) {
        oled_set_cursor(LOGO_COLUMN, LOGO_FIRST_LINE + page);
        oled_write_raw_P(visible ? logoBitmap + (page * LOGO_WIDTH) : blankPage, LOGO_WIDTH);
    }
}

void moduleOledRender(const char *layerName) {
    ModuleLinkState state = moduleLinkGetState();
    // wipe the screen once per state change so nothing from the previous state is left behind
    if (state != lastState) {
        oled_clear();
        lastState = state;
    }
    writeLine(TITLE_LINE, 0, MODULE_OLED_COLUMNS, TITLE_TEXT, true);
    writeLine(LAYER_LINE, 0, MODULE_OLED_COLUMNS, layerName, true);
    switch (state) {
        case MODULE_LINK_DETACHED:
            break;
        case MODULE_LINK_CONNECTING:
            // flashing logo while waiting for the module to answer
            drawLogo(((timer_read32() / LOGO_BLINK_MS) & 1) == 0);
            break;
        case MODULE_LINK_CONNECTED:
            drawLogo(true);
            writeLine(NAME_LINE, NAME_COLUMN, NAME_WIDTH, moduleLinkGetName(), false);
            break;
    }
}