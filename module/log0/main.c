
#include "liblog.h"

#define MOD_NAME_STR "log0"

struct mod_desc_t mod_desc = {
    "log",
    "controls file handle",
    "interfaces/log/00",
    0x0000,
    0x0032,
    0x00,
    0x000000bc
};


struct mod_struct_t *mod_ext=NULL;


int FMOD_GETVER(xmod_getver_params){
    return LOG0_CERT_VERIFY_VERSION;
}

int FMOD_INIT(xmod_init_params){
    char   szdir[256];
    mod_ext = ptr;

    do{
        if(mod_ext==NULL){
            fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        if(ptr->szdir == NULL){
            fatal("segment pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        //getcwd(szdir, sizeof(szdir));
        zconfig_server_t *config = (zconfig_server_t*) ptr->config;
        strcpy(szdir, CCSHOW(config->logs) );
        
        mod_ext->init   = l00_libmod_init;
        mod_ext->exit   = l00_libmod_exit;
        mod_ext->update = l00_libmod_update;
        mod_ext->find   = l00_libmod_find;
        mod_ext->xread  = l00_libmod_xread;
        mod_ext->xwrite = l00_libmod_xwrite;
        mod_ext->xfree  = l00_libmod_xfree;
        if(mod_ext->init!=NULL)
        mod_ext->init(szdir,ptr, str);
        
        libcrt_snupdate(str, ptr->name , NULL , 0);
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
        ptr->name = libcrt_new_str("/logs/handle");
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
    char  *buffer=NULL;
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
            case STACK_APPEND:
                buffer = (char *) *string;
                if(ptr->update!=NULL)
                   ptr->update(buffer, strlen(buffer));
            break;
            case STACK_UPDATE:
                //cstring_t  *str = (cstring_t*) string;
                //if(ptr->update!=NULL)
                //   ptr->update(str->buffer, str->length);
            break; 
            case STACK_SEND:
            break;
            case STACK_DESC:
                HANDLE_DESC;
            break;       
        }
    }while(0);
    return 0;
}
char* FMOD_NAME(xmod_name_params){
    return MOD_NAME_STR;
}
