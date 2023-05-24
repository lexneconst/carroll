#include "libdbo.h"

char path_file[256];

struct data_struct_t  *data_ext=NULL;

int d10_callback_buffer(char *path_cstr)
{
     d10_libmod_update(path_cstr, strlen(path_cstr));   
}

int d10_libmod_init(char *path_dev,  struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    //printf("%s:: %s \n", __FUNCTION__, path_dev);
    time_t        result=0;
	struct tm *   local=NULL;
    char *digest=NULL;
    unsigned char buffer[MD5_DIGEST_LENGTH];
    unsigned int length =0;
    MD5_CTX  ctx;
    char  buf[256];
    
    result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);

	sprintf(buf, "%02d%02d%04d%02d%02d%02d",
        local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
        local->tm_hour, local->tm_min, local->tm_sec);    

    MD5Init(&ctx);
    MD5Update(&ctx, buf, strlen(buf));
    MD5Final(buffer, &ctx);
    digest = cdigest(buffer, MD5_DIGEST_LENGTH);

    strcpy(path_file, path_dev);
    //strcat(path_file, "/");
    //strcat(path_file, digest);
    //strcat(path_file, ".tmp");
    
    length = strlen(path_file);
    path_file[length] = '\0';    

    #ifdef DEBUG_TEST
    printf("%s:: %s \n", __FUNCTION__, path_file);
    #endif
    
    //libcrt_create(path_file);
    search(path_file, d10_callback_buffer);

    result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);

	sprintf(buf, "%02d%02d%04d%02d%02d%02d",
        local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
        local->tm_hour, local->tm_min, local->tm_sec);  
    

    data_ext = (struct data_struct_t*) malloc( sizeof(struct data_struct_t) );
    if(data_ext==NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        goto endl;
    }
    memset(data_ext,0, sizeof(struct data_struct_t));

    data_ext->xdev = libcrt_new_str("/base/proc/0");
    data_ext->nbase = 0;
    data_ext->xinit   = NULL;
    data_ext->xexit   = NULL;
    data_ext->xctrl   = NULL;

    data_ext->stack = NULL;
    data_ext->addr  = NULL;
    data_ext->stack = (struct data_stack_t*) malloc( sizeof(struct data_stack_t) );
    if(data_ext->stack ==NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        goto endl;
    }
    memset(data_ext->stack,0, sizeof(struct data_stack_t));
    data_ext->addr = (struct data_addr_t *) malloc( sizeof(struct data_addr_t) );
    if(data_ext->addr == NULL){
        fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        goto endl;
    }
    memset(data_ext->addr,0, sizeof(struct data_addr_t));
    return 0;
    endl:
        d10_libmod_exit(ext_ptr, str);
        return -1;
}
int d10_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    if(data_ext!=NULL){ 
        if(data_ext->stack!=NULL)
            free(data_ext->stack),data_ext->stack=NULL;
        if(data_ext->addr !=NULL)
            free(data_ext->addr), data_ext->addr=NULL;
        free(data_ext);
        data_ext=NULL;
    }
    return 0;
}
int d10_libmod_update( char *buffer, unsigned int length)
{   
    #ifdef DEBUG_TEST
    printf("%s:: %s , %d \n", __FUNCTION__, buffer, length);
    #endif

    return 0;
}
int d10_libmod_find(char *str)
{
    return 0;
}
int d10_libmod_xread(unsigned int id, cstring_t *str)
{
    return 0;
}
int d10_libmod_xwrite(unsigned int id, cstring_t *str)
{
    return 0;
}
int d10_libmod_xfree(cstring_t *str)
{
    return 0;
}
