
#include "libbcd.h"

#define MOD_NAME_STR "bcdx"

struct mod_struct_t *mod_ext=NULL;

struct mod_desc_t mod_desc = {
    "bcdx",
    "brain",
    "interfaces/bcd/00",
    0x0000,
    0x0032,
    0x00,
    0x000000bc
};

int FMOD_GETVER(xmod_getver_params){
    return BCDX_CERT_VERIFY_VERSION;
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
        strcpy(szdir, ptr->szdir);
        strcat(szdir,"/cache.dat");
        mod_ext->init   = bcd_libmod_init;
        mod_ext->exit   = bcd_libmod_exit;
        mod_ext->update = bcd_libmod_update;
        mod_ext->find   = bcd_libmod_find;
        mod_ext->xread  = bcd_libmod_xread;
        mod_ext->xwrite = bcd_libmod_xwrite;
        mod_ext->xfree  = bcd_libmod_xfree;
        if(mod_ext->init!=NULL)
        mod_ext->init(szdir, ptr, str);
        
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

        ptr->szdir = libcrt_new_str( CCSHOW(szdir->szdir) );
        //ptr->name = (char *) malloc( 32 );
        //if(ptr->name==NULL){
        //    fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        //    break;        
        //}   
        ptr->name = libcrt_new_str("/bcdx/handle");
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
    struct mod_stack_t  *stack_ptr=NULL;
    struct cargv_stack_t *hdc = NULL;
    
    DEBUG_SHOW(__FUNCTION__, ptr, flags, string);
    
    do{
        if(ptr==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        hdc = (struct cargv_stack_t *) *string;

        switch(flags){
            case STACK_READ:
            break;
            case STACK_WRITE:
            break;
            case STACK_SERVICE:
                uint32_t argv0 = (uint32_t) *string;
                ptr->service = (uint32_t)argv0;
            break;
            case STACK_APPEND:
                buffer = (char *) *string;
                if(ptr->update!=NULL)
                   ptr->update(buffer, strlen(buffer));
            break;
            case STACK_INSERT:
                struct sysnode_proc_t   *argv2 = (struct sysnode_proc_t *) *string;
                ptr->system = (void*)argv2;
            break;
            case STACK_SEARCH:
                struct librmc_stack_t   *argv1 = (struct librmc_stack_t  *)*string;
                //if(pthread_join(threadid, &params)!=0){
                //    fatal("thread join ", __FILE__, __FUNCTION__, __LINE__);
                //    break;
                //}
                ptr->process = (void *)argv1;
            break;
            case STACK_UPDATE:
                if(hdc == NULL){
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;
                }
                stack_ptr = (struct mod_stack_t *) hdc->object;

                mod_post(stack_ptr, "cor0", STACK_UPDATE, (LPVOIDX)&hdc);
                //#ifdef DEBUG_TEST
                //printf("%s :: OUTPUT(%s) \n", __FUNCTION__, sptr->code);
                //#endif
                mod_post(stack_ptr, "cor0", STACK_RESULT, (LPVOIDX)&hdc);
                //#ifdef DEBUG_TEST
                //printf("%s :: INPUT(%s) \n", __FUNCTION__, sptr->code);
                //#endif

                mod_post(stack_ptr, "dbi0", STACK_SEARCH, (LPVOIDX)&hdc);
                //#ifdef DEBUG_TEST
                //printf("%s :: OUTPUT(%s) \n", __FUNCTION__, sptr->code);
                //#endif
                mod_post(stack_ptr, "dbi0", STACK_RESULT, (LPVOIDX)&hdc);
                //#ifdef DEBUG_TEST
                //printf("%s :: RESULT(%s) \n", __FUNCTION__, sptr->code);
                //#endif

                //cstring_t  *str = (cstring_t*) string;
                //if(ptr->update!=NULL)
                //   ptr->update(str->buffer, str->length);
            break; 
            case STACK_SEND:
                if(hdc == NULL){
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;
                }
                stack_ptr = (struct mod_stack_t *) hdc->object;
                mod_post(stack_ptr, "dbo0", STACK_SEARCH, (LPVOIDX)&hdc);
                mod_post(stack_ptr, "dbo0", STACK_RESULT, (LPVOIDX)&hdc);
                mod_post(stack_ptr, "dbo0", STACK_SEND, (LPVOIDX)&hdc);

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
