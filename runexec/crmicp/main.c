#include <libcrt.h>

struct sysnode_proc_t *psys=NULL;

char *opt = NULL;
uint32_t  opt_srv=0,opt_cfg=0;
uint32_t  opt_num=1,opt_len=0;

uint32_t close_final = 1;
int opt_sel = -1;

zconfig_server_t iserver;
struct libvbt_t ivbth;

int usage(){
    printf("run service or debian install commanline [crmicp -k] \n");
    printf("run program for not install commanline [crmicp] \n");
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
        ret = _conf_load(_conf_config(1), "client.conf", cserver_mod, &iserver);  

        strcpy(szdir, "null");
        CCNEW(iserver.szdir , szdir);     
        
    }else{
        ret = _conf_load(_conf_config(0), "client.conf", cserver_mod, &iserver); 

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

    cwrite(&iserver, buffer, length);

    return 0;
}

void thread_procw(void*args){
    //printf("initialized thread \r\n");
    int vbid=0;
    int i=0;
    //
    libsck_client(&iserver);
    libsck_info(&iserver);
    
    //libvbt_desc(&ivbth);
    //printf("start scan libdbus initialize \n");
    //libdbus_address(&ivbth);
    //libdbus_devices(&ivbth);
    //libdbus_name(&ivbth);
    //libdbus_audio(&ivbth);
    //libdbus_micophone(&ivbth);
    //printf("final scan libdbus initialize \n");
    //cmd_list(&ivbth);
    #ifdef DEBUG    
    for(i=1;i<=ivbth.count;i++){
        //if(libvbt_test(st,i)){
        //    mic = i;
        //    break;
        //}
        //str2ba(st->addr, &raddr.rc_bdaddr);
        ivbth.bdaddr = *BDADDR_ANY;
        cmd_connect(ivbth.ctl, ivbth.dev_id, &ivbth.bdaddr, &ivbth);
        //printf("address [%s] \n", ivbth.addr);

        libvbt_update(&ivbth, i);  
    }
    #endif

    if(opt_sel >= 0){
       vbid = opt_sel;
    }else{
       printf("select mode record : 0 = [microphone], 1++ = [bluetooth] : ");
       scanf("%d", &vbid);
    }
    if(vbid == 0){ // microphine mode
        librecord_wave(sound_callback_threads_ctx, psys);
    }else{ // bluetooth mode
        //printf("select list device bluetooth to record number %d : ", vbid);
        //vbid = libvbt_micp(&ivbth, ivbth.number);
        printf("select list device bluetooth to record number %d : \n", vbid);
        //if(vbid > 0 && libvbt_update(&ivbth, vbid)){
        //     libvbt_init(&ivbth);
        //     printf("close device bluetooth to record number %d : \n", vbid);   
        //     libvbt_callback(&ivbth, sound_callback_threads_ctx);
        //     libvbt_close(&ivbth);
        //}
       
    }
    //librecord_wave(sound_callback_threads_ctx, psys);
    
    while(libcrt_run()){
        sleep(1);
    }
    libvbt_poweroff();
}

void thread_procm(void*args){
    
    //libvbt_smcl(&ivbth);
    //
    //cmd_info(NULL);
    
    //cmd_create(ivbth.ctl, ivbth.dev_id, &ivbth.bdaddr, &ivbth);
    //cmd_listen(ivbth.ctl, ivbth.dev_id, &ivbth.bdaddr, &ivbth);

    while(libcrt_run()){
        sleep(1);
    }
    //cmd_release(ivbth.ctl, ivbth.dev_id, &ivbth.bdaddr, &ivbth);
   

    libvbt_poweroff();
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
               case 'f':
                  opt_num++;
                  opt_sel= atoi(argv[opt_num]);   
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

    ivbth.channel = 1;

    hci_init(&ivbth);

    //ivbth.number = libvbt_list(&ivbth);  
    //cmd_init(&ivbth);
    
    err = pthread_create(&threadid, NULL, &thread_procm, (void*)argv);
	if (err != 0)
 	{
	    //#ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        //#endif
    }
    pthread_getattr_np(threadid, &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);

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

    cmd_close(&ivbth);
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
