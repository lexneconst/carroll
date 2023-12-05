
#include "libcor.h"

#define MOD_NAME_STR "cor0"

struct mod_desc_t mod_desc = {
    "cor",
    "system core",
    "interfaces/cor/00",
    0x0000,
    0x0032,
    0x00,
    0x000000bc
};


struct mod_struct_t *mod_ext=NULL;

struct core_stack_t *sptr_stack=NULL;

int FMOD_GETVER(xmod_getver_params){
    return COR0_CERT_VERIFY_VERSION;
}

int FMOD_INIT(xmod_init_params){
    char   szdir[256];
    mod_ext = ptr;

    do{
        if(mod_ext==NULL){
            fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        if(ptr->szdir==NULL){
            fatal("segment pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        zconfig_server_t *config = (zconfig_server_t*) ptr->config;
        strcpy(szdir, CCSHOW(config->temp) );

        mod_ext->init   = c00_libmod_init;
        mod_ext->exit   = c00_libmod_exit;
        mod_ext->update = c00_libmod_update;
        mod_ext->find   = c00_libmod_find;
        mod_ext->xread  = c00_libmod_xread;
        mod_ext->xwrite = c00_libmod_xwrite;
        mod_ext->xfree  = c00_libmod_xfree;
        if(mod_ext->init!=NULL)
        mod_ext->init(szdir, ptr, str);
        
        libcrt_snupdate(str, ptr->name , sptr_stack , sizeof(struct core_stack_t));

    }while(0);
    return 0;
}
int FMOD_EXIT(xmod_exit_params){
    //printf("-proc (%s) (%x)\n", __FUNCTION__, ptr);
    if(ptr!=NULL){
        if(ptr->exit!=NULL)
           ptr->exit(ptr, str);
    }
    ptr = FMOD_FREE(ptr);
    //printf("+proc (%s) (%x)\n", __FUNCTION__, ptr);
    return 0;
}
struct mod_struct_t *FMOD_EXTW(xmod_extw_params)
{
    return mod_ext;
}
struct mod_struct_t *FMOD_ALLOC(xmod_alloc_params)
{
    struct mod_struct_t *ptr = (struct mod_struct_t*) malloc( sizeof(struct mod_struct_t));
    do{    
        if(ptr==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        HANDLE_MOD_INIT(ptr);

        ptr->config = (void*)szdir;
        ptr->szdir = libcrt_new_str(  CCSHOW(szdir->szdir)  );
        //ptr->name = (char *) malloc( 32 );
        //if(ptr->name==NULL){
        //    fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        //    break;        
        //}   
        ptr->name = libcrt_new_str("/core/proc");
    }while(0);
    return ptr;
}
struct mod_struct_t *FMOD_FREE(xmod_free_params)
{
    //printf("%s = %x, %d \n", __FUNCTION__, ptr, __LINE__);
    HANDLE_MOD_EXIT(ptr);
    return ptr;
}
int FMOD_CTRL(xmod_ctrl_params)
{
    int result = 0;

    DEBUG_SHOW(__FUNCTION__, ptr, flags, string);

    do{
        if(ptr==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        switch(flags){
            case STACK_READ:
            break;
            case STACK_WRITE:
            break;
            case STACK_UPDATE:
                struct cargv_stack_t  *str = (struct cargv_stack_t*) *string;
                if(str==NULL){
                    fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);                
                }else{
                    if(ptr->update!=NULL){
                        ptr->update((char *)str, sizeof(str));
                   //c00_libmod_xset(str->code); 
                    }
                }
            break;
            case STACK_RESULT:
                #ifdef DEBUG_TEST
                printf("%s:%s \n", FMOD_NAME(), __FUNCTION__);
                #endif
                if(*string==NULL){
                     fatal("stack pointer", __FILE__, __FUNCTION__, __LINE__);
                }else{
                    *string = (void *)c00_libmod_select(*string);
                    result = 1;
                }
            break;  
            case STACK_SEND:

            break;
            case STACK_DESC:
                HANDLE_DESC;
            break;      
        }
    }while(0);
    return result;
}
char* FMOD_NAME(xmod_name_params){
    return MOD_NAME_STR;
}
