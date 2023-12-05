#include <libcrt.h>

struct sysnode_proc_t *psys=NULL;

char *opt = NULL;
uint32_t  opt_srv=0,opt_cfg=0;
uint32_t  opt_num=1,opt_len=0;

uint32_t close_final = 1;

zconfig_server_t iserver;

int usage(){
    printf("run service or debian install commanline [xbin -k] \n");
    printf("run program for not install commanline [xbin] \n");
}

int load_config(uint32_t flags){
    uint32_t ret= 0;    
    char szdir[256],szpath[256];

    _envp_create(&iserver);

    CCNULL(iserver.szdir);
    CCNULL(iserver.modules);
    CCNULL(iserver.mydata);
    CCNULL(iserver.temp);
    CCNULL(iserver.logs);
    
    CCNULL(iserver.address);
    CCNULL(iserver.port);
    CCNULL(iserver.listen);
    CCNULL(iserver.timeout);

    if(flags == 1){
        ret = _conf_load(_conf_config(1), "server.conf", cserver_mod, &iserver);  

        strcpy(szdir, "null");
        CCNEW(iserver.szdir , szdir);     
        
    }else{
        ret = _conf_load(_conf_config(0), "server.conf", cserver_mod, &iserver); 

        getcwd(szdir, sizeof(szdir));
        CCNEW(iserver.szdir , szdir);

    }

    _envp_add(&iserver, "epath", szdir);
    
    #ifdef DEBUG_H
    _conf_infow(&iserver);
    _envp_infow(&iserver);
    #endif

    return ret;
}

