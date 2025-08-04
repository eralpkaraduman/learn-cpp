#pragma once

#include "pd_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// This must be called first in your eventHandler function
void eventHandler_pdnewlib(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg);

#ifdef __cplusplus
}
#endif