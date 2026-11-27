#include QMK_KEYBOARD_H
#include <string.h>
#include "uart.h"
#include "module_client.h"
#include "module_protocol.h"

#define MODULE_PACKET_TIMEOUT_MS 100

// the keyboard rejects names longer than the protocol allows
_Static_assert(sizeof(MODULE_NAME) - 1 <= MODULE_NAME_MAX_LENGTH, "MODULE_NAME is too long for the module protocol");

typedef enum {
    RX_WAIT_START,
    RX_TYPE,
    RX_LENGTH,
    RX_PAYLOAD,
    RX_CHECKSUM
} RxState;

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

static void sendId(void) {
    sendPacket(MODULE_MSG_ID, (const uint8_t *)MODULE_NAME, (uint8_t)strlen(MODULE_NAME));
}

static void handlePacket(void) {
    switch (rxType) {
        case MODULE_MSG_WHO:
            sendId();
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

void moduleClientInit(void) {
    uart_init(MODULE_BAUD_RATE);
}

void moduleClientTask(void) {
    readUart();
}

void moduleClientSendKey(uint16_t keycode, bool pressed) {
    uint8_t payload[MODULE_KEY_PAYLOAD_LENGTH];
    payload[0] = pressed ? MODULE_KEY_PRESSED : MODULE_KEY_RELEASED;
    payload[1] = keycode & 0xFF;
    payload[2] = (keycode >> 8) & 0xFF;
    sendPacket(MODULE_MSG_KEY, payload, MODULE_KEY_PAYLOAD_LENGTH);
}