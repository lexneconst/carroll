#include "libcor.h"

char path_file[256];

int c00_libmod_init(char *path_dev,  struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
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
    strcat(path_file, "/");
    strcat(path_file, digest);
    strcat(path_file, ".tmp");
    
    length = strlen(path_file);
    path_file[length] = '\0';    

    #ifdef DEBUG_TEST
    printf("%s:: %s \n", __FUNCTION__, path_file);
    #endif

    libcrt_create(path_file);

    do{
        sptr_stack = (struct core_stack_t *) malloc( sizeof(struct core_stack_t));
        if(sptr_stack == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        sptr_stack->count=0;
        sptr_stack->cwrite=0;
        sptr_stack->cread=0;
        sptr_stack->addr = NULL;
    }while(0);
    return 0;
}
int c00_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str)
{
    libcrt_remove(path_file);
    if(sptr_stack!=NULL)
       free(sptr_stack), sptr_stack =NULL;
    return 0;
}

int c00_libmod_search(char *code)
{
    struct core_addr_t  *prev=NULL, *next=NULL, *proc=NULL;
    int result = 0;
    unsigned int i=0;
    do{
        if(sptr_stack == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, sptr_stack, find_mod);
        #endif
        prev = sptr_stack->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<sptr_stack->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(prev->status == CORE_EMPTY || strcmp(prev->code, code)==0){
                result = 1;
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
    return result;  
}

int c00_libmod_xset(char *code, char *buffer, unsigned int length)
{
    struct core_addr_t  *prev=NULL, *next=NULL, *proc=NULL;
    int result = 0;
    unsigned int i=0;
    do{
        if(sptr_stack == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, sptr_stack, find_mod);
        #endif
        prev = sptr_stack->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<sptr_stack->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(prev->status == CORE_EMPTY){
                
                prev->status = CORE_HANDLE;
                
                //if(prev->buffer!=NULL)
                //    free(prev->buffer), prev->buffer=NULL;    
                
                prev->code = code;

                prev->buffer = buffer;
                prev->length = length;

                result = 1;
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
    return result;   
}


int c00_libmod_append(char *code, char *buffer, unsigned int length)
{
    int result = 0;
    struct core_addr_t *prev=NULL, *next=NULL;
    struct core_addr_t *upd=NULL;    
    unsigned int i=0;
    
    do{
        if(sptr_stack==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        } 
        
        //mod_create(ptr, ptr->count);
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //if(ptr->addr == NULL){
        //    break;
        //} 
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd = (struct core_addr_t *) malloc( sizeof(struct core_addr_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;         
        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd->status = CORE_HANDLE;
        
        upd->code = code;

        upd->buffer=buffer;
        upd->length=length; 
        
        upd->next = NULL;
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        next = sptr_stack->addr;   
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
        sptr_stack->addr = next;
        sptr_stack->count += 1;
        #ifdef DEBUG_TEST
        next = sptr_stack->addr;
        for(i=0;i<sptr_stack->count;i++){
            //printf("MOD = [%s] \n", next->xm_name);
            prev = next->next;
            next = prev;
        }
        #endif
        result = 1;
    }while(0);

    return 0;
}

int c00_libmod_update( char *buffer, unsigned int length)
{   
    unsigned int i=0;
    struct core_addr_t *prev=NULL, *next=NULL;
    int result = 0;
    struct cargv_stack_t  *str = (struct cargv_stack_t*) buffer;

    libcrt_clear(path_file);

    #ifdef DEBUG_TEST
    printf("%s:: %x , %d \n", __FUNCTION__, buffer, length);
    #endif

    do{ 
        if(sptr_stack==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        if(c00_libmod_search(str->code)){
            c00_libmod_xset(str->code, str->buffer, str->length);
        }else{        
            c00_libmod_append(str->code, str->buffer, str->length);
        }
        result = libcrt_update(path_file, sptr_stack, sizeof(struct core_stack_t)); 
        #ifdef DEBUG_TEST        
        printf("update-> result (%d) %x , %d\n", result, sptr_stack, sizeof(struct core_stack_t));
        #endif
        prev = sptr_stack->addr;
        if(prev==NULL){
            break;        
        }
        for(i=0;i<sptr_stack->count;i++){
            if(prev==NULL) break;
            libcrt_update(path_file, prev, sizeof(struct core_addr_t));
            next = prev->next;
            prev =next;
        }
    }while(0);

    #ifdef DEBUG_TEST
    printf("%s:: %x , %d \n", __FUNCTION__, buffer, length);
    #endif

    return 0;
}

struct cargv_stack_t  *c00_libmod_select( struct cargv_stack_t  *ptr)
{
    struct core_addr_t  *prev=NULL, *next=NULL, *proc=NULL;
   
    int result = 0;
    unsigned int i=0;
    do{
        if(ptr==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_stack == NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x ,(%s)  \n", __FUNCTION__, sptr_stack, find_mod);
        #endif
        prev = sptr_stack->addr;
        //printf("PRINT LIST [%d] \n", ptr->count);
        for(i=0;i<sptr_stack->count;i++){
            if(prev == NULL) break;
            //printf("numw : %d \n", i);
            //printf("seid : %d \n", prev->seid);
            //printf("name : %s \n", prev->xm_name);
            //printf("XM_NAME (%s) \n", prev->xm_name);
            if(prev->status == CORE_HANDLE){
                prev->status = CORE_EMPTY;

                ptr->buffer = prev->buffer;
                ptr->length = prev->length;
                ptr->code   = prev->code;
                //if(prev->buffer       !=NULL)
                //    free(prev->buffer), prev->buffer=NULL;    
                //prev->buffer = buffer;
                prev->buffer = NULL;
                prev->length = 0;

                result = 1;
                break;
            }
            next = prev->next;
            prev = next;
        }
        if(!result || result ==0) prev=NULL;
        //ptr->count = 0;
    }while(0);
    #ifdef DEBUG_TEST
    printf("%s = %x ,(%s)  \n", __FUNCTION__, ptr, find_mod);
    #endif
    return (struct cargv_stack_t*)ptr;      
}

int c00_libmod_find(char *str)
{
    return 0;
}
int c00_libmod_xread(unsigned int id, cstring_t *str)
{
    return 0;
}
int c00_libmod_xwrite(unsigned int id, cstring_t *str)
{
    return 0;
}
int c00_libmod_xfree(cstring_t *str)
{
    return 0;
}
