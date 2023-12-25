#include <libcrt.h>

uint32_t libsck_state = 1;


uint32_t cread(zconfig_server_t *s,char *buffer,uint32_t length);
uint32_t cwrite(zconfig_server_t *s,char *buffer, uint32_t length);

int libsck_poweroff(void)
{
    libsck_state = 0;
}

int libsck_info(zconfig_server_t *config){
    char buffer[256];
    struct sockaddr_in  ca;
    int sock_len = sizeof(ca);
    
    if(getpeername(config->fds, (struct sockaddr*)&ca, &sock_len)!=-1){
	    //buffer = (char *)VirtualAlloc(NULL, 128, MEM_COMMIT, PAGE_READWRITE);
		//if(NULL!= buffer){
	      sprintf(buffer, "%s:%d", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
	      //return 1;
          printf("-remote : %s \n", buffer);
	      //CCNEW(remoteaddr, buffer);
	    //}	
	}
	if(getsockname(config->fds, (struct sockaddr*)&ca, &sock_len)!=-1){
	    //buffer = (char *)VirtualAlloc(NULL, 128, MEM_COMMIT, PAGE_READWRITE);
		//if(NULL!= buffer){
	      sprintf(buffer, "%s:%d", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
	      printf("-server : %s \n", buffer);
          //CCNEW(serveraddr, buffer);
	      //return 1;
	    //}	
	}
}

int libsck_server(zconfig_server_t *config)
{
    struct sockaddr_in  sa;
    struct hostent     *he=NULL;
    char host[MAX_PATH];
    int    yes = 1;
    int    ret  = 0;

    config->fix = 1;
    config->fds = socket(AF_INET,SOCK_STREAM, 0);
    if(config->fds == SOCKET_ERROR){
         fatal("invalid socket create", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons( atoi( CCSHOW(config->port) )  );
    if(strncmp(CCSHOW(config->address), "{$DEF}", 6)==0||
            strncmp(CCSHOW(config->address), "${DEF}", 6)==0){
       sa.sin_addr.s_addr = INADDR_ANY;
       }else{
       sa.sin_addr.s_addr = inet_addr(CCSHOW(config->address));
    }
    printf("Server: [%s][%s]\n", CCSHOW(config->address),CCSHOW(config->port));
    yes = 1;
    if(setsockopt(config->fds, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(int)) == SOCKET_ERROR){
       fatal("set socket option addr", __FILE__, __FUNCTION__, __LINE__);
    }
    yes = 1;
    if(setsockopt(config->fds, SOL_SOCKET, SO_REUSEPORT, (char *) &yes, sizeof(int)) == SOCKET_ERROR){
       fatal("set socket option port", __FILE__, __FUNCTION__, __LINE__);
    }
    if(bind(config->fds, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR){
   	    fatal(" @bind to socket ..", __FILE__, __FUNCTION__, __LINE__);
        sa.sin_addr.s_addr = INADDR_ANY;
   	    if(bind(config->fds, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR){
   	 	    fatal(" #bind to socket ..", __FILE__, __FUNCTION__, __LINE__);
            gethostname(host, sizeof(host));
   	 	    he = gethostbyname(host);
   	 	    if(he != NULL){
                sa.sin_addr = *(struct in_addr *) he->h_addr;   	     
   	            if(bind(config->fds, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR){
	                fatal(" $bind to socket ..", __FILE__, __FUNCTION__, __LINE__);
	                ret = 0;
   	                goto END;
		        } 
	    	}else{
			   fatal(" bind to socket .", __FILE__, __FUNCTION__, __LINE__);
			   goto END;
	    	}	
	      }
    }
    listen(config->fds, atoi(CCSHOW(config->listen)));
    ret = 1;
    END:
    return ret;
}
int libsck_client(zconfig_server_t *config)
{
    struct sockaddr_in  sa;
    struct hostent     *he=NULL;
    int                 ret =0;

    config->fix = 0;
    config->fds = socket(AF_INET,SOCK_STREAM, 0);
    if(config->fds == SOCKET_ERROR){
        fatal("invalid socket create", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons( atoi( CCSHOW(config->port) )  );
    if(inet_pton(AF_INET, CCSHOW(config->address), &sa.sin_addr) == SOCKET_ERROR){
        fatal("invalid inet pton create", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }
    if((config->fdc = connect(config->fds,(struct sockaddr*)&sa, sizeof(sa))) == SOCKET_ERROR){
        fatal("connection socket", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }
   
    ret = 1;
 
    END:
        printf("Server: [%s][%s]\n", CCSHOW(config->address),CCSHOW(config->port));

    return ret;
}
int libsck_callback(zconfig_server_t *config, socket_callback_threads_ctx ctx)
{
   struct sockaddr_in ca,addr;
   uint32_t length = SSC_STREAM_BLOCK*MGC_SEEK1;
   uint32_t len = 0;
   char buffer[length];
   char *ptr=NULL;
   //printf("buffer length: %d \n", length);

   int  sock_len;
    if(config==NULL){
        fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
        goto END;
    }
    while(libsck_state > 0)
    {
        sock_len = sizeof(ca);
   	    config->fdc = accept(config->fds, (struct sockaddr*)&ca, &sock_len);
        if(config->fdc == INVALID_SOCKET){
   	   	    fatal("socket accept ", __FILE__, __FUNCTION__, __LINE__);
   	   	    closesocket(config->fdc);
   	   	   
   	    }else{
            fd_set fds;
            struct timeval tv;
            FD_ZERO(&fds);
            FD_SET(config->fdc,&fds);
            tv.tv_sec = atoi( CCSHOW(config->timeout) ) *10;
            tv.tv_usec = 0;
    //Bugs(1134);
            select(config->fdc+1,&fds,NULL,NULL,&tv);
            if(!FD_ISSET(config->fdc,&fds)){
                fatal("server timeout", __FILE__, __FUNCTION__, __LINE__); 
                goto CLOSE;
            } 	        
            //return 0;
            while(cread(config, buffer, length) > 0){
                ptr = libmsx_decode(&ctx, length, "hello lexne", &len);
                if(ctx!=NULL) ctx(ptr, len);
                cwrite(config, "+OK\r\n", 5);
            }
            CLOSE:
            printf("disconnect %s:%d \n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
        }
        //sleep(2);
        //ctx("hello world", 256);
    }
    END:
    closesocket(config->fds);
}
int libsck_recv(zconfig_server_t *config, char *buffer, uint32_t length)
{
    return cread(config, buffer, length);
}
int libsck_send(zconfig_server_t *config, char *buffer, uint32_t length)
{
    return cwrite(config, buffer, length);
}
int libsck_shutdown(zconfig_server_t *config)
{
    shutdown(config->fdc, SD_SEND);
}
int libsck_close(zconfig_server_t *config)
{
    if(config==NULL){
        
        fatal("segment memory", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }
    closesocket(config->fds);
}
uint32_t cread(zconfig_server_t *s,char *buffer,uint32_t length)
{
    //printf("> %s \n", __FUNCTION__);
  do{
    if(s == NULL){
       fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
       break;
    }
  
    #define ZSEOL      "\r\n"
    #define ZSEOL_SIZE  2
    
    char        *ptr=NULL;
    int          eol_matched = 0;
    unsigned int count=0;
    
    ptr= buffer;
    while(recv(s->fdc, ptr, 1, 0) == 1){
       if(count>= length){
           fatal("buffer limit", __FILE__, __FUNCTION__, __LINE__);
           break;
       }
       if(*ptr== ZSEOL[eol_matched]){
          eol_matched++;
          
          if(eol_matched == ZSEOL_SIZE){
             *(ptr+1-ZSEOL_SIZE) = '\0';
             
             return strlen(buffer);
          }
          
       }else{
          eol_matched=0;
       }
       ptr++;
       count+=1;
    }
  
  }while(0);
  
    return 0;
    
}
uint32_t cwrite(zconfig_server_t *s,char *buffer, uint32_t length)
{
    //printf("> %s \n", __FUNCTION__);
    //printf("W[%s]\n",buffer);
  do{        
    if(s == NULL){
       fatal("stack memory ", __FILE__, __FUNCTION__, __LINE__);
       break;
    }         
        
    unsigned int sent_bytes=0, bytes_to_send=0;
    bytes_to_send = length;
    while(bytes_to_send){
       if(buffer==NULL) break;
       if(sent_bytes >= length) break;
       //if(s->fix){
       sent_bytes = send(s->fdc, (char *)buffer, bytes_to_send,0);
       //}else{
       //   sent_bytes = send(s->fds, (char *)buffer, bytes_to_send,0);
       //}
       if(sent_bytes == -1){
          return 0;
       }
       bytes_to_send -= sent_bytes;
       buffer+= sent_bytes;
    }
  }while(0);
  
    return 1;
  
          
}

