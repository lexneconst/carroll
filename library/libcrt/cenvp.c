#include <libcrt.h>

uint32_t _envp_create(zconfig_server_t *ptr)
{
    uint32_t  ret=0;
    do{
        if(ptr==NULL){
            fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        ret = 1;
    }while(0);

    return ret;
}
uint32_t _envp_clean(zconfig_server_t *ptr)
{
    zconfig_envpath_t  *prev=NULL, *next=NULL;
    unsigned int i=0;
    uint32_t     ret = 0;

    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__); 
            break;        
        }
        prev = ptr->envp;
        for(i=0;i<ptr->count;i++){
            if(prev==NULL) break;
            next = prev;

            if(prev != NULL){
                free(prev);
                prev=NULL;            
            }else{
                fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
                break;
            }
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            prev =next->next;
        }

        ptr->count = 0;
        ret = 1;
    }while(0);
    return ret;   
}

char *_envp_get(zconfig_server_t *ptr, char *name)
{
    zconfig_envpath_t  *prev=NULL, *next=NULL;
    unsigned int i=0;

    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
        #endif
        prev = ptr->envp;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            if(prev->name == NULL){
                fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                break;            
            }
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(strcmp( CCSHOW(prev->name), name)==0){
                
                return CCSHOW(prev->value);
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
    return NULL;
}

uint32_t _envp_add(zconfig_server_t *ptr, char *name, char *value)
{
    uint32_t  ret = 0;
    zconfig_envpath_t *prev=NULL, *next = NULL, *upd=NULL;

    do{
        if(ptr==NULL){
            fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        upd = (zconfig_envpath_t *) malloc( sizeof(zconfig_envpath_t) );
        if(upd==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        CCNULL(upd->name);
        CCNULL(upd->value);

        CCNEW(upd->name, name);
        CCNEW(upd->value, value);

        next = ptr->envp;   
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

        //printf("VAR : %s=%s \n", name, value);

        ptr->envp = next;
        ptr->count += 1;
        
        ret = 1;
    }while(0);  

    return ret;
}

uint32_t _envp_set(zconfig_server_t *ptr, char *name, char *value)
{
    zconfig_envpath_t  *prev=NULL, *next=NULL;
    unsigned int i=0;
    uint32_t     ret = 0;

    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
        #endif
        prev = ptr->envp;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            if(prev->name == NULL){
                fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                break;            
            }
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(strcmp( CCSHOW(prev->name), name)==0){
                CCDELETE(prev->value);
                CCNEW(prev->value, value);
                ret = 1;
                break;            
            }
            next = prev->next;
            prev = next;
        }
        
        ret = _envp_add(ptr, name, value);
        //ptr->count = 0;
    }while(0);
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
    #endif
    return ret;

}

uint32_t  _envp_infow( zconfig_server_t *ptr)
{
    zconfig_envpath_t  *prev=NULL, *next=NULL;
    unsigned int i=0;
    uint32_t     ret = 0;

    do{
        if(ptr == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
        #endif
        prev = ptr->envp;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<ptr->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            if(prev->name == NULL){
                fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                break;            
            }
            CPRINT( CCSHOW(prev->name ), CCSHOW(prev->value ));

            next = prev->next;
            prev = next;
        }
        
        //ptr->count = 0;
    }while(0);
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
    #endif
    return ret;
}

