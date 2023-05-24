


#include <libcrt.h>

#ifdef EXEC_PROC

pthread_t threadid;
int sound_callback_threads_ctx(char *buffer, unsigned int length){
    
    char xdate[256];
    struct cargv_stack_t  *sptr=NULL;
    struct librmc_addr_t  *core=NULL;

    xgetdate(xdate);
    #ifdef DEBUG_TEST
    printf("%s %x:%d\r\n", xdate, buffer, length);
    #endif


    #ifndef DEBUG_TEST
    printf("%s :: %x - %d \n", __FUNCTION__, buffer, length);
    #endif
    return 0;
}

void thread_procw(void*args){
    //printf("initialized thread \r\n");
    librecord_wave(sound_callback_threads_ctx, psys);
    while(libcrt_run()){
        sleep(1);
    }
}

void thread_procx(void *args){
    //printf("initialized thread \r\n");
    unsigned int progress = 0, cfree=0, ptty=0;
    
    progress = libcrt_count();
    cfree = libcrt_cfree();
    ptty = percent(progress, cfree);
    printf("thread close handle (%d) \n", ptty);
    while(libcrt_run()){
        sleep(1);
    }
}
struct sysnode_proc_t *psys=NULL;

int main(int argc, char *argv[])
{
     int        err;
    handle_t fd;
    int  num =0;
    char ptr[256];
    char szdir[256], szpath[256];
    char libmod[256];        
    //char *librarypath = "module/dbi0.mod";
    xdev_t   *dev = NULL;
    unsigned int value = 0;
    unsigned int progress =0;
    unsigned int cfree = 0;
    unsigned int ptty = 0;
    unsigned int ptty_value = 0;
    unsigned int ctrl_state = 0;

     
    psys = libcrt_sninit();
    libcrt_init(psys);
    libmic_init(psys);

    dev = libdev_open(CDEVICE_NAME);
    libdev_ioctl(dev, CTL10_DEVINIT, &value);
    err = pthread_create(&threadid, NULL, &thread_procw, (void*)argv);
	if (err != 0)
 	{
	    //#ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        //#endif
    }
    
    //fatal("xxxx", __FILE__, __FUNCTION__, __LINE__);
    ptty = 0;
    libdev_ioctl(dev, CTL10_PROGWRITE, &ptty);
    libdev_ioctl(dev, CTL10_RESTART, &ptty_value);
    progress = libcrt_count();
    while(libcrt_run() && dev!=NULL ){
        TRY:           
        //sleep(1);     
        value = 0;
        //ptty_value=0;
        cfree = libcrt_cfree();
        libdev_ioctl(dev, CTL10_STATEOFF, &value);
        ptty = percent(progress, cfree);
        //printf("progress = %d - %d , state = %d \n", ptty ,progress, value);
        libdev_ioctl(dev, CTL10_PROGWRITE, &ptty);
        //libdev_ioctl(dev, CTL10_PROGREAD, &ptty_value);
        if(libcrt_sysnode_run()==0 && !cfree && cfree <= 0){
            ptty_value = CTL10_DISABLE_VALUE;
            libdev_ioctl(dev, CTL10_POWEROFF, &ptty_value); 
            printf("Processer shutdown handle: D(%d)=F(%d)\n", libcrt_sysnode_run(), cfree);       
        }
        if(value == CTL10_DISABLE_VALUE){
            if(!ptty && ptty <= 1){
                err = pthread_create(&threadid, NULL, &thread_procx, (void*)argv);
	            if (err != 0)
 	            {
	            //#ifdef DEBUG_H
                printf("can't create thread :[%s]\n", strerror(err));
                //#endif
                }
            }
            if(ptty >= 0 && ptty <= 60 && !ctrl_state){
                psys = libcrt_snexit(psys);
                ctrl_state = 1;
                printf("close system handle: D(%d) \n", ptty);
            }
            if(ptty >= 99){
                libmic_poweroff();
                libcrt_poweroff();
                printf("Disable Project : complete progress (%d) \n", ptty);
                sleep(1);
            }
        }
    }
    if(!libcrt_sysnode_run()) libdev_ioctl(dev, CTL10_RESTART, &ptty_value);

    if(dev==NULL){
        printf("debug : free time (%s) \n", __FUNCTION__);
    }
    dev = libdev_close(dev);
    libmic_exit(psys);
    libcrt_exit(psys);
   ENDL:
    return 0;

    return 0;
}
#endif


