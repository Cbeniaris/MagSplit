#pragma once

#include <stdbool.h>

typedef enum {
    MODULE_LINK_DETACHED,
    MODULE_LINK_CONNECTING,
    MODULE_LINK_CONNECTED
} ModuleLinkState;

void moduleLinkInit(void);
void moduleLinkTask(void);
ModuleLinkState moduleLinkGetState(void);
const char *moduleLinkGetName(void);