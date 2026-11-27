#pragma once

#include <stdbool.h>
#include <stdint.h>

void moduleClientInit(void);
void moduleClientTask(void);
void moduleClientSendKey(uint16_t keycode, bool pressed);