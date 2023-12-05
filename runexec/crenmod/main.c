#include <libcrt.h>

int main(int argc, char *argv[])
{   
    char szdir[256],szmods[256], szmodu[256];
    handle_t fd;
    int  num =0;
    char ptr[256];
    char libcw[256], libca[256];

    if(argc > 1 && strcmp(argv[1],"-k") == 0){
        strcpy(szmods, "/usr/lib/carroll\0");
        strcpy(szmodu, "/etc/carroll/mods\0");
    }else{
        getcwd(szdir, sizeof(szdir));
        strcpy(szmods, szdir);
        strcat(szmods, "/module\0");
        strcpy(szmodu, szdir);
        strcat(szmodu, "/mods\0");
    }

    fd = find_init( szmods );
    if(fd != NULL){
        
        while(fd != NULL && (num = find_next(fd, ptr))>0){

            memset(libcw,0, sizeof(libcw));
            memset(libca,0, sizeof(libca));
            
            if(num == 2){
              //printf("DIR::MOD: %s \n", CCSHOW(iserver.szdir));     
              strcpy(libcw, szmods);
              strcat(libcw, "/");
              strcat(libcw, ptr);

              strcpy(libca, szmodu);
              strcat(libca, "/");              
              strcat(libca, ptr);

              printf("enmod [%s] ln -s %s %s \n", ptr, libcw, libca);
              
              symlink(libcw, libca);
              //char *binaryPath = "ln";
              //char *args[]={binaryPath,"-s", libcw, libca, NULL };
              //execle(binaryPath, binaryPath, "-s", libcw, libca , NULL);

            } 
        }
        //printf("load module library (%d)\n", __LINE__);   
        find_exit(fd);
        //printf("load module library (%d)\n", __LINE__);    
    }

    return 0;
}
