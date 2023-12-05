#include "libmsx.h"

int libmsx_encode_file(const char *file,char *buf, uint32_t len, char *keys, uint32_t seek)
{
    int fd = -1;
    uint32_t length = 0;
    char     *buffer=NULL;
    libc0_t  *etr=NULL;
    char     *ptr=NULL;
    int       ret = 0;
    struct stat st;

    fd = open(file, O_RDWR,0);
    if(fd == -1){
       fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
       goto END;
    }

    ptr = libmsx_encode(buf, len , keys, &length);
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
    lseek(fd, seek+0, SEEK_SET);

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
    lseek(fd, seek+0, SEEK_SET);
    
    etr = (libc0_t*) ptr; 

    if(write(fd, etr, sizeof(libc0_t))<=0){
       fatal("writefile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }  
    lseek(fd, 0, SEEK_END);
    lseek(fd, seek+sizeof(libc0_t), SEEK_SET);
    if(write(fd, etr->buffer, etr->number)<=0){
       fatal("writefile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }

    CLEAN(buffer);

    if(etr!=NULL){    
        CLEAN(etr->buffer);
        CLEAN(etr);
    }
    ret = 1;

    END:
    close(fd);
    return ret;
}

char *libmsx_decode_file(const char *file,  uint32_t *len, char *keys, uint32_t seek)
{
    int  fd = -1;
    char  *buffer=NULL;
    uint32_t  length = *len;
    libc0_t   *ctx=NULL;

    fd = open(file, O_RDWR,0);
    if(fd == -1){
       fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
       goto END;
    }
    lseek(fd, 0, SEEK_END);
    length = get_filesize(fd);
    lseek(fd, seek + 0, SEEK_SET);

    buffer = (char *) emalloc( length );
    if(buffer == NULL){
       fatal("allocate memory failed.,", __FILE__, __FUNCTION__, __LINE__);
       goto END;
    }

    if(read(fd, buffer, length)<=0){
       fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }

    if(strstr(buffer,"MSX")==NULL){
        fatal("decode file last version", __FILE__, __FUNCTION__, __LINE__);
        goto END; 
    }
    CLEAN(buffer);
   
    lseek(fd, 0, SEEK_END);
    length = get_filesize(fd);
    lseek(fd, seek+0, SEEK_SET);
    
    ctx = (libc0_t*) emalloc( sizeof(libc0_t) );
    if(ctx == NULL){
        fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }

    if(read(fd, ctx, sizeof(libc0_t))<=0){
       fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }
   
    ctx->buffer = (char *) emalloc( ctx->number+4 );
    if(ctx->buffer == NULL){
        fatal("allocate memory ", __FILE__, __FUNCTION__, __LINE__);
        goto END;    
    }

    lseek(fd, 0, SEEK_END);
    lseek(fd, seek+sizeof(libc0_t), SEEK_SET);

    if(read(fd, ctx->buffer, ctx->number)<=0){
       fatal("readfile.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;    
    }

    buffer = libmsx_decode(ctx, ctx->maxinum, keys , &length);
    if(buffer==NULL || length == 0){
       fatal("handle code stack.. ", __FILE__, __FUNCTION__, __LINE__);
       goto END;     
    }
    if(fchmod(fd, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
       fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
       goto END; 
    }
    
    *len = ctx->maxinum;

    END:
    if(ctx!=NULL){
        CLEAN(ctx->buffer);        
        CLEAN(ctx);
    }

    close(fd);
    return buffer;
}
