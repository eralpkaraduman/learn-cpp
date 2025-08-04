// C++ runtime support functions
// Simplified version of playdate-cpp's pdnewlib functionality

#include "pd_api.h"

static PlaydateAPI* pd = NULL;

void eventHandler_pdnewlib(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    (void)arg; // unused parameter
    
    if (event == kEventInit)
    {
        pd = playdate;
    }
}