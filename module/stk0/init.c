#include "libstk.h"

char path_file[256];


pthread_t threadid;


int s10_libmod_init(char *path_dev, struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    //printf("%s:: %s \n", __FUNCTION__, path_dev);
    time_t        result=0;
	struct tm *   local=NULL;
    char *digest=NULL;
    unsigned char buffer[MD5_DIGEST_LENGTH];
    unsigned int length =0;
    MD5_CTX  ctx;
    char  buf[256];
    int        ret = -1;
    int        err;
    struct mod_stack_t *stack = NULL;
    void *stack_addr;
    uint32_t stack_size;
    pthread_attr_t attr;

    if(ext_ptr!=NULL){
       stack = (struct mod_stack_t *) ext_ptr->handle;

    }else{
        fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
        goto ENDZ;
    }

    result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);

	sprintf(buf, "%02d%02d%04d%02d%02d%02d",
        local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
        local->tm_hour, local->tm_min, local->tm_sec);    

    MD5Init(&ctx);
    MD5Update(&ctx, buf, strlen(buf));
    MD5Final(buffer, &ctx);
    digest = cdigest(buffer, MD5_DIGEST_LENGTH);

    //strcpy(path_file, path_dev);
    //strcat(path_file, "/");
    //strcat(path_file, digest);
    //strcat(path_file, ".tmp");
    
    length = strlen(path_file);
    path_file[length] = '\0';    
    
    libstk_init();
    #ifdef DEBUG_TEST
    printf("%s:: %s \n", __FUNCTION__, path_dev);
    #endif
    err = pthread_create(&threadid, NULL, &thread_stk_procv, (void*) ext_ptr);
	if (err != 0)
 	{
	    //#ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        //#endif
    }
    pthread_getattr_np(threadid, &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    mod_thread_update(stack, threadid, stack_addr, STK0_CERT_VERIFY_VERSION);

    ret = 0;

    ENDZ:
    return ret; 
}
int s10_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    uint32_t i=0;
    struct mod_stack_t *stack = NULL;
    if(ext_ptr!=NULL){
        stack = (struct mod_stack_t*) ext_ptr->handle;
        //for(i=0;i<stack->thcount;i++){
        mod_thread_close(stack, STK0_CERT_VERIFY_VERSION);
            //pthread_cancel();
        //}
    }
    libstk_poweroff();
    return 0;
}

int s10_libmod_search(char *code)
{
    return 0; 
}

int s10_libmod_xset(char *code, char *buffer, unsigned int length)
{
    return 0; 
}


int s10_libmod_append(char *code, char *buffer, unsigned int length)
{

    return 0;
}

int s10_libmod_update( char *buffer, unsigned int length)
{   


    return 0;
}

struct cargv_stack_t  *s10_libmod_select( struct cargv_stack_t  *ptr)
{
    return NULL;  
}

int s10_libmod_find(char *str)
{
    return 0;
}
int s10_libmod_xread(unsigned int id, cstring_t *str)
{
    return 0;
}
int s10_libmod_xwrite(unsigned int id, cstring_t *str)
{
    return 0;
}
int s10_libmod_xfree(cstring_t *str)
{
    return 0;
}
