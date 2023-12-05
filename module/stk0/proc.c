#include "libstk.h"



void thread_stk_procv(void*args){

    struct mod_struct_t *mod_data = (struct mod_struct_t *)args;     
    struct mod_stack_t  *mod_app = NULL;
    char bfile[MAX_PATH]; 
    char dfile[MAX_PATH];   
    handle_t  *fdev=NULL;
    uint32_t  i=0;
    int       fd=-1;
    #ifdef DEBUG_TEST    
    printf("initialized thread (%s) \r\n", __FUNCTION__);
    #endif 
    
    while(libstk_runstate() > 0){
        if(!libstk_runstate() || libstk_runstate() == 0) break;
        if(mod_data!=NULL){
            //mod_app = mod_data->object;
            //if(mod_app != NULL){
                #ifdef DEBUG_TEST
                printf("%s::%x - %x\n",__FUNCTION__, mod_data, mod_app);
                mod_print(mod_app);
                #endif
                //printf("===============================================\n");
            //}
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

