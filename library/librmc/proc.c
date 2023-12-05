#include "librmc.h"


pthread_t threadid;

void thread_proc_xsd(void*args){
    librmc_callback_threads thread = (librmc_callback_threads) args;
    unsigned int code = 0x00;    
    unsigned int i=0;
    uint32_t min=0,sec=0;
    uint32_t brian_core = 0;
    #ifdef DEBUG_TEST
    printf("initialized thread (%s) (%x)\n", __FUNCTION__, thread);
    #endif

    sptr_core->thread = thread;

    //librecord_wave(sound_callback_threads_ctx);
    while(librmc_runstate()){
        sleep(1);
        if(sptr_core!=NULL){
            #ifdef DEBUG_TEST            
            printf("thread proc (%s) : %d \n", __FUNCTION__, sptr_core->count); 
            #endif
            get_time_start(&sptr_core->core->min,&sptr_core->core->sec);
            if(sptr_core->count <= 0){
                fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
                break;
            }
            if(sptr_core->state == NULL){
                fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
                break;
            }
            memset(sptr_core->state->vstate,0, sizeof(sptr_core->state->vstate));
            for(i=0;i<sptr_core->count;i++){
                if(librmc_runstate()<=0) break;
                code = librmc_find(code, rand()%CORE_BITS-1);
            }
            min = sptr_core->core->min;
            sec = sptr_core->core->sec;
            brian_core = get_time_final(min, sec, &sptr_core->core->min,&sptr_core->core->sec);
            #ifdef DEBUG_TEST            
            printf("brian core time (%d) \n", brian_core);
            #endif
        }
    }

    if(librmc_runstate()) libcrt_snerror("thread system shutdown");

    #ifdef DEBUG_TEST
    printf("exit thread \n"); 
    #endif
}

