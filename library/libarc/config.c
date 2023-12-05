#include <libcrt.h>

const char *_server_variables[] =
{
"module",
"mydata",
"temp",
"logs",
"server",
"client",
"port",
"listen",
"timeout"
};

char * _conf_config(uint32_t flags)
{
    char *ptr = (char *) emalloc(256);
    ptr[0] = '\0';    
    if(flags == 1){
        strcpy(ptr, "/etc/carroll");    
    }else if(flags == 0){
        //printf("initialized current directory .. \n");
        getcwd(ptr, 256);    
    }else{
        strcpy(ptr, "/etc/carroll");    
    }
   // printf("curdir: [%s] \n", ptr);
    return ptr;
}

unsigned int get_filesize(int fd)
{
   struct stat ts;
   //memset(&ts, 0, sizeof(ts));
   if(fstat(fd, &ts)==-1){ 
      fatal("open length", __FILE__,__FUNCTION__,__LINE__);
      return 0;
   }
   return (int)ts.st_size;
}
//_conf_load(lpszroot, "rhttps.conf", cserver_mod);

uint32_t  _conf_readline(const char *buff,cserver_string mod, zconfig_server_t *iserver)
{
   unsigned int ret=1;
    unsigned int length;
    unsigned int len=0,cnt=0;
    unsigned int i,j;
    
    char         *ptr=NULL,*str=NULL, *pstr=NULL;
    char         *buffer= NULL, *sbuf=NULL;
	
do{	
	length = strlen(buff);
    if(length <= 0){
       fatal("size of memory ", __FILE__, __FUNCTION__, __LINE__);
       ret=0;
       break;
    }
    
    if(buffer!=NULL){
        fatal("memory new allocate", __FILE__, __FUNCTION__, __LINE__);
    }
	buffer = (char *) emalloc( length );
    if(buffer ==NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       ret =0;
       break;
    }
    if(sbuf!=NULL){
        fatal("memory new allocate ", __FILE__, __FUNCTION__, __LINE__);
    }
    sbuf = (char *) emalloc( length+128 );
    if(sbuf == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       ret = 0;
       break;
    }
    strcpy(buffer, buff);
    
    //kusage("initialized", __FILE__, __FUNCTION__, __LINE__);
    
    length = strlen(buffer);
    for(i=0;i<length;i++ ){
        if(buffer[i] == '\n') len += 1;
    }
    
    //printf("=====[%s]=====\n", buffer);
    //kusage("initialized", __FILE__, __FUNCTION__, __LINE__);
    //printf("==========================\n"); 	  
	for(i=0;i<len;i++){
     	if((ptr = strchr(buffer,'\n')) != NULL){
               ptr = ptr + 1;
               str = _new_str(ptr);
               *ptr = 0;   
               ptr = NULL;
               if((ptr = strchr(buffer,'\n'))!= NULL && strlen(buffer)>5){
                 *ptr = 0;
                 ptr = NULL;
                 
                 //printf("CJ[%s]\n", buffer );
                 
                 if(strncmp(buffer,";",1) ==0 || 
				    strncmp(buffer,"#",1) ==0 ||
					strncmp(buffer,"@",1) ==0 ||
					strncmp(buffer,"/",1) ==0 ||
					strncmp(buffer,"[",1) ==0){
                                          
				      // SKIP		
				 }else{
				 	//if(strncmp(buffer,"PW",2)==0){
				 		//printf("LD[%s]\n", buffer );
				 		
                         if((pstr = strchr(buffer,'='))!=NULL){
				 		     //  #ifdef __BUGS__
                             //  printf("DEBUG FUNCTION[%s] LINE[%d] \n", __FUNCTION__, __LINE__);  
                             //   #endif
				 		     if(buffer[strlen(buffer)-1] == '\r')
				 		        buffer[strlen(buffer)-1] ='\0';
				 		     
                             if(buffer[0] == '$') buffer+=1;
                             //printf("buffer[%s]\n", buffer);
                             memset(sbuf, 0, length);
                             for(j=0;j<strlen(buffer);j++){
                                if(buffer[j]!= ' '){
                                   strncat(sbuf, buffer+j, 1);
                                }
                                
                             }
                             //printf("BUFFER[%s]\n", sbuf);    
                             //sbuf[cnt] ='\0';
				 		    if(mod!=NULL)
				 		          mod(sbuf, iserver);
							//printf("+OK[%s]\n", buffer );
				 		   // OK
				 			// OK
				 		}else{
				 		   //fatalW("Analyze Unknown Line ",
						   //       i+2, __FILE__ , __FUNCTION__, __LINE__);
				 		   CLEAN(str);
				 		   return i+2;	
				 		}
				 		
				 		
				 		
				 		
				 	//}else{
				 		//printf("[%s]\n", buffer);
				 	//	printf("[WARN] Analyze Unknown Line <%d> <%d, %s>\n",
					//	      i+2, __LINE__, __FUNCTION__);
				 	//	free(str);
				 	//	return i+2;
				 	//}
				 }
				 
				 
               }
              strcpy(buffer,str ); 
     
        }
    }
    //printf("==========================\n");
    //kusage("initialized", __FILE__, __FUNCTION__, __LINE__);
}while(0);

    CLEAN(buffer);
    CLEAN(sbuf);
      
    return ret; 
}


