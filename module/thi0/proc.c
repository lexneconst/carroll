#include "libthi.h"



void thread_thi_procv(void*args){

    struct mod_struct_t *mod_data = (struct mod_struct_t *)args;     
    struct mod_stack_t  *mod_app = NULL;
    struct mod_thread_t *prev=NULL, *next=NULL;
    uint64_t th_id=0;
    uint32_t i=0;
    #ifdef DEBUG_TEST    
    printf("initialized thread (%s) \r\n", __FUNCTION__);
    #endif 
    
    while(libthi_runstate() > 0){
        if(!libthi_runstate() || libthi_runstate() == 0) break;
        if(mod_data!=NULL){
            mod_app = mod_data->handle;
            if(mod_app != NULL){
                #ifdef DEBUG_TEST
                printf("%s:: count(%d) \n", __FUNCTION__, mod_app->thcount);
                #endif
                prev = mod_app->thaddr;
                for(i=0;i<mod_app->thcount;i++){
                    if(mod_data->service || !libthi_runstate()){
                        break;
                    }
                    if(prev==NULL) break;
                    //pthread_getunique_np(prev->tid, &th_id);
                    #ifdef DEBUG_TEST
                    printf("%s:: %d pid(%x) thread (%x) address(%x) (%d)\n", __FUNCTION__, i, prev->pid, prev->tid, prev->addr, th_id); 
                    #endif
                    if(prev->tid !=NULL && pthread_detach(prev->tid)!=0){
                    //    printf("Error: thread (%x) \n", prev->tid);
                        if(prev->tid!=NULL && pthread_join(prev->tid, NULL)==-1){
                            fatal("pthread join ", __FILE__, __FUNCTION__, __LINE__);
                        }
                    }

                    next = prev->next;
                    prev = next;
                }
                #ifdef DEBUG_TEST
                printf("%s::%x - %x\n",__FUNCTION__, mod_data, mod_app);
                mod_print(mod_app);
                #endif
            }else{
                #ifdef DEBUG_TEST 
                printf("mod_app:: thread procedure (%x)\n", mod_app);
                #endif
            }
            //mod_data = 0x000000cd;
        }else{
        #ifdef DEBUG_TEST        
        printf("mod_data:: thread procedure (%x)\n", mod_data);
        #endif
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

