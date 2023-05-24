
#include <libcrt.h>

CRT_FUNCDECL(int, libcrt_create,(char *file))
{
    int fd = -1;
    do{
        fd = open(file, O_CREAT | O_RDWR);
        if(fd==-1){
            fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(fchmod(fd, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
            fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
    }while(0);

    if(fd!=-1)
        close(fd), fd=-1;

    return 0;
}
CRT_FUNCDECL(int, libcrt_clear,(char *file))
{
    int fd = -1;
    struct stat st;
    unsigned int length=0;
    char *buffer=NULL;

    do{
        fd = open(file, O_RDWR);
        if(fd==-1){
            fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(fchmod(fd, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
            fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        lseek(fd, 0, SEEK_END);        
        if(fstat(fd, &st)<0){ 
            fatal("fstat file ", __FILE__, __FUNCTION__, __LINE__);
            break; 
        }
        lseek(fd, 0, SEEK_SET);

        length = st.st_size;
        if(length <= 0) break;        
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
            break;     
        }
    }while(0);
    //if(buffer!=NULL)
    //    free(buffer), buffer=NULL;

    if(fd!=-1)
        close(fd), fd=-1;  
    return 0;   
}
CRT_FUNCDECL(int, libcrt_update,(char *file, char *buffer, unsigned int length))
{
    int fd = -1;
    int result = 0;

    do{
        fd = open(file, O_APPEND | O_RDWR);
        if(fd==-1){
            fatal("open file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(fchmod(fd, S_IXUSR | S_IRUSR | S_IWUSR )!=0){
            fatal("chmod file ", __FILE__, __FUNCTION__, __LINE__);
            break;
        }
        if(write(fd, buffer, length)<=0 ){
            fatal("write file ", __FILE__, __FUNCTION__, __LINE__);            
            break;        
        }
        result = 1;
    }while(0);

    if(fd!=-1)
        close(fd), fd=-1;  
    return result;
}
CRT_FUNCDECL(int, libcrt_remove,(char *file))
{
    remove(file);
    return 0;
}
