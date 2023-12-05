#include "libbcd.h"


void thread_bcdx_procv(void*args){
    #ifdef DEBUG_TEST    
    printf("initialized thread (%s) \r\n", __FUNCTION__);
    #endif
    struct mod_struct_t *ext_ptr = (struct mod_struct_t *)args;
    struct mod_stack_t  *stack = NULL;
    struct librmc_stack_t *procl = NULL;

    //librecord_wave(sound_callback_threads_ctx, psys);
    while(libbcd_runstate() > 0){
        if(!libbcd_runstate() || libbcd_runstate() == 0) break;
        if(ext_ptr!=NULL){
           stack = (struct mod_stack_t*) ext_ptr->handle;
           procl = (struct librmc_stack_t*) ext_ptr->process;
           if(stack !=NULL){
\               
           }
           if(procl !=NULL){
              
           } 
        }
        #ifdef DEBUG_TEST        
        printf("bcd:: thread procedure \n");
        #endif
        sleep(1);
    }
    pthread_exit(NULL);
}

