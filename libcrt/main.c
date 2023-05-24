

#include <libcrt.h>

int crt_run_state = 0;

CRT_FUNCDECL(int, libcrt_init,(struct sysnode_proc_t *str))
{
    crt_run_state = 1;

//    /str = libcrt_sninit();
    return 0;
}
CRT_FUNCDECL(int, libcrt_run,(void))
{
    return crt_run_state;
}
CRT_FUNCDECL(int, libcrt_exit,(struct sysnode_proc_t *str))
{
    //str = libcrt_snexit(str);
    return 0;
}
CRT_FUNCDECL(int, libcrt_error,(char *err))
{
    fatal(err, __FILE__, __FUNCTION__, __LINE__);
    crt_run_state = 0;
    return 0;
}
CRT_FUNCDECL(int, libcrt_poweroff,(void)){ crt_run_state=0; return 0; }

unsigned int crt_count = 0;
unsigned int crt_free = 0;
unsigned int libcrt_count(){
    return crt_count;
}
CRT_FUNCDECL(unsigned int, libcrt_cfree,()){
    return crt_free;
}
CRT_FUNCDECL(void, fatal,(char *msg, char *file, char *func, unsigned int line))
{
    printf("fatal: %s [%s] %s@%d\r\n", msg, file, func, line);
    return ;
}

CRT_FUNCDECL(handle_t, find_init,(char *dir))
{
    DIR *dp;
    
    //struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    return (handle_t)dp;
}
CRT_FUNCDECL(int, find_next,(handle_t fd, char *name))
{
    DIR *dp = (DIR*) fd;
    struct dirent *entry;
    if((entry = readdir(dp)) != NULL){  
        
        if(strcmp(entry->d_name, ".")==0) return 1;
        if(strcmp(entry->d_name, "..")==0) return 1;  
        strcpy(name, entry->d_name);   
        name[strlen(entry->d_name)] = '\0';   
        if(strlen(entry->d_name)<=0) return 0;
        return 2;
    }
    
    return 0;
} 
CRT_FUNCDECL(void, find_exit,(handle_t fd))
{
    DIR *dp = (DIR*) fd;

    closedir(dp);
}



CRT_FUNCDECL(unsigned long,
search,(const char *path, int (*callback_thread)(char *buffer)))
{
	unsigned char *ptr=NULL,*str=NULL;
	char  *root_dir_t=NULL;
	char   buff[11];
	unsigned long length = 0, len =0;
	char buffer[1024];
    char sdir[4096];
    char old_dir[4096];
    int fd = -1;
    struct stat st;    
    int num = 0;

   root_dir_t = (char *) malloc( strlen(path)+8 );
   if(root_dir_t==NULL){
	   fatal("allocate memory ", __FUNCTION__, __FILE__ , __LINE__);
	   return 0;
   }
	   
	do{
	   sprintf(root_dir_t, "%s", path);
       //printf("FILE>[%s]\r\n", root_dir_t);
	   memset(buffer, 0, sizeof(buffer));
	   handle_t ffd = find_init(root_dir_t);
	   do{
	       if(strcmp(buffer,".")==0 || strcmp(buffer,"..")==0){
			   
	       }else{
		      //printf("> %s \r\n", fd.name );
		     
			  if(root_dir_t!=NULL) free(root_dir_t), root_dir_t=NULL;
			  root_dir_t = (char *) malloc( strlen(path)+strlen(buffer)+8);
			  if(root_dir_t ==NULL){
			      fatal("allocate memory \r\n", __FUNCTION__, __FILE__ , __LINE__);
			   	  break;
			  }
              if(strlen(buffer)<=0){
                sprintf(root_dir_t,"%s", path);
              }else if(strlen(buffer)>0 && num ==2){
	            sprintf(root_dir_t,"%s/%s", path, buffer);
              }else{
                sprintf(root_dir_t,"%s", path);
              }  
			  //printf("FILE>{%s}\r\n", root_dir_t);
			  
			  //if(fd.attrib & D_DIRECTORY){
			  //}else{
			  //  length = tsize(root_dir_t);
			  //}
			  /********************** set file and folder start ****************************/
	          
	          //printf("> =========[%x]========= \r\n", dir.dir_attrib);
	          
			  //sprintf(root_dir_t,"%s/%s/0",path, buffer);
			   /***************** create header file in folder ******************/		   
			  //printf("# file [%s] \r\n", root_dir_t);
				 
		
			  //if(str!=NULL) free(str), str=NULL;
	          //str = (unsigned char *) malloc( length+6 );
	          //if(str!=NULL){
	             //printf("1) =========================\r\n");
	            
				 //if(fd.attrib & D_DIRECTORY){
	                
					//printf("> RE [%s] \r\n", root_dir_t);
					//
					 
			     //}else{
			   strcpy(sdir, root_dir_t);		 
					/************************ write file in data ****************************/
               //if(strlen(buffer)>0){
	           //     sprintf(sdir,"%s/%s", root_dir_t);
               //}else{
               //     sprintf(sdir,"%s", root_dir_t);
               //}
               stat(sdir, &st);
               
			   //printf("::[%s] mode (%x)(%x) \r\n", sdir, ((st.st_mode >> 8) & 0xff)-1 , ((S_IFDIR >> 8) & 0xff)); 
               //printf(">DIR(%s)OLD(%s) \n", old_dir, root_dir_t);
               if(((st.st_mode >> 8) & 0xff)-1 == ((S_IFDIR >> 8) & 0xff) && strcmp(old_dir, root_dir_t)!=0 && strcmp(path, root_dir_t)!=0){
                    strcpy(old_dir, root_dir_t);
                    old_dir[strlen(root_dir_t)] = '\0';

                    search(root_dir_t, callback_thread);   
               }else{
                    fd = open(sdir, O_RDWR);
                    if(fd == -1){
                        //fatal("open file ", __FILE__, __FUNCTION__, __LINE__);                    
                    }else{
                         
                        fstat(fd, &st);
                        //printf("file(%s)\n", sdir);
                        //if(st.st_mode == S_IFDIR){
                        //    search(root_dir_t, callback_thread);
                        //}else{
                            if(callback_thread!=NULL)
                                callback_thread(sdir);
                        //}
                        close(fd);
                        fd = -1;
                    } 
					//printf("3) =========================\r\n");
				}	
				//if(str!=NULL) free(str), str=NULL;
					
					//printf("----------------------------------------------------\r\n");
					 //root.current_dir = re_root(root_dir_t);
					 
					 //goto root_next;
				 //}
			     //printf("4) =========================\r\n");
		      //}else{
		      //	fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
								
		      //}
			  //if(str!=NULL) free(str), str=NULL;
	          //printf("5) =========================\r\n");
	         
			  
			  if(root_dir_t!=NULL) free(root_dir_t), root_dir_t=NULL;
			  
		  }
	   }while((num = find_next(ffd,buffer))>0);
	   find_exit(ffd);
	   
	   //if(count > 0){ count-=1;  goto root_next;  }
	   
	   if(root_dir_t!=NULL) free(root_dir_t), root_dir_t=NULL;
    }while(0);
	
	return 0;
}




