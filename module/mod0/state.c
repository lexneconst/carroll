#include "libmod.h"

uint32_t modx_state =1;

int libmod_init(void)
{
    modx_state = 1;
    return 0;
}

int libmod_runstate(void)
{
    return modx_state;
}
int libmod_poweroff(void)
{
    modx_state =0;
    return 0;
}
