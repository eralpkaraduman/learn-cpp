// Setup.c supporting both simulator and device builds
// Based on playdate-cpp with full device support

#include "pd_api.h"
#include <sys/stat.h>

typedef int (PDEventHandler)(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg);

extern PDEventHandler eventHandler;

static void* (*pdrealloc)(void* ptr, size_t size);

#if TARGET_PLAYDATE
// Device-specific code for ARM Cortex-M7

typedef const void(*init_routine_t)(void);
extern init_routine_t __preinit_array_start, __preinit_array_end, __init_array_start, __init_array_end, __fini_array_start, __fini_array_end;
static PlaydateAPI* pd;

static void exec_array(init_routine_t* start, init_routine_t* end)
{
    while (start < end)
    {
        if (*start) (*start)();
        ++start;
    }
}

int eventHandlerShim(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    if ( event == kEventInit )
    {
        pd = playdate;
        pdrealloc = playdate->system->realloc;
        exec_array(&__preinit_array_start, &__preinit_array_end);
        exec_array(&__init_array_start, &__init_array_end);
    }

    if (event == kEventTerminate)
    {
        int result = eventHandler(playdate, event, arg);
        exec_array(&__fini_array_start, &__fini_array_end);
        return result;
    }

    return eventHandler(playdate, event, arg);
}

// Standard library functions for device
void* _malloc_r(struct _reent* _REENT, size_t nbytes) { return pdrealloc(NULL,nbytes); }
void* _realloc_r(struct _reent* _REENT, void* ptr, size_t nbytes) { return pdrealloc(ptr,nbytes); }
void _free_r(struct _reent* _REENT, void* ptr ) { pdrealloc(ptr,0); }

// System call stubs for ARM device builds
void _exit(int status) { while(1); }
int _getpid(void) { return 1; }
int _kill(int pid, int sig) { return -1; }
int _fstat(int file, struct stat *st) { return -1; }
int _isatty(int file) { return 0; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _read(int file, char *ptr, int len) { return 0; }
int _write(int file, char *ptr, int len) { return len; }
int _close(int file) { return -1; }

PDEventHandler* PD_eventHandler __attribute__((section(".capi_handler"))) = &eventHandlerShim;

extern uint32_t bssStart asm("__bss_start__");
uint32_t* _bss_start __attribute__((section(".bss_start"))) = &bssStart;

extern uint32_t bssEnd asm("__bss_end__");
uint32_t* _bss_end __attribute__((section(".bss_end"))) = &bssEnd;

// ARM exception handling symbols (since linker script discards .ARM.exidx)
uint32_t __exidx_start = 0;
uint32_t __exidx_end = 0;

#else
// Simulator-only shim for macOS

int eventHandlerShim(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInit)
        pdrealloc = playdate->system->realloc;

    return eventHandler(playdate, event, arg);
}

// Standard library functions for simulator
void* malloc(size_t nbytes) { 
    return pdrealloc(NULL, nbytes); 
}

void* realloc(void* ptr, size_t nbytes) { 
    return pdrealloc(ptr, nbytes); 
}

void free(void* ptr) {
    pdrealloc(ptr, 0);
}

#endif