#include "libcda.h"


xcda_handle_t *libcda_open(char *dev_name)
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
            dev = libcda_close(dev);
            break;        
        }

    }while(0);
    return dev;
}

int libcda_setheader(xcda_handle_t *dev,  xcda_header_t *str)
{
    return 0;
}
int libcda_getheader(xcda_handle_t *dev,  xcda_header_t **str)
{
    return 0;
}

int libcda_read(xcda_handle_t * dev, char *buffer, unsigned int length)
{
    return 0;
}

int libcda_write(xcda_handle_t * dev, char *buffer, unsigned int length)
{
    return 0;
}

int libcda_ioctl(xcda_handle_t * dev, unsigned int flags, unsigned int *value)
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

xdev_t *libcda_close(xcda_handle_t *dev)
{
    if(dev!=NULL){
        if(dev->fd != -1) close(dev->fd),dev->fd = -1;
        free(dev);
        dev=NULL;
    }
    return dev;
}
