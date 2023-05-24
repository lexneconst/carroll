#include "libstk.h"

uint32_t stkx_state =1;

int libstk_init(void)
{
    stkx_state = 1;
    return 0;
}

int libstk_runstate(void)
{
    return stkx_state;
}
int libstk_poweroff(void)
{
    stkx_state =0;
    return 0;
}
