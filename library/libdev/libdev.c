#include <libcrt.h>

xdev_t *libdev_open(char *dev_name)
{
    xdev_t  *dev=NULL;
    do{
        dev = (xdev_t*) malloc( sizeof(xdev_t));
        if(dev==NULL){
            fatal("allocate memory", __FILE__, __FUNCTION__, __LINE__);
            break;       
        }
        #ifdef DEBUG_TEST
        printf("open device name (%s) \n", dev_name);
        #endif
        dev->fd = open(dev_name, O_RDWR);
        #ifdef DEBUG_TEST
        printf( "Value of fd is: %d \n", dev->fd );
        #endif
        if(dev->fd < 0){
            fatal("open device failed", __FILE__, __FUNCTION__, __LINE__);
            dev = libdev_close(dev);
            break;        
        }

    }while(0);
    return dev;
}

int libdev_read(xdev_t * dev, char *buffer, unsigned int length)
{
    return 0;
}

int libdev_write(xdev_t * dev, char *buffer, unsigned int length)
{
    return 0;
}

int libdev_ioctl(xdev_t * dev, unsigned int flags, unsigned int *value)
{
    //printf("%s = %x, %d \n", __FUNCTION__, flags, *value); 
    do{
        if(dev==NULL){
            #ifdef DEBUG_TEST            
            fatal("stack memory", __FILE__, __FUNCTION__, __LINE__);
            #endif
            break;        
        }
        #ifdef DEBUG_TEST
        printf("%s = %x, %d \n", __FUNCTION__, flags, *value);
        #endif    
        ioctl(dev->fd, flags, value); 
    }while(0);
    return 0;
}

xdev_t *libdev_close(xdev_t *dev)
{
    if(dev!=NULL){
        if(dev->fd != -1) close(dev->fd),dev->fd = -1;
        free(dev);
        dev=NULL;
    }
    return dev;
}
