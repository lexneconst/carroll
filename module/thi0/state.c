#include "libthi.h"

uint32_t thix_state =1;

int libthi_init(void)
{
    thix_state = 1;
    return 0;
}

int libthi_runstate(void)
{
    return thix_state;
}
int libthi_poweroff(void)
{
    thix_state =0;
    return 0;
}
