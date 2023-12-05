
#include <libcrt.h>


char *_newadd_str(const char *str, const char *pad)
{
    char *buff= NULL;
	unsigned int length =  strlen(str)+strlen(pad)+32;
  
  do{
    if(buff!=NULL){
       fatal("memory new allocate", __FILE__, __FUNCTION__, __LINE__);
    }
	buff = (char *) emalloc( length );
    if(buff== NULL){
        
        fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
        
    }
    
    strcpy(buff, str);
    if(buff[strlen(buff)-1] != '/'){
       strcat(buff, pad);
    }
  }while(0);
  
    return buff;
} 

char *_new_str(const char *zstr)
{
    char *ptr= NULL;
	unsigned int length=0;
	
do{
	if(zstr==NULL){
		fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
		break;
	}

	length = strlen(zstr)+2;
	
	if(ptr!=NULL){
        fatal("memory new allocate", __FILE__,__FUNCTION__, __LINE__);
    }
	ptr = (char *) emalloc( length );
    if(ptr==NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       return NULL;
    }    
    
    strcpy(ptr, zstr);

}while(0);

    return ptr;  
}
char *_new_str1(const char *zstr, uint32_t length)
{
    char *ptr= NULL;
	
do{
	if(zstr==NULL){
		fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
		break;
	}
    if(strlen(zstr) > length){
    	fatal("stack memory failed", __FILE__, __FUNCTION__, __LINE__);
    	break;
	}
	if(ptr!=NULL){
        fatal("memory new allocate", __FILE__,__FUNCTION__, __LINE__);
    }
	ptr = (char *) emalloc( length );
    if(ptr==NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       return NULL;
    }    
    
    strcpy(ptr, zstr);
    ptr[strlen(zstr)] = '\0';
    
}while(0);

    return ptr;  
}

