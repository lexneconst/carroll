#include "librmc.h"


unsigned int rmc_run_state=1;
int librmc_runstate(void)
{
    return rmc_run_state;
}
int librmc_poweroff(void)
{
    rmc_run_state = 0;
    return 0;
}

