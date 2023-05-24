



#include <libcrt.h>

#ifdef EXEC_PROC
int main(int argc, char *argv[])
{
    //fatal("xxxx", __FILE__, __FUNCTION__, __LINE__);
    xdev_t   *dev = NULL;
    unsigned int ptty_value = 0;
    unsigned int old_value = 0;
    unsigned int count = 0;
   do{ 
    
    dev = libdev_open(CDEVICE_NAME);
    if(argc > 1){
            
        if(strcmp(argv[1], "restart")==0){
            printf("restart project \n");
            ptty_value = 1;
            libdev_ioctl(dev, CTL10_RESTART, &ptty_value); 
            break;        
        }    
    
    }
    printf("disable project \n");
    
    ptty_value = CTL10_DISABLE_VALUE;
    libdev_ioctl(dev, CTL10_POWEROFF, &ptty_value);
    while(dev!=NULL){
        sleep(1);
        if(count >= 5) break;
        ptty_value = 0;
        libdev_ioctl(dev, CTL10_PROGREAD, &ptty_value);
        if(ptty_value >= 99){ 
            printf("Disable Project : complete progress (%d) \n", ptty_value);
            break;
        }
        printf("progress : (%3d) \n", ptty_value);
        if(ptty_value < 1){
            count += 1;        
        }else
        if(ptty_value == old_value){
            count += 1;        
        }else{
            old_value = ptty_value;
        }
        
    }
    libdev_ioctl(dev, CTL10_RESTART, &ptty_value); 

    dev = libdev_close(dev);
    }while(0);

    return 0;
}
#endif


