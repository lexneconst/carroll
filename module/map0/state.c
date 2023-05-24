#include "libmap.h"

uint32_t mapx_state =1;

int libmap_init(void)
{
    mapx_state = 1;
    return 0;
}

int libmap_runstate(void)
{
    return mapx_state;
}
int libmap_poweroff(void)
{
    mapx_state =0;
    return 0;
}
