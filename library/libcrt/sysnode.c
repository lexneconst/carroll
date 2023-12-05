#include <libcrt.h>

struct sysnode_proc_t *crt_module = NULL;
unsigned int sysn_module = 1;

CRT_FUNCDECL(struct sysnode_proc_t *,libcrt_sysnode_init,())
{
    struct sysnode_proc_t *ptr = crt_module;

    do{
        ptr = (struct sysnode_proc_t *) malloc( sizeof(struct sysnode_proc_t));
        if(ptr == NULL){
            fatal("allcoate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        memset(ptr, 0, sizeof(struct sysnode_proc_t));

        sysn_module = 1;
    }while(0);

    return ptr;
}
CRT_FUNCDECL(struct sysnode_proc_t *,libcrt_sysnode_exit,(struct sysnode_proc_t *str))
{
    unsigned int i = 0;
    struct sysnode_addr_t *prev=NULL, *next=NULL;

    do{
        if(str==NULL){
            fatal("segment address", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = str->addr;
        if(prev==NULL) break;
        for(i=0;i<str->num;i++){
            
            next = prev->next;

            if(prev==NULL) break;
            if(prev->name!=NULL) free(prev->name), prev->name=NULL;
            free(prev);
            prev=NULL;

            prev = next;
            
            crt_free += 1;
            //if(crt_count>0) crt_count-=1;
        }
    }while(0);
    if(str!=NULL) free(str), str=NULL;
    return str;
}
CRT_FUNCDECL(int, libcrt_sysnode_proc,(struct sysnode_proc_t *str))
{
    return 0;
}
CRT_FUNCDECL(int, libcrt_sysnode_update,(struct sysnode_proc_t *str, char *sysdev, addr_t module, unsigned int size))
{
    int result = 0;
    unsigned int i=0;
    struct sysnode_addr_t  *prev=NULL,*next=NULL;
    struct sysnode_addr_t  *upd;

    //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
    do{
        if(str==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        } 
        //mod_create(ptr, ptr->count);
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //if(ptr->addr == NULL){
        //    break;
        //} 
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd = (struct sysnode_addr_t *) malloc( sizeof(struct sysnode_addr_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;         
        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd->num  = str->num; 
        upd->name = libcrt_new_str(sysdev);

        upd->sysd = module;
        upd->size = size;
        //upd->sysd = NULL;
        //upd->size = 0;

        upd->next = NULL;
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        
        next = str->addr;   
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
        str->addr = next;
        str->num += 1;
        #ifdef DEBUG_TEST
        next = str->addr;
        for(i=0;i<str->num;i++){
            printf("snupdate crt sysnode = [%s] \n", next->name);
            prev = next->next;
            next = prev;
        }
        #endif
        result = 1;

        crt_count+= 1;

    }while(0);
    return result;
}

CRT_FUNCDECL(int, libcrt_sysnode_ctrl,(char *proc, unsigned int flags, handle_t *str))
{
    return 0;
}

CRT_FUNCDECL(int, libcrt_sysnode_error,(char *msg))
{
    do{
        if(crt_module ==NULL) break;
        printf("error: %s \n", msg);
        sysn_module = 0;
    }while(0);
    return 0;
}

CRT_FUNCDECL(int, libcrt_sysnode_run,(void))
{
    #ifdef DEBUG_TEST
    printf("sysn_module: %d \n", sysn_module);
    #endif
    return sysn_module;
}

