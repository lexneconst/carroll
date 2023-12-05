#include "libdbm.h"

int stack_core_state = 1;

int libdbm_runstate(void)
{
    return stack_core_state;
}
int libdbm_poweroff(void)
{
    stack_core_state =0;
    return 0;
}


int libdbm_create(struct mod_args_t *args)
{
    #ifdef DEBUG_TEST
    printf("%s - %x\n", __FUNCTION__, args);
    #endif 
    struct mod_struct_t  *args_ptr = args->argv0;
    struct cargv_stack_t *core_ptr = args->argv1;
    #ifdef DEBUG_TEST
    printf("search: %s:%s\n", FMOD_NAME(), __FUNCTION__);
    #endif
    return 0;
}

