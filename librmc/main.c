#include "librmc.h"



struct librmc_stack_t *sptr_core=NULL;

int librmc_init(struct mod_stack_t *ptr, struct sysnode_proc_t *str)
{
    char  dev_name[FDEV_BITS];
    unsigned char code_name[128];
    unsigned int i=0;

    #ifdef DEBUG_TEST
    printf("%s : %d\n", __FUNCTION__, __LINE__); 
    #endif

    sptr_core = (struct librmc_stack_t*) malloc( sizeof(struct librmc_stack_t) );
    do{
        if(sptr_core ==NULL){
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        sptr_core->name = libcrt_new_str("librmc_core64");
        
        sptr_core->error_s = (char *) malloc( 64);        
        if(sptr_core->error_s ==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            sptr_core = librmc_free(sptr_core);

            break;        
        }
        sptr_core->count = 0;
        sptr_core->addr = NULL;
        sptr_core->sector=0;
        
        sptr_core->core = (struct librmc_time_t*) malloc( sizeof(struct librmc_time_t) );
        if(sptr_core->core==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            sptr_core = librmc_free(sptr_core);

            break;  
        }        

        sptr_core->state = (struct librmc_state_t*) malloc( sizeof(struct librmc_state_t));
        if(sptr_core->state ==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            sptr_core = librmc_free(sptr_core);

            break; 
        }
        sptr_core->handle = NULL;

        libcrt_snupdate(str, "/sysd/idle", (addr_t)sptr_core, sizeof(struct librmc_stack_t));

        memset(code_name, 0, sizeof(code_name));
        for(i=0;i<CORE_BITS;i++){
            sprintf(dev_name, "/sysd/rmc%d", i);
            
            librmc_update(dev_name, code_name, i, str);         
        }
        
        sptr_core->thread = NULL;
    }while(0);
            

    return 0;
}

struct librmc_stack_t * librmc_sysd(void)
{
    return sptr_core;
}
struct librmc_addr_t  *librmc_create(char *buffer, unsigned int length)
{
     struct librmc_addr_t  *prev=NULL,*next=NULL;
     unsigned int i=0;
     int  result = 0;

     do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = sptr_core->addr;
        if(prev==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_core->count <= 0){
            fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        for(i=0;i<sptr_core->count;i++){
            if(sptr_core==NULL) break;
            if(prev==NULL) break;
            if(librmc_runstate()<=0) break;
            if(prev->status == CRMC_ENABLE){
                if(prev->first!=NULL){
                    get_time_start(&prev->first->min,&prev->first->sec);
                }else{
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;                    
                }
                prev->status = CRMC_PROCESS;
                result = 1;
                break;
            }
            next = prev->next;            
            prev =next;        
        }
        if(!result || result ==0) prev=NULL;
    }while(0);
    return prev; 
}

struct librmc_addr_t  *librmc_final(char *code)
{
     struct librmc_addr_t  *prev=NULL,*next=NULL;
     unsigned int i=0;
     unsigned int result = 0;
     do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = sptr_core->addr;
        if(prev==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_core->count <= 0){
            fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        for(i=0;i<sptr_core->count;i++){
            if(librmc_runstate()<=0) break;
            if(prev==NULL) break;
            if(code!=NULL){
                if(strcmp(prev->code,code)==0 && prev->status==CRMC_FINAL){ 
                    prev->status = CRMC_ENABLE;  
                    result = 1;
                    break;  
                }
            }else{
                if(prev->status == CRMC_FINAL){   
                    prev->status = CRMC_ENABLE;  
                    result = 1;
                    break;            
                }
            }
            next = prev->next;            
            prev =next;        
        }
        if(!result || result==0){
            prev = NULL;        
        }
    }while(0);
    return prev;      
}

struct librmc_addr_t  *librmc_randstr(void)
{
     struct librmc_addr_t  *prev=NULL,*next=NULL;
     unsigned int i=0;
     unsigned int c=rand()%CORE_BITS-1;
     do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = sptr_core->addr;
        if(prev==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_core->count <= 0){
            fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        for(i=0;i<sptr_core->count;i++){
            if(librmc_runstate()<=0) break;
            if(prev->hex==c){     
                break;            
            }
            next = prev->next;            
            prev =next;        
        }
        
    }while(0);
    return prev;      
}

struct librmc_addr_t  *librmc_select(char *code, char *data)
{
     struct librmc_addr_t  *prev=NULL,*next=NULL;
     unsigned int i=0;
     int result = 0;
     do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = sptr_core->addr;
        if(prev==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_core->count <= 0){
            fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        for(i=0;i<sptr_core->count;i++){
            if(sptr_core==NULL) break;
            if(prev==NULL) break;
            if(librmc_runstate()<=0) break;
            if(strcmp(prev->code,code)==0 && prev->status == CRMC_PROCESS){
                prev->status == CRMC_FINAL;  
                prev->core = data;
                result = 1;
                break;            
            }
            next = prev->next;            
            prev =next;        
        }
        if(!result || result ==0) prev=NULL;
    }while(0);
    return prev;     
}


unsigned short librmc_find(unsigned short code, unsigned short rand_s)
{
     struct librmc_addr_t  *prev=NULL,*next=NULL;
     unsigned int i=0;
     uint32_t min=0,sec=0;
     uint32_t brian_core=0;

     do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        }
        prev = sptr_core->addr;
        if(prev==NULL){
            fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(sptr_core->count <= 0){
            fatal("memory pointer", __FILE__, __FUNCTION__, __LINE__);
            break;
        }

        for(i=0;i<sptr_core->count;i++){
            if(librmc_runstate()<=0) break;
            if(sptr_core==NULL) break;
            if(sptr_core->state == NULL) break;
            if(prev==NULL) break;
            if(prev->num == code){

                sptr_core->state->vstate[code] += 1;

                prev->hex = rand_s;
                if(prev->first!=NULL && prev->last!=NULL){
                    if(prev->status == CRMC_PROCESS || prev->status== CRMC_FINAL){                    
                        brian_core = get_time_final(prev->first->min, prev->first->sec, &prev->last->min,&prev->last->sec);
                        if(brian_core>15){
                            printf("+brian core timeout (%d) \n", brian_core);
                            prev->status = CRMC_ENABLE; 
                        }
                    }
                }else{
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;
                }
                if(sptr_core->core!=NULL){
                    min = sptr_core->core->min;
                    sec = sptr_core->core->sec;
                    brian_core = get_time_final(min, sec, &sptr_core->core->min,&sptr_core->core->sec);
                    if(brian_core>15){                
                        printf("brian core timeout (%d) \n", brian_core);
                        if(prev->status == CRMC_PROCESS){ prev->status = CRMC_ENABLE; }
                    }
                }else{
                    fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                    break;                
                }
                code = prev->hex;
                break;            
            }
            next = prev->next;            
            prev =next;        
        }
        
    }while(0);
    return code; 
}

