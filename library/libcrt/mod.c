
#include <libcrt.h>


CRT_FUNCDECL(struct mod_stack_t *,mod_alloc,(void))
{
    struct mod_stack_t *ptr = (struct mod_stack_t *) malloc( sizeof(struct mod_stack_t) );
    if(ptr == NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    ptr->count = 0;
    ptr->addr = NULL;
    return ptr;
}
CRT_FUNCDECL(int, mod_create,(struct mod_stack_t *ptr, unsigned int cnt))
{
    int result=0;
    do{
        if(ptr==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        
        ptr->addr = (struct mod_addr_t *) malloc( sizeof(struct mod_addr_t) );
        if(ptr->addr== NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }

        //ptr->count += 1;

        result = 1;
    }while(0);
    return result;
}
CRT_FUNCDECL(int, mod_update,(struct mod_stack_t *ptr, zconfig_server_t *szpath, char *libmod, struct sysnode_proc_t *str))
{
    int result = 0;
    unsigned int count = 0, i=0;
    struct mod_addr_t  *prev=NULL,*next=NULL;
    struct mod_addr_t  *upd;
    unsigned int error_s = 0;
    char    buf[256];
    int     err=0;

    //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
    do{
        if(ptr==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        } 
        
        //mod_create(ptr, ptr->count);
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //if(ptr->addr == NULL){
        //    break;
        //} 
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd = (struct mod_addr_t *) malloc( sizeof(struct mod_addr_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;         
        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
       

        upd->seid       = ptr->count;      
        TRY:  
        if(count > 30) goto END;
        upd->libhandle  = dlopen(libmod, RTLD_LAZY);
        if(upd->libhandle){
            goto END;
        }else{
            //fatal("load module library", __FILE__, __FUNCTION__, __LINE__);
            count++;
            goto TRY;        
        }
        END:
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        count = 0;
        PREV:
        if(count >= 30) goto NEXT;
        upd->xm_getver  = dlsym(upd->libhandle, SMOD_GETVER);
        upd->xm_init    = dlsym(upd->libhandle, SMOD_INIT);
        upd->xm_exit    = dlsym(upd->libhandle, SMOD_EXIT);
        upd->xm_extw    = dlsym(upd->libhandle, SMOD_EXTW); 
        upd->xm_alloc   = dlsym(upd->libhandle, SMOD_ALLOC);
        upd->xm_ctrl    = dlsym(upd->libhandle, SMOD_CTRL);
        xmod_name xxm_name = dlsym(upd->libhandle, SMOD_NAME);
        
        if(upd->xm_getver==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;   
        }
        if(upd->xm_init==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;
        }
        if(upd->xm_exit==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;
        }
        if(upd->xm_extw==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;
        }
        if(upd->xm_alloc==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;
        }
        if(upd->xm_ctrl==NULL){
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;
        }
        if(xxm_name!=NULL){
            upd->xm_name = xxm_name();

             sprintf(buf, "/module/%s\0",xxm_name());
             upd->module_name = libcrt_new_str(buf);
            //printf("MODULE NAME(%s) \n", upd->xm_name);
        }else{
           //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
           error_s+=1;           
        }
        if(upd->xm_init!=NULL && upd->xm_alloc!=NULL){
            err = upd->xm_init(upd->xm_alloc(ptr, szpath), str);
        }else{
            //fatal("stack pointer ", __FILE__, __FUNCTION__, __LINE__);
            error_s+=1;        
        }
        count+=1;
        if(error_s) goto PREV;
        NEXT:

        if(error_s){
            printf("debug: %s = (%s)\n", __FUNCTION__, libmod); 
        }        
        upd->next = NULL;
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        next = ptr->addr;   
        if(!next){
            next = upd;
            next->next=NULL;        
        }else{
            prev = next;
            while(1){
               if(prev==NULL){
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;
               }
               if(prev->next==NULL){
                    prev->next = upd;
                    break; 
               }
               prev = prev->next; 
            }

        }

        libcrt_snupdate(str, upd->module_name , next , sizeof(struct mod_addr_t));
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //printf(" ===================================== \n");
        ptr->addr = next;
        ptr->count += 1;
        #ifdef DEBUG_TEST
        next = ptr->addr;
        for(i=0;i<ptr->count;i++){
            printf("MOD = [%s] \n", next->xm_name);
            prev = next->next;
            next = prev;
        }
        #endif
        result = 1;

        crt_count+= 1;
    }while(0);

        if(err!=0) result=err;

    return result;
}
CRT_FUNCDECL(struct mod_addr_t  *, mod_object,(struct mod_stack_t *ptr))
{
    unsigned int count = 0, i=0;
    struct mod_addr_t  *prev=NULL,*next=NULL; 
    prev = ptr->addr;
    if(ptr->count==1){
        goto ENDL;
    }
    for(i=0;i<ptr->count-1;i++){
            if(prev == NULL) break;
            next = prev->next;
            prev = next;
    }
   ENDL:
    return prev;  
}
CRT_FUNCDECL(char*, mod_getname,(struct mod_addr_t *ptr))
{
    if(ptr==NULL){
        fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    return ptr->xm_name ? ptr->xm_name : NULL;
}
CRT_FUNCDECL(struct mod_addr_t  *,mod_find,(struct mod_stack_t *ptr, char *find_mod))
{
     struct mod_addr_t  *prev=NULL, *next=NULL, *proc=NULL;
    unsigned int i=0;
    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
        #endif
        prev = ptr->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            if(prev->xm_name == NULL){
                fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                break;            
            }
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(strcmp(prev->xm_name, find_mod)==0){
                proc = prev;
                break;            
            }
            next = prev->next;
            prev = next;
        }

        //ptr->count = 0;
    }while(0);
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
    #endif
    return proc;   
}
CRT_FUNCDECL(int, mod_post,(struct mod_stack_t *ptr, char *find_mod, unsigned int ctrl, void* value))
{
    int result = 0;
    struct mod_addr_t  *proc=NULL;
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s) %d - %x \n", __FUNCTION__, ptr, find_mod, ctrl, value);
    #endif
    do{
        if(ptr==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(find_mod ==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        proc = mod_find(ptr, find_mod);
        if(proc == NULL){
            fatal("find mod handle ", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s) %d - %x \n", __FUNCTION__, ptr, find_mod, ctrl, value);
        #endif
        if(proc->xm_ctrl!=NULL && proc->xm_extw!=NULL){
            result = proc->xm_ctrl(proc->xm_extw(), ctrl, value);
        }else{
            fatal("stack pointer", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
    }while(0);
    return result;
}
CRT_FUNCDECL(int, mod_free,(struct mod_stack_t *ptr, struct sysnode_proc_t *str))
{
    struct mod_addr_t  *prev=NULL, *next=NULL;
    struct mod_struct_t *ext=NULL;
    unsigned int i=0;
    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__); 
            break;        
        }
        prev = ptr->addr;
        for(i=0;i<ptr->count;i++){
            if(prev==NULL) break;
            next = prev;

            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            //printf(" module library name (%s) \n", prev->xm_name); 
            if(prev->xm_exit!=NULL && prev->xm_extw!=NULL){
                //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
                ext= prev->xm_extw();   
                //printf("ext:: %x \n", ext); 
                //if(ext!=NULL && prev->xm_exit!=NULL)            
                prev->xm_exit(ext, str);
                
                //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            }else{
                fatal("stack pointer", __FILE__, __FUNCTION__, __LINE__);             
            }
            dlclose(prev->libhandle);
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
           
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            if(prev != NULL){
                free(prev);
                prev=NULL;            
            }else{
                fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
                break;
            }

            crt_free += 1;
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            prev =next->next;
        }

        ptr->count = 0;
    }while(0);
    return 0;
}
CRT_FUNCDECL(char*, mod_getmod_func,(struct mod_stack_t *ptr, uint32_t result_cert))
{
    struct mod_addr_t  *prev=NULL, *next=NULL, *proc=NULL;
    uint32_t result= 0;
    unsigned int i=0;
    char    *str=NULL;

    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
        #endif
        prev = ptr->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            result = result_cert;

            //printf("search function name (%s)\n", mod_getname(prev));

            mod_post(ptr, mod_getname(prev), STACK_VERIFY, (LPVOIDX) &result);
            if(result){
                str = mod_getname(prev);
                break;
            }
            next = prev->next;
            prev = next;
        }

        //ptr->count = 0;
    }while(0);
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
    #endif
    return str;       
}
CRT_FUNCDECL(struct mod_desc_t *, mod_new,(struct mod_desc_t *ptr, struct mod_desc_t *str))
{
    do{
        if(ptr==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        if(str==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;
       }

        ptr->name = str->name;    
        ptr->desc = str->desc;
        ptr->interface = str->interface ;

        ptr->service = str->service;
        ptr->hardware = str->hardware;
        ptr->bus = str->bus;
        ptr->serial = str->serial;    
    }while(0);
    return ptr;
}
CRT_FUNCDECL(struct mod_desc_t *, mod_clean,(struct mod_desc_t *ptr))
{
    return NULL;
}
CRT_FUNCDECL(int, mod_print,(struct mod_stack_t *ptr))
{
     struct mod_addr_t  *prev=NULL, *next=NULL;
    unsigned int i=0;
    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = ptr->addr;
        printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            if((i%16) == 15){
                printf("\n");
            }
            printf("%s ", prev->xm_name);
            
            next = prev->next;
            prev =next;
        }
        printf("\n");

        //ptr->count = 0;
    }while(0);
    return 0;   
}


CRT_FUNCDECL(int, mod_thread_init, (struct mod_stack_t *ptr))
{
    if(ptr!=NULL){
        ptr->thcount=0;
        ptr->thaddr = NULL;
    }
    return 0;
}
CRT_FUNCDECL(int, mod_thread_update, (struct mod_stack_t *ptr, pthread_t thread, void *addr, uint32_t pid))
{
    struct mod_thread_t *prev=NULL, *next=NULL, *upd=NULL;
    uint32_t i=0;

    if(ptr!=NULL){
        upd = (struct mod_thread_t *) malloc( sizeof(struct mod_thread_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            goto ENDL;         
        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
       
        upd->pid  = pid;
        upd->addr = addr;
        upd->tid  = thread;
        
        upd->next = NULL;
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        next = ptr->thaddr;   
        if(!next){
            next = upd;
            next->next=NULL;        
        }else{
            prev = next;
            while(1){
               if(prev==NULL){
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;
               }
               if(prev->next==NULL){
                    prev->next = upd;
                    break; 
               }
               prev = prev->next; 
            }

        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //printf(" ===================================== \n");
        ptr->thaddr = next;
        ptr->thcount += 1;

        #ifdef DEBUG_TEST
        printf("update thread %x , %x - %x, %d \n", thread, addr, pid, ptr->thcount);
        #endif

        #ifdef DEBUG_TEST
        next = ptr->thaddr;
        for(i=0;i<ptr->thcount;i++){
            
            prev = next->next;
            next = prev;
        }
        #endif
    }
    //printf("%s = %x - %x::%x %x \n", __FUNCTION__, ptr, thread, addr, pid);
   ENDL:
    return 0;
}
CRT_FUNCDECL(int, mod_thread_close, (struct mod_stack_t *ptr, uint32_t pid))
{
    struct mod_thread_t *prev=NULL, *next=NULL;
    uint32_t i=0;
    if(ptr!=NULL){
        //stack = (struct mod_stack_t*) ext_ptr->handle;
        prev = ptr->thaddr;
        for(i=0;i<ptr->thcount;i++){
            if(prev==NULL) break;
            next = prev;

            if(prev != NULL){
                if(prev->pid == pid){
                   if(prev->tid != NULL && pthread_cancel(prev->tid) == -1){
                        fatal("pthread cancel ", __FILE__, __FUNCTION__, __LINE__);
                   }
                   prev->tid = NULL;
                }
                //free(prev);
                //prev=NULL;            
            }else{
                fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
                break;
            }
            prev =next->next;
        }

        //ptr->thcount = 0;
        //if(ptr->thaddr!=NULL)
           //free(ptr->thaddr), ptr->thaddr=NULL;
    }
    return 0;
}
CRT_FUNCDECL(int, mod_thread_exit, (struct mod_stack_t *ptr))
{
    struct mod_thread_t *prev=NULL, *next=NULL;
    uint32_t i=0;

    #ifdef DEBUG_TEST
    printf("%s :: %x\n", __FUNCTION__, ptr);
    #endif

    if(ptr!=NULL){
        //stack = (struct mod_stack_t*) ext_ptr->handle;
        #ifdef DEBUG_TEST
        printf("%s :: %x :: %d \n", __FUNCTION__, ptr->thaddr, ptr->thcount); 
        #endif
        prev = ptr->thaddr;
        for(i=0;i<ptr->thcount;i++){
            if(prev==NULL){ 
                printf("end search address %x \n", prev);
                break;
            }
            next = prev;

            if(prev != NULL){
                #ifdef DEBUG_TEST
                printf("close thread id(%x) (addr:%x, pid:%x)\n", prev->pid, prev->addr, prev->tid);
                #endif

                if(prev->tid != NULL && pthread_cancel(prev->tid) ==-1){
                    fatal("pthread cancel ", __FILE__, __FUNCTION__, __LINE__);
                }
                prev->tid = NULL;
                free(prev);
                prev=NULL;            
            }else{
                fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
                break;
            }
            prev =next->next;
        }

        ptr->thcount = 0;
        //if(ptr->thaddr!=NULL)
           //free(ptr->thaddr), ptr->thaddr=NULL;
    }else{
        fatal("empty thread address", __FILE__, __FUNCTION__, __LINE__);
    }
    return 0;
}

CRT_FUNCDECL(int, mod_disable_service, (struct mod_stack_t *ptr))
{
    struct mod_addr_t  *prev=NULL, *next=NULL;
    uint32_t value = 1;
    unsigned int i=0;
    int result=0;
    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = ptr->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            if((i%16) == 15){
            //    printf("\n");
            }
            if(prev->xm_ctrl!=NULL && prev->xm_extw!=NULL){
                result = prev->xm_ctrl(prev->xm_extw(), STACK_SERVICE, &value);
            }else{
                fatal("stack pointer", __FILE__, __FUNCTION__, __LINE__);
            break;        
            }
            //printf("%s ", prev->xm_name);
            
            next = prev->next;
            prev =next;
        }
        //printf("\n");

        //ptr->count = 0;
    }while(0);
    return 0;      
}

