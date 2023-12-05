#include <libcrt.h>

int main(int argc, char *argv[])
{
    char szdir[256],szmods[256], szmodu[256];
    handle_t fd;
    int  num =0;
    char ptr[256];
    char params[256], libca[256];
    char *str=NULL;
    uint32_t length = 0;

    memset(params,0, sizeof(params));
    if(argc > 1){
        if(argc > 2 && strcmp(argv[1],"-k") == 0){
            strcpy(params, argv[2]);
        }else{
            strcpy(params, argv[1]);        
        }
        strcpy(szmods, "/usr/lib/carroll\0");
        strcpy(szmodu, "/etc/carroll/mods\0");

    }else{
        getcwd(szdir, sizeof(szdir));
        strcpy(szmods, szdir);
        strcat(szmods, "/module\0");
        strcpy(szmodu, szdir);
        strcat(szmodu, "/mods\0");
    }
    if((str= strchr(params, '-'))!=NULL){
        length = strlen(str);
        params[length] = '\0';
    }
    fd = find_init( szmods );
    if(fd != NULL){
        
        while(fd != NULL && (num = find_next(fd, ptr))>0){
            if(num == 2){
              
              if(strncmp(ptr, params, strlen(params)) ==0){
                strcpy(libca, szmodu);
                strcat(libca, "/");              
                strcat(libca, ptr);
              
                chmod(libca, 0755);
                remove(libca);

              }
            }  
        }
        //printf("load module library (%d)\n", __LINE__);   
        find_exit(fd);
        //printf("load module library (%d)\n", __LINE__);    
    }

    return 0;
    return 0;
}