int librmc_proc(struct mod_stack_t *ptr, librmc_callback_threads thread)
{
    int        err;
    void *stack_addr;
    uint32_t stack_size;
    pthread_attr_t attr;

    #ifdef DEBUG_TEST
    printf("%s : %d\n", __FUNCTION__, __LINE__);
    #endif
    err = pthread_create(&threadid, NULL, &thread_proc_xsd, (void*)thread);
	if (err != 0)
 	{
	    //#ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        //#endif
    }
    pthread_getattr_np(threadid, &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    mod_thread_update(ptr, threadid, stack_addr, LRMC_CERT_VERIFY_VERSION);

    return 0;
}

int librmc_exit(struct mod_stack_t *ptr, struct sysnode_proc_t *str)
{
    #ifdef DEBUG_TEST
    printf("%s : %d\n", __FUNCTION__, __LINE__);
    #endif
    #ifdef DEBUG_TEST    
    printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, 0);
    #endif
    mod_thread_close(ptr, LRMC_CERT_VERIFY_VERSION);
    sptr_core = librmc_free(sptr_core);
    #ifdef DEBUG_TEST
    printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, 1);
    #endif
    return 0;
}

int librmc_update(char *dev_name, unsigned char *data, unsigned int number, struct sysnode_proc_t *str)
{
    
    int result = 0;
    unsigned int count = 0, i=0;
    struct librmc_addr_t  *prev=NULL,*next=NULL;
    struct librmc_addr_t  *upd;
    MD5_CTX  ctx;
    char  buf[256];
    unsigned char digest[16];

    #ifdef DEBUG_TEST
    printf("%s :: dev(%s)\n", __FUNCTION__, dev_name);
    #endif
    //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
    do{
        if(sptr_core==NULL){
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            break;        
        } 
        
        //mod_create(ptr, ptr->count);
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //if(ptr->addr == NULL){
        //    break;
        //} 
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd = (struct librmc_addr_t *) malloc( sizeof(struct librmc_addr_t) );
        if(upd == NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;         
        }
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        upd->fdev = NULL;

        //upd->fdev = (char *) malloc( FDEV_BITS );
        //if(upd->fdev == NULL){
        //    fatal("allocate memory", __FILE__, __FUNCTION__ ,__LINE__);
        //    break;        
        //}
        //memset(upd->fdev, 0, FDEV_BITS);
        upd->fdev = libcrt_new_str(dev_name);
        //upd->fdev[strlen(dev_name)] = '\0';
        upd->data = NULL;

        upd->hex = rand()%CORE_BITS-1;
        upd->num = number;

        upd->brain = rand()%2;

        sprintf(buf, "%s:%d:%d", upd->fdev, upd->hex, upd->num);
        MD5Init(&ctx);
        MD5Update(&ctx, buf, strlen(buf));
        MD5Final(digest, &ctx);

        upd->code = cdigest(digest, 16);

        upd->status = CRMC_ENABLE;
        upd->core = NULL;        

        if(upd->hex > CORE_BITS-1){
            upd->hex = rand()%CORE_BITS-1;
            if(upd->hex > CORE_BITS) upd->hex = 0x0;
            //printf("%s: test line %d , (%x) \n", __FUNCTION__, __LINE__, upd->hex);
        }

        upd->first = (struct librmc_time_t*) malloc( sizeof(struct librmc_time_t) );
        if(upd->first==NULL){
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        upd->last = (struct librmc_time_t*) malloc( sizeof(struct librmc_time_t) );
        if(upd->last == NULL){
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        upd->next = NULL;

        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        next = sptr_core->addr;   
        if(!next){
            next = upd;
            next->next=NULL;        
        }else{
            prev = next;
            while(1){
               if(prev->next==NULL){
                    prev->next = upd;
                    break; 
               }
               prev = prev->next; 
            }

        }
        libcrt_snupdate(str, dev_name, next, sizeof(struct librmc_addr_t));
        //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
        //printf(" ===================================== \n");
        sptr_core->addr = next;
        sptr_core->count += 1;
        #ifdef DEBUG_TEST
        next = sptr_core->addr;
        for(i=0;i<ptr->count;i++){
            //printf("MOD = [%s] \n", next->xm_name);
            prev = next->next;
            next = prev;
        }
        #endif
        result = 1;

        rmc_count += 1;
    }while(0);
    
    if(!result) sptr_core = librmc_free(sptr_core);
        
    return result;
}

struct librmc_stack_t * librmc_free(struct librmc_stack_t *ptr)
{
    struct librmc_addr_t *prev=NULL, *next=NULL;
    unsigned int i=0;

    if(ptr!=NULL){
        #ifdef DEBUG_TEST        
        printf("debug [%s] = %d \n", __FUNCTION__, __LINE__);
        #endif
        if(ptr->name!=NULL) free(ptr->name), ptr->name=NULL;
        if(ptr->error_s!=NULL) free(ptr->error_s), ptr->error_s=NULL;
        if(ptr->state !=NULL) free(ptr->state), ptr->state = NULL;
        #ifdef DEBUG_TEST
        printf("debug [%s] = %d \n", __FUNCTION__, __LINE__);
        #endif

        if(ptr->core !=NULL) free(ptr->core), ptr->core=NULL;

        prev = ptr->addr;
        for(i=0;i<ptr->count;i++){
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            if(prev != NULL){
                next = prev->next;
                #ifdef DEBUG_TEST
                printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
                #endif
                if(prev->fdev!=NULL) free(prev->fdev), prev->fdev=NULL;
                #ifdef DEBUG_TEST
                printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
                #endif
                if(prev->data!=NULL) free(prev->data), prev->data=NULL;

                if(prev->first!= NULL) free(prev->first), prev->first=NULL;
                if(prev->last != NULL) free(prev->last),  prev->last=NULL;

                #ifdef DEBUG_TEST
                printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
                #endif
                free(prev);
                prev=NULL;
                #ifdef DEBUG_TEST
                printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
                #endif
                //if(i >= 1023) break;           
            }else{
                fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
                break;            
            }

            rmc_free +=1;
            //printf("%s: test line %d \n", __FUNCTION__, __LINE__);
            prev =next;
        }
        #ifdef DEBUG_TEST
        printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
        #endif    
        if(ptr!=NULL)
            free(ptr), ptr=NULL;
        #ifdef DEBUG_TEST
        printf("%s: test line %d - %d\n", __FUNCTION__, __LINE__, i);
        #endif 
    }
    return ptr;    
}


unsigned int rmc_count = 0;
unsigned int rmc_free = 0;
unsigned int librmc_count()
{
    return rmc_count;
}
unsigned int librmc_cfree()
{
    return rmc_free;
}




