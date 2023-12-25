#include <libcrt.h>

char *opt = NULL;
uint32_t  opt_srv=0,opt_cfg=0;
uint32_t  opt_num=1,opt_len=0,opt_fds=0, opt_sha=0;
char      *opt_arg, *opt_key;

int usage(){
    printf("run service or debian install commanline [xbin -k] \n");
    printf("run program for not install commanline [xbin] \n");
    printf("\targs = [kernel] -k [current] -s [file] -f [code] -c  \n");
    printf("\ttest = -s -f test.conf -c 5555 \n");
}

int load_dir(uint32_t flags){
    char *ptr = (char *) emalloc( MAX_PATH );
    if(ptr==NULL){
        fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
        return NULL;    
    }
    if(flags == 1){
        strcpy(ptr, "/etc/carroll"); 
    }else{
        getcwd(ptr, MAX_PATH);

    }
    return ptr;
}

int main(int argc, char *argv[])
{
    char *ptr=NULL;
    char szdir[MAX_PATH];
    int  fd=-1;
    char *buffer=NULL;
    int  length=0,size =0;
    libc0_t     ctx;
    struct stat st;

    while(argc > 1){
        if(opt_num == argc) break;        
        
        opt= argv[opt_num];
        opt_len = strlen(opt);
        do{
            switch(*opt){
               case '-':

               break;
               case 'k':
                  opt_cfg=1;
               break; 
               case 's':
                  opt_srv=1;
               break;
               case 'f':
                  opt_fds=1;
                  opt_num++;
                  opt_arg = argv[opt_num]; 
               break;
               case 'c':
                  opt_sha=1;
                  opt_num++;
                  opt_key = argv[opt_num]; 
               break; 
            }    
            opt+=1;
            opt_len--;
        }while(opt_len>0);

        
        opt_num++;
    }
    
    if((ptr=load_dir(opt_cfg))==NULL){
        usage();
        goto END;    
    }
    
    if(!opt_fds || opt_arg ==NULL || !opt_sha || opt_key ==NULL){
        usage();
        goto END;        
    }

    strcpy(szdir, ptr);
    strcat(szdir,"/");
    strcat(szdir,opt_arg);
    
    fd = open(szdir, O_RDWR,0);
    if(fd == -1){
       fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
       goto END;
    }
   
    lseek(fd, 0, SEEK_END);
    length = get_filesize(fd);
    lseek(fd, 0, SEEK_SET);

    buffer = (char *) emalloc( length);
    if(buffer == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       goto END;
    }
    
    if((size = read(fd, buffer, length))<=0){
       fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }
    if(strstr(buffer,"[settings]")==NULL){
        fatal("encode file last version", __FILE__, __FUNCTION__, __LINE__);
        goto END; 
    }

    ptr = libmsx_encode(buffer, size , opt_key, &length);
    if(ptr == NULL || length ==0){
       fatal("handle code stack .. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;       
    }
    if(fchmod(fd, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
       fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
       goto END; 
    }
    lseek(fd, 0, SEEK_END);        
    if(fstat(fd, &st)<0){ 
       fatal("fstat file ", __FILE__, __FUNCTION__, __LINE__);
       goto END;  
    }
    lseek(fd, 0, SEEK_SET);

    //length = st.st_size;
    //if(length <= 0) goto END;         
        //buffer = (char *) malloc(length+1);
        //if(buffer ==NULL){
        //    fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
        //    break;                            
        //}

        //buffer[0]='\0';
        //if(write(fd, buffer, length)<=0 ){
        //    fatal("write file ", __FILE__, __FUNCTION__, __LINE__);            
        //    break;        
        //}
    if (ftruncate(fd, 0) != 0){
       fatal("ftruncate file ", __FILE__, __FUNCTION__, __LINE__);
       goto END;      
    }
    lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    libc0_t  *etr = (libc0_t*) ptr; 

    if(write(fd, etr, sizeof(libc0_t))<=0){
       fatal("writefile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }  
    lseek(fd, 0, SEEK_END);
    lseek(fd, sizeof(libc0_t), SEEK_SET);
    if(write(fd, etr->buffer, etr->number)<=0){
       fatal("writefile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }

    CLEAN(buffer);
    
    if(etr!=NULL){    
        CLEAN(etr->buffer);
        CLEAN(etr);
    }

    END:
    close(fd);
    return 0;
}