pthread_t threadid;
int sound_callback_threads_ctx(char *buffer, unsigned int length){
    
    printf("handle : %x - %d \n", buffer, length);
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
  

    progress = libcrt_count()+librmc_count();
    cfree = libcrt_cfree()+librmc_cfree();
    ptty = percent(progress, cfree);
    printf("thread close handle (%d) \n", ptty);

    close_final = 0;
    while(libcrt_run()){
        sleep(1);
    }  
    pthread_exit(NULL);
}

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
    struct mod_desc_t *mod_descx=NULL;
    struct librmc_stack_t *prmc =NULL;
    void *stack_addr;
    uint32_t stack_size;
    pthread_attr_t attr;


    close_final =1;

    signal(SIGINT,libcrt_signal_handle);	/* Interactive attention signal.  */
    signal(SIGILL,libcrt_signal_handle);	/* Illegal instruction.  */
    signal(SIGABRT,libcrt_signal_handle);	/* Abnormal termination.  */
    signal(SIGFPE,libcrt_signal_handle);	/* Erroneous arithmetic operation.  */
    signal(SIGSEGV,libcrt_signal_handle);	/* Invalid access to storage.  */
    signal(SIGTERM,libcrt_signal_handle);	/* Termination request.  */

    /* Historical signals specified by POSIX. */
    signal(SIGHUP,libcrt_signal_handle);	/* Hangup.  */
    signal(SIGQUIT,libcrt_signal_handle);	/* Quit.  */
    signal(SIGTRAP,libcrt_signal_handle);	/* Trace/breakpoint trap.  */
    signal(SIGKILL,libcrt_signal_handle);	/* Killed.  */
    signal(SIGBUS,libcrt_signal_handle);	/* Bus error.  */
    signal(SIGSYS,libcrt_signal_handle);	/* Bad system call.  */
    signal(SIGPIPE,libcrt_signal_handle);	/* Broken pipe.  */
    signal(SIGALRM,libcrt_signal_handle);	/* Alarm clock.  */

    /* New(er) POSIX signals (1003.1-2008, 1003.1-2013).  */
    signal(SIGURG,libcrt_signal_handle);	/* Urgent data is available at a socket.  */
    signal(SIGSTOP,libcrt_signal_handle);	/* Stop, unblockable.  */
    signal(SIGTSTP,libcrt_signal_handle);	/* Keyboard stop.  */
    signal(SIGCONT,libcrt_signal_handle);	/* Continue.  */
    signal(SIGCHLD,libcrt_signal_handle);	/* Child terminated or stopped.  */
    signal(SIGTTIN,libcrt_signal_handle);	/* Background read from control terminal.  */
    signal(SIGTTOU,libcrt_signal_handle);	/* Background write to control terminal.  */
    signal(SIGPOLL,libcrt_signal_handle);	/* Pollable event occurred (System V).  */
    signal(SIGXCPU,libcrt_signal_handle);	/* CPU time limit exceeded.  */
    signal(SIGXFSZ,libcrt_signal_handle);	/* File size limit exceeded.  */
    signal(SIGVTALRM,libcrt_signal_handle);	/* Virtual timer expired.  */
    signal(SIGPROF,libcrt_signal_handle);	/* Profiling timer expired.  */
    signal(SIGUSR1,libcrt_signal_handle);	/* User-defined signal 1.  */
    signal(SIGUSR2,libcrt_signal_handle);	/* User-defined signal 2.  */

    /* Nonstandard signals found in all modern POSIX systems
    (including both BSD and Linux).  */
    signal(SIGWINCH,libcrt_signal_handle);	/* Window size change (4.3 BSD, Sun).  */

    /* Archaic names for compatibility.  */
    signal(SIGIO,libcrt_signal_handle);	/* I/O now possible (4.2 BSD).  */
    signal(SIGIOT,libcrt_signal_handle);	/* IOT instruction, abort() on a PDP-11.  */
    signal(SIGCLD,libcrt_signal_handle);	/* Old System V name */
       
    psys = libcrt_sninit();
    libcrt_init(psys);
    
    dev = libdev_open(CDEVICE_NAME);
    if(dev==NULL){
        fatal("device initialized", __FILE__, __FUNCTION__, __LINE__);
        goto HDC;
    }
    libdev_ioctl(dev, CTL10_DEVINIT, &value);

    libmic_init(psys);


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
            }    
            opt+=1;
            opt_len--;
        }while(opt_len>0);

        
        opt_num++;
    }



    //if(argc > 1){
    if(!load_config(opt_cfg)){
        usage();
        goto HDC;    
    }
    

    err = pthread_create(&threadid, NULL, &thread_procw, (void*)argv);
	if (err != 0)
 	{
	    //#ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        //#endif
    }
    pthread_getattr_np(threadid, &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    //mod_thread_update(stack_ptr, threadid, stack_addr, MAIN_CERT_VERIFY_VERSION);
 
    //fatal("xxxx", __FILE__, __FUNCTION__, __LINE__);
    ptty = 0;
    libdev_ioctl(dev, CTL10_PROGWRITE, &ptty);
    libdev_ioctl(dev, CTL10_RESTART, &ptty_value);
    progress = libcrt_count()+librmc_count();
    while(libcrt_run() && dev!=NULL ){
        TRY:         
        value = 0;
        //ptty_value=0;
        cfree = libcrt_cfree()+librmc_cfree();
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
        //printf("handle control value (%d) \n", value);

        if(value == CTL10_DISABLE_VALUE){
            if(!ptty && ptty <= 1){
                err = pthread_create(&threadid, NULL, &thread_procx, (void*)argv);
	            if (err != 0)
 	            {
	            //#ifdef DEBUG_H
                printf("can't create thread :[%s]\n", strerror(err));
                //#endif
                }
                //mod_disable_service(stack_ptr);
            }
            if(ptty >= 0 && ptty <= 60 && !ctrl_state){
                psys = libcrt_snexit(psys);
                ctrl_state = 1;
                printf("close system handle: D(%d) \n", ptty);
                
            }
            if(ptty >= 90 && !close_final && close_final ==0){
                if(opt_srv){
                    libsck_poweroff();
                }
                librmc_poweroff();
                //libmic_poweroff();
                libcrt_poweroff();
                printf("Disable Project : complete progress (%d) \n", ptty);
                sleep(5);
                continue;
            }
        }else{     
            sleep(1);  
        }
    }
    HDC:
   
    //printf("-----------------------------------\n");
    if(!libcrt_sysnode_run()) libdev_ioctl(dev, CTL10_RESTART, &ptty_value);
    //printf("-----------------------------------\n");
    if(dev==NULL){
        printf("debug : free time (%s) \n", __FUNCTION__);
    }
    //printf("-----------------------------------\n");
    dev = libdev_close(dev);
    //printf("-----------------------------------\n");

    //printf("-----------------------------------\n");
    libmic_exit(psys);
    //printf("-----------------------------------\n");
    libcrt_exit(psys);

    _envp_clean(&iserver);

    return 0;
}
