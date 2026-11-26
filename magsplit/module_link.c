#include QMK_KEYBOARD_H
#include "split_util.h"
#include "uart.h"
#include "module_link.h"
#include "module_protocol.h"

#define MODULE_DETECT_PIN B2
#define MODULE_DETECT_DEBOUNCE_MS 50
#define MODULE_WHO_INTERVAL_MS 1000
#define MODULE_PACKET_TIMEOUT_MS 100

typedef enum {
    RX_WAIT_START,
    RX_TYPE,
    RX_LENGTH,
    RX_PAYLOAD,
    RX_CHECKSUM
} RxState;

static bool linkActive = false;
static ModuleLinkState linkState = MODULE_LINK_DETACHED;
static char moduleName[MODULE_NAME_MAX_LENGTH + 1] = "";
static uint32_t lastWhoTime = 0;

// detect pin debounce
static bool detectCandidate = false;
static bool detectStable = false;
static uint32_t detectChangeTime = 0;

// packet parser
static RxState rxState = RX_WAIT_START;
static uint8_t rxType = 0;
static uint8_t rxLength = 0;
static uint8_t rxIndex = 0;
static uint8_t rxChecksum = 0;
static uint8_t rxPayload[MODULE_PAYLOAD_MAX_LENGTH];
static uint32_t lastByteTime = 0;

static void sendPacket(uint8_t type, const uint8_t *payload, uint8_t length) {
    uint8_t checksum = type ^ length;
    uart_write(MODULE_PACKET_START);
    uart_write(type);
    uart_write(length);
    for (uint8_t i = 0; i < length; i++) {
        uart_write(payload[i]);
        checksum ^= payload[i];
    }
    uart_write(checksum);
}

static void sendWho(void) {
    sendPacket(MODULE_MSG_WHO, NULL, 0);
    lastWhoTime = timer_read32();
}

static void resetParser(void) {
    rxState = RX_WAIT_START;
    rxChecksum = 0;
    rxIndex = 0;
}

static void flushReceiveBuffer(void) {
    while (uart_available()) {
        uart_read();
    }
    resetParser();
}

static void handlePacket(void) {
    switch (rxType) {
        case MODULE_MSG_ID:
            if (linkState != MODULE_LINK_DETACHED && rxLength >= 1) {
                memcpy(moduleName, rxPayload, rxLength);
                moduleName[rxLength] = '\0';
                linkState = MODULE_LINK_CONNECTED;
            }
            break;
        case MODULE_MSG_KEY:
            if (linkState == MODULE_LINK_CONNECTED && rxLength == MODULE_KEY_PAYLOAD_LENGTH) {
                uint16_t keycode = rxPayload[1] | (rxPayload[2] << 8);
                if (rxPayload[0] == MODULE_KEY_PRESSED) {
                    register_code16(keycode);
                } else {
                    unregister_code16(keycode);
                }
            }
            break;
        default:
            break;
    }
}

static void feedByte(uint8_t data) {
    switch (rxState) {
        case RX_WAIT_START:
            if (data == MODULE_PACKET_START) {
                rxState = RX_TYPE;
            }
            break;
        case RX_TYPE:
            rxType = data;
            rxChecksum = data;
            rxState = RX_LENGTH;
            break;
        case RX_LENGTH:
            if (data > MODULE_PAYLOAD_MAX_LENGTH) {
                rxState = RX_WAIT_START;
                break;
            }
            rxLength = data;
            rxChecksum ^= data;
            rxIndex = 0;
            rxState = (rxLength == 0) ? RX_CHECKSUM : RX_PAYLOAD;
            break;
        case RX_PAYLOAD:
            rxPayload[rxIndex++] = data;
            rxChecksum ^= data;
            if (rxIndex >= rxLength) {
                rxState = RX_CHECKSUM;
            }
            break;
        case RX_CHECKSUM:
            if (data == rxChecksum) {
                handlePacket();
            }
            rxState = RX_WAIT_START;
            break;
    }
}

static void readUart(void) {
    while (uart_available()) {
        uint8_t data = uart_read();
        // a packet that stalls mid-way is dropped so the parser can resync
        if (rxState != RX_WAIT_START && timer_elapsed32(lastByteTime) > MODULE_PACKET_TIMEOUT_MS) {
            rxState = RX_WAIT_START;
        }
        lastByteTime = timer_read32();
        feedByte(data);
    }
}

static void handleAttach(void) {
    flushReceiveBuffer();
    moduleName[0] = '\0';
    linkState = MODULE_LINK_CONNECTING;
    sendWho();
}

static void handleDetach(void) {
    linkState = MODULE_LINK_DETACHED;
    moduleName[0] = '\0';
    resetParser();
    // release anything the module was holding
    clear_keyboard();
}

static void updateDetect(void) {
    // the module pulls B2 low when attached
    bool attachedNow = !gpio_read_pin(MODULE_DETECT_PIN);
    if (attachedNow != detectCandidate) {
        detectCandidate = attachedNow;
        detectChangeTime = timer_read32();
    }
    if (detectCandidate != detectStable && timer_elapsed32(detectChangeTime) >= MODULE_DETECT_DEBOUNCE_MS) {
        detectStable = detectCandidate;
        if (detectStable) {
            handleAttach();
        } else {
            handleDetach();
        }
    }
}

void moduleLinkInit(void) {
    // the module connector only exists on the left half, and only the main half can send keys to the host
    linkActive = is_keyboard_master() && is_keyboard_left();
    if (!linkActive) {
        return;
    }
    gpio_set_pin_input_high(MODULE_DETECT_PIN);
    uart_init(MODULE_BAUD_RATE);
}

void moduleLinkTask(void) {
    if (!linkActive) {
        return;
    }
    updateDetect();
    if (linkState == MODULE_LINK_DETACHED) {
        return;
    }
    readUart();
    if (linkState == MODULE_LINK_CONNECTING && timer_elapsed32(lastWhoTime) >= MODULE_WHO_INTERVAL_MS) {
        sendWho();
    }
}

ModuleLinkState moduleLinkGetState(void) {
    return linkState;
}

const char *moduleLinkGetName(void) {
    return moduleName;
}