#ifdef TEST_DEV
void printdir(char *dir)
{
    DIR *dp;
    struct dirent *entry;
    //struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    //chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        //lstat(entry->d_name,&statbuf);
        /*if(S_ISDIR(statbuf.st_mode)) {
            
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            
            printdir(entry->d_name,depth+4);
        }
        else 
        */
        printf("%s\n",entry->d_name);
    }
    //chdir("..");
    closedir(dp);
}
#endif

CRT_FUNCDECL(float,  percent,(unsigned int total_marks, unsigned int scored))
{
	float  per=0;
	do{
		if(total_marks==0){
		   fatal("number line ", __FILE__, __FUNCTION__, __LINE__);
		   break;	
		} 
		//if(scored==0){
		//   fatal("number line", __FILE__, __FUNCTION__, __LINE__);
		//    break;
		//}
		
		per = (float)scored / total_marks * 100.0;
		total_marks=0;
        scored=0;
	}while(0);
	return per;
}

CRT_FUNCDECL(int, xgetdate,(char *buffer))
{
    time_t        result=0;
	struct tm *   local=NULL;
	result = time(NULL);
	local = (struct tm *)localtime((time_t*)&result);

	sprintf(buffer, "%d-%d-%d %2d,%2d,%2d",
        local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
        local->tm_hour, local->tm_min, local->tm_sec);
    return 0;
}

CRT_FUNCDECL(char *,cdigest,(unsigned char *p,unsigned int len))
	{
	static char bufs[10][20];
	static int bnum=0;
	char *ret;
	int i;
	static char *f="0123456789ABCDEF";

	ret= &(bufs[bnum++][0]);
	bnum%=10;
	for (i=0; i<len/2; i++)
		{
		ret[i*2]=f[(p[i]>>4)&0xf];
		ret[i*2+1]=f[p[i]&0xf];
		}
	ret[len]='\0';
	return(ret);
}

CRT_FUNCDECL(char, *libcrt_new_str,(char *str))
{
    unsigned int length = 0;
    char         *ptr = NULL;
    do{
        if(str==NULL) break;
        length = strlen(str);
        ptr = (char *) malloc( length +1);
        if(ptr==NULL){
            fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        strcpy(ptr, str);
        #ifdef DEBUG_TEST
        printf("proc :: %s \n", ptr); 
        #endif
    }while(0);
    return ptr;
}

