#include "libmod.h"



void thread_mod_procv(void*args){

    struct mod_struct_t *mod_data = (struct mod_struct_t *)args;     
    struct mod_stack_t  *mod_app = NULL;
    struct mod_addr_t   *prev=NULL, *next=NULL;
    uint32_t i=0;

    #ifdef DEBUG_TEST    
    printf("initialized thread (%s) \r\n", __FUNCTION__);
    #endif 
    
    while(libmod_runstate() > 0){
        if(!libmod_runstate() || libmod_runstate() == 0) break;
        if(mod_data!=NULL){
            mod_app = mod_data->object;
            if(mod_app != NULL){
                prev = mod_app->addr;
                for(i=0;i<mod_app->count;i++){
                    if(prev==NULL || mod_data->service || !libmod_runstate()) break;
                    
                    next = prev->next;
                    prev = next;
                }
                #ifdef DEBUG_TEST
                printf("%s::%x - %x\n",__FUNCTION__, mod_data, mod_app);
                mod_print(mod_app);
                #endif
            }
            //mod_data = 0x000000cd;
        }else{
        #ifdef DEBUG_TEST        
        printf("bcd:: thread procedure (%x)\n", mod_data);
        #endif
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

