#include "pd_api.h"

static int update(void* userdata);

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg;
	
	if ( event == kEventInit )
	{
		pd->display->setRefreshRate(20);
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}

static int update(void* userdata)
{
	PlaydateAPI* pd = (PlaydateAPI*)userdata;
	
	pd->graphics->clear(kColorWhite);
	pd->graphics->drawText("Hello C++!", 11, kASCIIEncoding, 100, 120);
	pd->system->drawFPS(0,0);

	return 1;
}