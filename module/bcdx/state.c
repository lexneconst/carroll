#include "libbcd.h"


uint32_t bcdx_state =0;

int libbcd_init(void)
{
    bcdx_state = 1;
    return 0;
}

int libbcd_runstate(void)
{
    return bcdx_state;
}
int libbcd_poweroff(void)
{
    bcdx_state =0;
    return 0;
}
