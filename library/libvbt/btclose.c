#include <libcrt.h>

int libvbt_state = 1;

int libvbt_poweroff()
{
    libvbt_state = 0;
    return 0;
}
