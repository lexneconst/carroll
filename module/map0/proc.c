#include "libmap.h"



void thread_map_procv(void*args){

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
    
    while(libmap_runstate() > 0){
        if(!libmap_runstate() || libmap_runstate() == 0) break;
        if(mod_data!=NULL){
            //mod_app = mod_data->object;
            //if(mod_app != NULL){
                #ifdef DEBUG_TEST
                printf("%s::%x - %x\n",__FUNCTION__, mod_data, mod_app);
                mod_print(mod_app);
                #endif
                sprintf(bfile, "/proc/%d\0", getpid());
                //printf("FILE: %s \n", bfile);
                fdev = find_init(bfile);
                while(fdev !=NULL && find_next(fdev, dfile) > 0 && libmap_runstate() && !mod_data->service){
                    if((i%16) == 15){
                         //printf("\n");
                    }                    
                    //printf("%s ", dfile);
                    i++;
                }
                find_exit(fdev);
                sprintf(bfile, "/proc/%d/mem\0", getpid());
                fd = open(bfile, O_RDWR);
                if(fd == -1){
                    fatal("open file procedure ", __FILE__, __FUNCTION__, __LINE__);
                }else{
                }
                if(fd!=-1) close(fd), fd=-1;
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

