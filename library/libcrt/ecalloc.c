#include <libcrt.h>

uint32_t erun = 0;
typedef struct estruct_t
{
    void     *ptr;
    uint32_t  length;
    
    char      *_file_;
    char      *_func_;
    uint32_t   _line_;
    
    struct estruct_t *next;
}ecstruct_t;

struct estruct_t  *one=NULL,*two=NULL;
 
 
uint32_t einit(void *s)
{
  while(erun){ }
               
  #ifdef DBG_C
  do{
    one = (struct estruct_t *) malloc( sizeof(struct estruct_t)+ 64 );
    if(one != NULL){
           
        one->ptr = (void*)NULL;   
        one->length = 0;
         
        one->next = two;
        two= one;
    }else{
       fatal("allocate memory failed", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    
  }while(0);
  #endif
 
    erun = 0;
    
    return 0;     
}

uint32_t efini(void *s)
{  
  struct estruct_t  *next=NULL,*prev=NULL;       
  prev = two;
  
  do{
    while(prev != NULL){
       if(prev == NULL){ 
         break;
       }  
       next = prev->next;
       if(prev!=NULL){
         if(prev->ptr!=NULL){
            //printf("> ECSTR(%p) \n", prev->ptr);
         }
         CLEAN(prev->ptr);
       }
       if(prev!=NULL) free(prev), prev=NULL;
       
       prev = next;
    }
    
    //if(one!=NULL) free(one),one=NULL;
    //if(two!=NULL) free(two),two=NULL;
    
  }while(0);     
    
    erun = 0;
    
    return 0;
}

void *emalloc(uint32_t  length)
{
    void  *ptr=NULL;
    
    struct estruct_t  *next=NULL,*prev=NULL; 
    
    
  do{
    ptr = (void *) malloc( length+ 3 );
    if(ptr==NULL){
       fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    
    if(erun)
    {
      prev = two;
      
      next = (struct estruct_t *) malloc( sizeof(struct estruct_t)+ 128 );
      if(next != NULL){
           
        next->ptr = (void*)ptr;   
        next->length = length;
         
        next->next = prev;
        prev= next;
      }else{
       fatal("allocate memory failed", __FILE__, __FUNCTION__, __LINE__);
       break;
      }  
      
      two = prev;
    
    }
            
    
    //printf("> ALLOC(%p) \n", ptr);
     
  }while(0);
  return ptr;
  
}

void *efree(void *ptr)
{
  do{
     //printf("> FREEW(%p) \n", ptr);
     
     if(ptr!=NULL)
       free(ptr), ptr=NULL;
       
  }while(0);
  return ptr;
}

