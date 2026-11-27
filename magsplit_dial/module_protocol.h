#pragma once

// Packet layout: START, TYPE, LENGTH, PAYLOAD (LENGTH bytes), CHECKSUM
// CHECKSUM is the XOR of TYPE, LENGTH and every payload byte
// This header is shared by the left half firmware and every module's firmware

#define MODULE_BAUD_RATE 115200
#define MODULE_PACKET_START 0xAA

#define MODULE_MSG_WHO 0x01 // keyboard to module, no payload
#define MODULE_MSG_ID 0x02  // module to keyboard, payload is the module name in ASCII
#define MODULE_MSG_KEY 0x03 // module to keyboard, payload is state, keycode low byte, keycode high byte

#define MODULE_NAME_MAX_LENGTH 15
#define MODULE_KEY_PAYLOAD_LENGTH 3
#define MODULE_PAYLOAD_MAX_LENGTH MODULE_NAME_MAX_LENGTH

#define MODULE_KEY_RELEASED 0x00
#define MODULE_KEY_PRESSED 0x01