uint32_t  _conf_load(const char *root, const char *file, cserver_string mod, zconfig_server_t *iserver)
{
    char       *ptr = NULL;
    uint32_t    length =0, size=0;
    int         fd= -1;
    char        *buffer = NULL;
    uint32_t    ret =0;
    libc0_t     *ctx=NULL;
    uint32_t    no_enc=0;

do{
    length = strlen(root)+ strlen(file) ;
    
    if(ptr!=NULL){
       fatal("memory new allocate ", __FILE__, __FUNCTION__, __LINE__);
    }
    ptr = (char *) emalloc( length+ 3 );       
    if(ptr == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    strcpy(ptr, root);
    if(ptr[strlen(ptr)-1] != '/')
      strcat(ptr,"/");
    strcat( ptr, file );
    
    //printf("ptr[%s]\n", ptr);
	
    fd = open(ptr, O_RDONLY,0);
    if(fd == -1){
       fatal("opening file. ", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    lseek(fd, 0, SEEK_END);
    length = get_filesize(fd);
    lseek(fd, 0, SEEK_SET);
    
    
    if(buffer!=NULL){
       fatal("memory new allocate", __FILE__, __FUNCTION__, __LINE__);
    }
    buffer = (char *) emalloc( length);
    if(buffer == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    if((size=read(fd, buffer, length))<=0){
       fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
    buffer[size]='\0';
    if(strstr(buffer,"[settings]")!=NULL){
        no_enc=1;
    }
    
    if(!no_enc){
        lseek(fd, 0, SEEK_END);
        length = get_filesize(fd);
        lseek(fd, 0, SEEK_SET);

        ctx = (libc0_t*) emalloc( sizeof(libc0_t));
        if(ctx == NULL){
            fatal("allocate memory.. ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if((size=read(fd, ctx, sizeof(libc0_t)))<=0){
            fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        //printf("=================================================\n");
        length = ctx->number;
        ctx->buffer = (char *) emalloc( length);
        if(ctx->buffer == NULL){
            fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        lseek(fd, 0, SEEK_END);
        lseek(fd, sizeof(libc0_t), SEEK_SET);
        if((size=read(fd, ctx->buffer, length))<=0){
            fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        
        CLEAN(buffer);
        length = ctx->maxinum;
        buffer = (char *) emalloc( length+1 );
        if(buffer == NULL){        
            fatal("allocate memory.. ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        libc0_decrypt(ctx, buffer, length);

        
    }
    _conf_readline( buffer, mod, iserver);
    //printf("> [%s][%s] \n", root, file);
    ret = 1;
}while(0);
    if(ctx!=NULL){
        CLEAN(ctx->buffer);
        CLEAN(ctx);
    }
    CLEAN(buffer);
    
    if(fd!=-1) close(fd);
    
    return ret;
}
char* _anycfg_value(char *data, unsigned int seek_flags)
{
    unsigned int ret=1;
    unsigned int i=0;
    
	unsigned int length = strlen(data);
	 if(seek_flags){  // END
	 	do{
	 		if(data[strlen(data)-1] == ' ')
	 		   data[strlen(data)-1] = '\0';
	 		else 
	 		   break;
	 	}while(--length);
	 }else{            // BEGIN
	 	//printf("[IMG] [%s] \n", data);
	 	for(i=0;i<length;i++){
	 		if(*data ==' ')
	 		   *data++;
	        else
	          continue;
	 	}
	 	//printf("[EXP] [%s] \n", data);
	 }
	 
	 return data;
}

int cserver_mod(char *str, zconfig_server_t *iserver )
{
    //printf("LINE[%s]\n", str);
  do{
    if(iserver==NULL){
       fatal("heap allocate ", __FILE__, __FUNCTION__, __LINE__);
       break;              
    }
    _conf_copy(&iserver, str);
  }while(0);
}

uint32_t  _conf_copy(zconfig_server_t **info, const char *buff)
{
    unsigned int ret=0;
    unsigned int index;
    char         *ptr=NULL;
    char         *str=NULL;
    char         *pstr=NULL;
    //
    char         *buffer = NULL;
    unsigned int  length =0;
    zconfig_server_t   *server = (zconfig_server_t*) *info;
    #define zis NULL
do{
	length= strlen(buff);
	
	if(buffer!=NULL){
         fatal("memory new allocate ", __FILE__, __FUNCTION__, __LINE__);
    }
	buffer = (char *) emalloc( length );
    if(buffer == NULL){
      fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
      ret = 0;
      break;
    }
    
    strcpy(buffer, buff);
    //printf("_%s_[%s] \n",__FUNCTION__, buffer);
    
    if(strncmp(buffer, _server_variables[0] , 
	           strlen(_server_variables[0]))==0)   // modules:
	{
	   ptr = buffer + strlen(_server_variables[0])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);	
	   
	   CCNEW(server->modules , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[1] , 
	           strlen(_server_variables[1]))==0)   // mydata:
	{
	   ptr = buffer + strlen(_server_variables[1])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
       CCNEW(server->mydata ,ptr);
	   //printf("PTR[%s]\n", ptr );  
       
       ret = 1;          	
	}else if(strncmp(buffer, _server_variables[2] , 
	          strlen(_server_variables[2]))==0)   // temp:
	{
	   ptr = buffer + strlen(_server_variables[2])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->temp , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[3] , 
	          strlen(_server_variables[3]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[3])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->logs , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[4] , 
	          strlen(_server_variables[4]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[4])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->address , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[5] , 
	          strlen(_server_variables[5]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[5])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->address , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[6] , 
	          strlen(_server_variables[6]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[6])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->port , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[7] , 
	          strlen(_server_variables[7]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[7])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->listen , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}else if(strncmp(buffer, _server_variables[8] , 
	          strlen(_server_variables[8]))==0)   // logs:
	{
	   ptr = buffer + strlen(_server_variables[8])+1;
	   str =_anycfg_value(ptr, TRUE);
	   ptr =_anycfg_value(str, FALSE);		
	   
	   
	   CCNEW(server->timeout , ptr);
	   //printf("PTR[%s]\n", ptr );   
       
       ret = 1;        	
	}
    else{
      fatal("insert empty handle", __FILE__, __FUNCTION__, __LINE__);
      ret = 1;
    } 
    
       
}while(0);
    
    *info = server;
    
    CLEAN(buffer);
    
    return ret;
}


uint32_t  _conf_infow( zconfig_server_t *info)
{
    if(info==NULL) return 0;
    CPRINT("modules", CCSHOW(info->modules ));
    CPRINT("mydata", CCSHOW(info->mydata ));
    CPRINT("temp", CCSHOW(info->temp ));
    CPRINT("logs", CCSHOW(info->logs ));
    
  
    
}
uint32_t  _conf_null(zconfig_server_t **info)
{
   uint32_t  ret=0;
   zconfig_server_t  *infow = (zconfig_server_t*) *info;
do{
   
   if(infow == NULL){
      fatal("stack memory failed", __FILE__, __FUNCTION__, __LINE__);
      break;
   }          
    CCNULL( infow->modules );
    CCNULL( infow->mydata );
    CCNULL( infow->temp );
    CCNULL( infow->logs );
      
}while(0);
   
   *info = infow;
      
   return ret;
    
}

uint32_t  _conf_clean(zconfig_server_t **info)
{
   uint32_t  ret=0;
   zconfig_server_t  *infow = (zconfig_server_t*) *info;
do{
   
   if(infow == NULL){
      fatal("stack memory failed", __FILE__, __FUNCTION__, __LINE__);
      break;
   }
   
    CCLEAN( infow->modules );
    CCLEAN( infow->mydata );
    CCLEAN( infow->temp );
    CCLEAN( infow->logs );
    
    
    CLEAN(infow);
}while(0);
   
   *info = infow;
      
   return ret;
}

