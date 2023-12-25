#ifndef LIBCRT_H
#define LIBCRT_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> 
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>      /*  timeval */
#include <sys/ioctl.h>     /*  ioctl  */
#include <pthread.h>
#include <signal.h>
#include <alsa/asoundlib.h>

// superset of unistd, same

//Bluetooth
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>

//socket
#include <sys/socket.h>

#include <config.h>

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define CHECK(x) \
    int __sdk_check(x){ if(x == NULL){ \
        int result = 1; \
            fatal("segment memory", __FILE__, __FUNCTION__, __LINE__); \
            result = 0; \
        } \
        return result; \
    } 

#define CRT_FUNCDECL(ret,func, params) ret func params

typedef void * handle_t;
typedef long * addr_t;

typedef unsigned char *POINTER;
/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;
/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef _SYS_TYPES_H
typedef long long unsigned int uint64_t;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#ifndef SOCKET_ERROR 
#define SOCKET_ERROR  INVALID_SOCKET
#endif
#ifndef SD_SEND
#define SD_SEND    SHUT_WR
#endif
#ifndef SD_RECEIVE
#define SD_RECEIVE SHUT_RD
#endif
#ifndef SD_BOTH
#define SD_BOTH    SHUT_RDWR
#endif
#ifndef closesocket
#define closesocket(x)  close(x)
#endif

struct cargv_stack_t;
struct sysnode_proc_t;

CRT_FUNCDECL(void, fatal,(char *msg, char *file, char *func, unsigned int line));

CRT_FUNCDECL(int, libcrt_init,(struct sysnode_proc_t *ptr));
CRT_FUNCDECL(int, libcrt_exit,(struct sysnode_proc_t *ptr));
CRT_FUNCDECL(int, libcrt_run,(void));
CRT_FUNCDECL(int, libcrt_error,(char *err));

CRT_FUNCDECL(handle_t, find_init,(char *dir));
CRT_FUNCDECL(int, find_next,(handle_t fd, char *name));
CRT_FUNCDECL(void, find_exit,(handle_t fd));

CRT_FUNCDECL(int, libcrt_poweroff,(void));

CRT_FUNCDECL(float,  percent,(unsigned int total_marks,unsigned int scored));
CRT_FUNCDECL(int, xgetdate,(char *buffer));
CRT_FUNCDECL(void, hexdump,(const unsigned char *src, size_t len, unsigned long fp));

extern
unsigned int crt_count;
extern
unsigned int crt_free;

CRT_FUNCDECL(unsigned int ,libcrt_count,());
CRT_FUNCDECL(unsigned int ,libcrt_cfree,());


CRT_FUNCDECL(char *,cdigest,(unsigned char *p,unsigned int len));
CRT_FUNCDECL(void, libcrt_signal_handle,(int sign));

CRT_FUNCDECL(int, libcrt_create,(char *file));
CRT_FUNCDECL(int, libcrt_clear,(char *file));
CRT_FUNCDECL(int, libcrt_update,(char *file, char *buffer, unsigned int length));
CRT_FUNCDECL(int, libcrt_remove,(char *file));

typedef int (*crt_callback_thread)(char *buffer);

CRT_FUNCDECL(unsigned long,
search,(const char *path, crt_callback_thread ptr));

CRT_FUNCDECL(void, get_time_start,(uint32_t *min, uint32_t *sec));
CRT_FUNCDECL(uint32_t, get_time_final,(uint32_t min, uint32_t sec, uint32_t *lmin, uint32_t *lsec));

CRT_FUNCDECL(char *, libcrt_new_str,(char *str));

struct sysnode_addr_t
{
    unsigned int         num;
    unsigned int         erro;
    char                 *name;
                
    unsigned int         size;
    addr_t               sysd;
    
    void                *obj;
    struct node_addr_t  *next;
};

struct sysnode_proc_t
{
    unsigned int          num;
    struct node_addr_t   *addr;
};

struct crt_struct_t
{
    int (*ctrl)(char *proc, unsigned int flags, handle_t *str); 
};

CRT_FUNCDECL(struct sysnode_proc_t *,libcrt_sysnode_init,());
CRT_FUNCDECL(struct sysnode_proc_t *,libcrt_sysnode_exit,(struct sysnode_proc_t *str));
CRT_FUNCDECL(int, libcrt_sysnode_proc,(struct sysnode_proc_t *str));
CRT_FUNCDECL(int, libcrt_sysnode_update,(struct sysnode_proc_t *str, char *sysdev, addr_t module, unsigned int size));
CRT_FUNCDECL(int, libcrt_sysnode_ctrl,(char *proc, unsigned int flags, handle_t *str));
CRT_FUNCDECL(int, libcrt_sysnode_error,(char *str));
CRT_FUNCDECL(int, libcrt_sysnode_run,(void));
CRT_FUNCDECL(void*, libcrt_sysnode_find,(char *name));
extern 
struct sysnode_proc_t *crt_module;
extern
unsigned int sysn_module;

#define libcrt_sninit          libcrt_sysnode_init
#define libcrt_snexit          libcrt_sysnode_exit 
#define libcrt_snproc(a)       libcrt_sysnode_proc(a)
#define libcrt_snupdate(a,b,c,d) libcrt_sysnode_update(a,b,c,d)
#define libcrt_snerror(x)      libcrt_sysnode_error(x)
/* ======================================================================= 







=========================================================================*/
typedef struct _xcstring_t
{
	char     *buffer;
	uint32_t length;
}xcstring_t;


uint32_t einit(void *s);
uint32_t efini(void *s);
void *emalloc(uint32_t  length);
void *efree(void *ptr);

uint32_t cstring_create(xcstring_t** ptr);
uint32_t cstring_clean(xcstring_t** ptr);
uint32_t cstring_clone(xcstring_t** ptr, xcstring_t *str);
uint32_t cstring_check(xcstring_t **ptr);
char* cstring_str(xcstring_t *ptr);
uint32_t cstring_len(xcstring_t *ptr);
uint32_t cstring_newadd(xcstring_t **ptr, xcstring_t *dst , xcstring_t *src);
uint32_t cstring_new(xcstring_t **ptr, const char *buf);
uint32_t cstring_newpad(xcstring_t **ptr, const char *buf,const char *pad);
uint32_t cstring_renew(xcstring_t **ptr, const char *buf);
uint32_t cstring_stat(xcstring_t **ptr, const char v);
uint32_t cstring_remove(xcstring_t **ptr, uint32_t len);
uint32_t cstring_clear(xcstring_t **ptr);
uint32_t cstring_delete(xcstring_t **ptr);
uint32_t cstring_restr(xcstring_t **ptr, char *str);
/* ======================================================================= 







=========================================================================*/


#define CPRINT(c, x) if(x!=NULL){ printf("%16s:%s\n",c, x); }
#define CLEAN(x)  if(x!=NULL) (x= efree(x)),x=NULL;
#define CNULL(x)  x=NULL;

#define CCSTAT(x,l)       cstring_stat(&x,l)
#define CCREMOVE(x, l)    cstring_remove(&x, l)
#define CCNEWPAD(x,c,s) cstring_newpad(&x,c,s)
#define CCNEWADD(x,a,b) cstring_newadd(&x,a,b)
#define CCNEW(x,c)  cstring_new(&x,c)
#define CCRENEW(x,l) cstring_renew(&x, l)
#define CCCLONE(x,l) cstring_clone(&x, l)
#define CCSHOW(x)  cstring_str(x)
#define CCCHECK(x) cstring_check(&x)
#define CCLEAN(x) cstring_clean(&x)
#define CCNULL(x) cstring_create(&x)
#define CCCLEAR(x) cstring_clear(&x)
#define CCDELETE(x) cstring_delete(&x)
#define CCNEWSTR(x,p)  cstring_restr(&x, p)


#define _MGC_STRING  xcstring_t *

typedef struct _zconfig_envpath_t
{
    _MGC_STRING                 name;
    _MGC_STRING                 value;
    struct _zconfig_envpath_t  *next;
}zconfig_envpath_t;
typedef struct _zconfig_server_t
{       
     int                   fix;
     int                   fds;
     int                   fdc;

    _MGC_STRING            szdir;

    _MGC_STRING            address;
    _MGC_STRING            port;
    _MGC_STRING            listen;
    _MGC_STRING            timeout;

    _MGC_STRING            modules;
    _MGC_STRING            mydata;
    _MGC_STRING            temp;
    _MGC_STRING            logs;

    uint32_t                    count;
    struct _zconfig_envpath_t  *envp;
}zconfig_server_t;

typedef int (*cserver_string)(char *str, zconfig_server_t *iserver);

char * _conf_config(uint32_t flags);

int cserver_mod(char *str, zconfig_server_t *iserver );
uint32_t  _conf_copy(zconfig_server_t **info, const char *buff);
char* _anycfg_value(char *data, unsigned int seek_flags);
uint32_t  _conf_load(const char *root, const char *file, cserver_string mod, zconfig_server_t *iserver );
uint32_t  _conf_infow( zconfig_server_t *info);
uint32_t  _conf_clean(zconfig_server_t **info);
uint32_t  _conf_null(zconfig_server_t **info);
uint32_t  _conf_readline(const char *buff,cserver_string mod, zconfig_server_t *iserver );
unsigned int get_filesize(int fd);

extern 
zconfig_server_t *server;

/* ======================================================================= 







=========================================================================*/

uint32_t _envp_create(zconfig_server_t *ptr);
uint32_t _envp_clean(zconfig_server_t *ptr);
char *_envp_get(zconfig_server_t *ptr, char *name);
uint32_t _envp_add(zconfig_server_t *ptr, char *name, char *value);
uint32_t _envp_set(zconfig_server_t *ptr, char *name, char *value);
uint32_t  _envp_infow( zconfig_server_t *ptr);

/* ======================================================================= 







=========================================================================*/
typedef int (*socket_callback_threads_ctx)(char *buffer, unsigned int length);

int libsck_server(zconfig_server_t *config);
int libsck_client(zconfig_server_t *config);
int libsck_close(zconfig_server_t *config);
int libsck_shutdown(zconfig_server_t *config);
int libsck_callback(zconfig_server_t *config, socket_callback_threads_ctx ctx);
int libsck_poweroff(void);
int libsck_recv(zconfig_server_t *config, char *buffer, uint32_t length);
int libsck_send(zconfig_server_t *config, char *buffer, uint32_t length);
/* ======================================================================= 







=========================================================================*/
#define N64K_SIZE  64000
#define N56K_SIZE  56000

#define HASH_SIZE  64
#define SKEY_SIZE  32

typedef struct _libchash_t // unicode
{
   char       hash[HASH_SIZE];
   u_int      length;
}libchash_t;

typedef struct _libckey_t
{
    char       key[SKEY_SIZE];
    u_int      length;
}libckey_t;

typedef struct _libc0_t
{
    uint8_t    s0,s1,s2;
    uint32_t   h1,h2;
    char       private_key[SKEY_SIZE];
    uint32_t   b1,b2;
    char       public_key[SKEY_SIZE];

    uint32_t   core;    
    u_int      algo;
    
    libchash_t mhash;
    libckey_t  skey;
    libckey_t  ckey;
    
    u_int      number;
    u_int      vstate;
    u_int      ustatus;
    u_int      length;
    u_int      maxinum;
    u_int      mininum;
    u_int      rbits; 
    uint8_t    *buffer;
} libc0_t;

 int  libc0_decrypt(libc0_t *c, uint8_t *buffer, uint32_t length);
 int  libc0_encrypt(libc0_t *c, const uint8_t *buffer, uint32_t length);

 int  libc0_context_info(libc0_t *ctx);

#define LEXNE_CERT_PRIVATE_KEY "lexne 2008 inc,."

char *libmsx_encode(const char *buffer, uint32_t len, char *keys, uint32_t *length);
char *libmsx_decode(const char *buffer, uint32_t len, char *keys, uint32_t *length);

#define CDA_KEYS  "CDA"

int libmsx_encode_file(const char *file,char *buf, uint32_t len, char *keys, uint32_t seek);
char *libmsx_decode_file(const char *file,  uint32_t *length, char *keys, uint32_t seek);
/* ======================================================================= 







=========================================================================*/
#ifndef MGC_SEEK1
#define MGC_SEEK1     22048
//#define MGC_SEEK1     24576
#endif

#define SSC_STREAM_MAX       1024*2
#define SSC_STREAM_BLOCK     24
//#define SSVN_TSIZE   (SSC_STREAM_MAX*SSC_STREAM_BLOCK)

#define LIBMGC_POINT         1 // 32/2

#define EXT_C(x)        x
#define LIBPUBSIZEA     EXT_C(245) // +32 118
#define LIBPUBSIZEW     EXT_C(25)

#define SOUND_BEGIN   1
#define SOUND_CUR     2
#define SOUND_FINAL   3
#define SOUND_WAIT    4

struct sound_struct_t {
    char            *buffer;
    uint32_t        length;


    uint32_t        block;
    uint32_t        check;

    uint32_t        flags;
};
extern
int libmic_run_state;

CRT_FUNCDECL(int, libmic_init,(struct sysnode_proc_t *ptr));
CRT_FUNCDECL(int, libmic_exit,(struct sysnode_proc_t *ptr));

CRT_FUNCDECL(int, libmic_poweroff,(void));
CRT_FUNCDECL(int, libmic_run,(void));

CRT_FUNCDECL(int, libmic_create,(struct sound_struct_t *ptr, char *buffer, unsigned int length));
CRT_FUNCDECL(struct sound_struct_t *,libmic_alloc,(void));
CRT_FUNCDECL(struct sound_struct_t *,libmic_realloc,(struct sound_struct_t *ptr));
CRT_FUNCDECL(struct sound_struct_t *,libmic_free,(struct sound_struct_t *ptr));
CRT_FUNCDECL(struct sound_struct_t *,libmic_empty,(struct sound_struct_t *ptr));
CRT_FUNCDECL(uint32_t,libmic_surw,(struct sound_struct_t *pstr_ctx,char *buffer, unsigned int length));
typedef uint32_t *(*sound_struct_threads)(struct sound_struct_t *pstr_ctx,char *buffer, unsigned int length);
CRT_FUNCDECL(uint32_t, sound_detection,(struct sound_struct_t **ptr, sound_struct_threads ctx, char *buf, unsigned int len, uint32_t *flags));

typedef int *(*sound_callback_threads)(char *buffer, unsigned int length);

struct sound_callback
{
    int (*callback)(char *buffer, unsigned int length);
    int (*libmic_init)(struct sysnode_proc_t *str);
    int (*libmic_exit)(struct sysnode_proc_t *str);

    sound_struct_threads libmic_surx;
};

CRT_FUNCDECL(int, librecord_wave,(sound_callback_threads ctx, struct sysnode_proc_t *str));

/* ======================================================================= 







=========================================================================*/
#ifndef DEBUG_TEST 
#define DEBUG_SHOW(x,a,b,c) 
#else
#define DEBUG_SHOW(x,a,b,c)  printf("%s: %x - %d, %x\n", x, a, b, c); 
#endif

#define MAIN_CERT_VERIFY_VERSION  0x0001
#define LRMC_CERT_VERIFY_VERSION  0x0002
#define BCDX_CERT_VERIFY_VERSION  0x1000
#define BIO0_CERT_VERIFY_VERSION  0x1001
#define COR0_CERT_VERIFY_VERSION  0x1002
#define CTL0_CERT_VERIFY_VERSION  0x1003
#define DBI0_CERT_VERIFY_VERSION  0x1004
#define DBO0_CERT_VERIFY_VERSION  0x1005
#define DEV0_CERT_VERIFY_VERSION  0x1006
#define GAT0_CERT_VERIFY_VERSION  0x1007
#define HOL0_CERT_VERIFY_VERSION  0x1008
#define ICE0_CERT_VERIFY_VERSION  0x1009
#define LOG0_CERT_VERIFY_VERSION  0x1010
#define MOD0_CERT_VERIFY_VERSION  0x1011
#define POP0_CERT_VERIFY_VERSION  0x1012
#define RWX0_CERT_VERIFY_VERSION  0x1013
#define SEC0_CERT_VERIFY_VERSION  0x1014
#define STK0_CERT_VERIFY_VERSION  0x1015
#define VID0_CERT_VERIFY_VERSION  0x1016
#define THI0_CERT_VERIFY_VERSION  0x1017
#define MAP0_CERT_VERIFY_VERSION  0x1018
#define SYS0_CERT_VERIFY_VERSION  0x1019
#define DBM0_CERT_VERIFY_VERSION  0x1020
#define DBV0_CERT_VERIFY_VERSION  0x1021

struct mod_struct_t;
struct mod_stack_t;
typedef struct sound_struct_t cstring_t;



#define DIGEST_LENGTH  16

#define CORE_EMPTY      0
#define CORE_HANDLE     1

#define LPVOIDX void **
struct core_addr_t 
{
    unsigned int        status;

    char                *code;
    
    char                *buffer;
    unsigned int         length;  


    struct core_addr_t   *next;
};

struct core_stack_t 
{
    unsigned int          count;
    unsigned int          cread;
    unsigned int          cwrite;
    struct core_addr_t   *addr;  
};

extern 
struct core_stack_t *sptr_stack;

struct data_struct_t;

typedef int (*xdata_init)(struct data_struct_t *ptr);
typedef int (*xdata_exit)(struct data_struct_t *ptr);
typedef int (*xdata_ctrl)(struct data_struct_t *ptr, unsigned int flags, LPVOIDX string);

struct data_stack_t
{
    unsigned int  pos;
    unsigned int  max;
    unsigned int  state;  
};
struct data_addr_t
{
    unsigned int         seid;
    char                *code;
    char                *base;
    struct data_addr_t  *next;
};
struct data_struct_t
{
    char            *xdev;

    xdata_init      xinit;
    xdata_exit      xexit;
    xdata_ctrl      xctrl;

    struct data_stack_t *stack;

    unsigned int         nbase;
    struct data_addr_t  *addr;
};

extern 
struct data_struct_t  *data_ext;

struct mod_struct_t{
    char *fdev;
    char *name;
    char *szdir;

    void *object;
    void *handle;
    
    void *process;
    void *system;

    uint8_t service;
        
    void *config;    

    int (*init)(char *path_dev, struct mod_struct_t* ext_ptr, struct sysnode_proc_t *str);
    int (*exit)(struct mod_struct_t* ext_ptr, struct sysnode_proc_t *str);
    int (*find)(char *str);
    int (*xread)(unsigned int id, cstring_t *str);
    int (*xwrite)(unsigned int id, cstring_t *str);
    int (*xfree)(cstring_t *str);
    int (*update)(char *buffer, unsigned int length); 
};

struct mod_args_t{
    struct mod_struct_t *argv0;
    struct cargv_stack_t *argv1;
};

extern
struct mod_struct_t *mod_ext;

#define EXT_C(x)    x

#define xmod_init_params     struct mod_struct_t *ptr, struct sysnode_proc_t *str
#define xmod_free_params     struct mod_struct_t *ptr
#define xmod_exit_params     struct mod_struct_t *ptr, struct sysnode_proc_t *str
#define xmod_extw_params     void
#define xmod_alloc_params    struct mod_stack_t *stack, zconfig_server_t *szdir
#define xmod_getver_params   void
#define xmod_ctrl_params     struct mod_struct_t *ptr, unsigned int flags, LPVOIDX string
#define xmod_name_params     void

typedef int (*xmod_getver) (xmod_getver_params);
typedef int (*xmod_init)(xmod_init_params);
typedef int (*xmod_exit)(xmod_exit_params);
typedef struct mod_struct_t *(*xmod_extw)(xmod_extw_params);
typedef struct mod_struct_t *(*xmod_alloc)(xmod_alloc_params);
typedef int (*xmod_ctrl)(xmod_ctrl_params);
typedef char* (*xmod_name)(xmod_name_params);

#define SMOD_GETVER "zmod_ver"
#define SMOD_INIT   "zmod_init"
#define SMOD_EXIT   "zmod_exit"
#define SMOD_EXTW   "zmod_extw"
#define SMOD_CTRL   "zmod_ctrl"
#define SMOD_ALLOC  "zmod_alloc"
#define SMOD_NAME   "zmod_name"


#define FMOD_GETVER EXT_C(zmod_ver)
#define FMOD_INIT   EXT_C(zmod_init)
#define FMOD_EXIT   EXT_C(zmod_exit)
#define FMOD_EXTW   EXT_C(zmod_extw)
#define FMOD_CTRL   EXT_C(zmod_ctrl)
#define FMOD_ALLOC  EXT_C(zmod_alloc)
#define FMOD_NAME   EXT_C(zmod_name)
#define FMOD_FREE   EXT_C(zmod_free)

int FMOD_GETVER(xmod_getver_params);
char *FMOD_NAME(xmod_name_params);
int FMOD_INIT(xmod_init_params);
int FMOD_EXIT(xmod_exit_params);
struct mod_struct_t *FMOD_EXTW(xmod_extw_params);
struct mod_struct_t *FMOD_ALLOC(xmod_alloc_params);
struct mod_struct_t *FMOD_FREE(xmod_free_params);
int FMOD_CTRL(xmod_ctrl_params);
//char* mod_name(void);

struct mod_addr_t
{
    char         *module_name;

    unsigned int seid;
    
    void *libhandle;

    char        *xm_name;
    xmod_getver  xm_getver;
    xmod_init    xm_init;
    xmod_exit    xm_exit;
    xmod_extw    xm_extw;
    xmod_alloc   xm_alloc;
    xmod_ctrl    xm_ctrl;
    struct mod_addr_t *next;
};
struct mod_thread_t
{
    uint32_t             pid;
    void*                addr;
    
    pthread_t            tid;
    struct mod_thread_t *next;
};
struct mod_stack_t
{
    unsigned int        count;
    struct mod_addr_t   *addr;

    uint32_t             thcount; 
    struct mod_thread_t *thaddr;
};

struct mod_desc_t
{
    char  *name;    
    char  *desc;
    char  *interface;

    uint32_t service;
    uint32_t hardware;
    uint32_t bus;
    uint32_t serial;
};
#define STACK_READ   1
#define STACK_WRITE  2
#define STACK_UPDATE 3
#define STACK_APPEND 4
#define STACK_SEARCH 5
#define STACK_RESULT 6
#define STACK_SEND   7
#define STACK_INSERT 8

#define STACK_DESC   30
#define STACK_VERIFY 31
#define STACK_SERVICE 40

CRT_FUNCDECL(struct mod_stack_t *,mod_alloc,(void));
CRT_FUNCDECL(int, mod_create,(struct mod_stack_t *ptr, unsigned int cnt));
CRT_FUNCDECL(int, mod_update,(struct mod_stack_t *ptr, zconfig_server_t *path, char *libmod, struct sysnode_proc_t *str));
CRT_FUNCDECL(int, mod_free,(struct mod_stack_t *ptr, struct sysnode_proc_t *str));
CRT_FUNCDECL(int, mod_print,(struct mod_stack_t *ptr));
CRT_FUNCDECL(int, mod_post,(struct mod_stack_t *ptr, char *find_mod, unsigned int ctrl, void* value));
CRT_FUNCDECL(struct mod_addr_t  *, mod_object,(struct mod_stack_t *ptr));
CRT_FUNCDECL(char*, mod_getname,(struct mod_addr_t *ptr));
CRT_FUNCDECL(struct mod_desc_t *, mod_new,(struct mod_desc_t *ptr, struct mod_desc_t *str));
CRT_FUNCDECL(struct mod_desc_t *, mod_clean,(struct mod_desc_t *ptr));


CRT_FUNCDECL(char*, mod_getmod_func,(struct mod_stack_t *ptr, uint32_t result));

CRT_FUNCDECL(int, mod_thread_init, (struct mod_stack_t *ptr));
CRT_FUNCDECL(int, mod_thread_update, (struct mod_stack_t *ptr, pthread_t thread, void *addr, uint32_t pid));
CRT_FUNCDECL(int, mod_thread_exit, (struct mod_stack_t *ptr));
CRT_FUNCDECL(int, mod_thread_close, (struct mod_stack_t *ptr, uint32_t pid));
CRT_FUNCDECL(int, mod_disable_service, (struct mod_stack_t *ptr));
#define HANDLE_DESC \
    struct mod_desc_t *mod = (struct mod_desc_t *)*string; \
    *string = (struct mod_desc_t*) mod_new(mod, &mod_desc);

#define HANDLE_MOD_INIT(p) \
    if(p!=NULL){ \
        p->szdir=NULL;\
        p->name=NULL; \
        p->fdev=NULL; \
        p->handle = (void*) stack; \
        p->object = NULL; \
        p->process = NULL; \
        p->system  = NULL; \
        p->service = 0; \
    }

#define HANDLE_MOD_EXIT(p) \
    if(p!=NULL){ \
        if(p->szdir!=NULL) \
            free(p->szdir), p->szdir=NULL; \
        if(p->name!=NULL) \
            free(p->name), p->name=NULL; \
        if(p->fdev!=NULL) \
            free(p->fdev), p->fdev=NULL; \
        free(p); \
        p=NULL; \
    }
/* ======================================================================= 







=========================================================================*/

#include "devctl10.h"

#define FDEV_BITS   64

#define CORE_BITS   2048

#define CRMC_ENABLE  1
#define CRMC_PROCESS 2
#define CRMC_FINAL   3

typedef int (*librmc_callback_threads)(void *addr, unsigned int cnt, unsigned int pyt);
typedef int (*librmc_controls_handles)(xmod_ctrl_params);

struct cargv_stack_t{
    char         *buffer;
    unsigned int length;
    char         *code;
    char         *core;

    void *       object;
    void *       handle;
    void *       stack;
};
struct librmc_time_t
{
    uint32_t       min;
    uint32_t       sec;
};

struct librmc_state_t 
{
    unsigned int vstate[CORE_BITS];
};

struct librmc_addr_t
{
    char                   *fdev;
    unsigned char          *data;

    char                    *code;
    
    unsigned int            num;
    unsigned short          hex;
    
    unsigned int            status;
    unsigned int            brain;

    char                    *core;


    struct librmc_time_t    *first;
    struct librmc_time_t    *last;

    struct librmc_addr_t   *next;
};

struct librmc_stack_t
{
    char *name;
    char *error_s;
    unsigned int    sector;
    unsigned int    count;
    struct librmc_addr_t  *addr;

    struct librmc_state_t *state;

    struct librmc_time_t  *core;

    librmc_callback_threads  thread;

    librmc_controls_handles  handle;
};

extern
struct librmc_stack_t *sptr_core;

int librmc_init(struct mod_stack_t *ptr,struct sysnode_proc_t *str);
int librmc_proc(struct mod_stack_t *ptr,librmc_callback_threads thread);
int librmc_exit(struct mod_stack_t *ptr,struct sysnode_proc_t *str);
struct librmc_stack_t * librmc_free(struct librmc_stack_t *ptr);
struct librmc_stack_t * librmc_sysd(void);

int librmc_update(char *dev_name, unsigned char *data, unsigned int number, struct sysnode_proc_t *str);
unsigned short librmc_find(unsigned short code, unsigned short rand_s);

struct librmc_addr_t  *librmc_create(char *buffer, unsigned int length);
struct librmc_addr_t  *librmc_randstr(void);
struct librmc_addr_t  *librmc_select(char *code, char *data);
struct librmc_addr_t  *librmc_final(char *code);

extern
unsigned int rmc_run_state;
int librmc_runstate(void);
int librmc_poweroff(void);

extern
unsigned int rmc_count;
extern
unsigned int rmc_free;
unsigned int librmc_count();
unsigned int librmc_cfree();


struct dev_t{
    int fd;  
};
typedef struct dev_t xdev_t;

xdev_t *libdev_open(char *dev);
int libdev_read(xdev_t * dev, char *buffer, unsigned int length);
int libdev_write(xdev_t * dev, char *buffer, unsigned int length);
int libdev_ioctl(xdev_t * dev, unsigned int flags, unsigned int *value); 
xdev_t *libdev_close(xdev_t *dev);


struct cda_handle_t{
    int fd;  
};
typedef struct cda_handle_t xcda_handle_t;
struct cda_header_t{
    uint8_t  pe;
    uint8_t  xe;
    char code[16];
    char file[256];
    char text[256];
	
	time_t  ctime;
		
	uint32_t  length;
	uint32_t  len_old;
	
	uint32_t  wrap;
	
	uint32_t  percent;
	uint32_t  current;
	uint32_t  maximum;
	
	uint32_t  swap;
};
typedef struct cda_header_t xcda_header_t;
xcda_handle_t *libcda_open(char *dev);
int libcda_read(xcda_handle_t * dev, char *buffer, unsigned int length);
int libcda_write(xcda_handle_t * dev, char *buffer, unsigned int length);
int libcda_ioctl(xcda_handle_t * dev, unsigned int flags, unsigned int *value); 
xdev_t *libcda_close(xcda_handle_t *dev);
int libcda_setheader(xcda_handle_t *dev, xcda_header_t *str);
int libcda_getheader(xcda_handle_t *dev, xcda_header_t **str);


extern
struct sysnode_proc_t *psys; 


struct qtw_block_t {
    uint32_t id;
    uint32_t type;

    long     code;
    int      x,y,z;
};
typedef struct qtw_block_t zqtw_block_t;

zqtw_block_t *libqtw_block(uint32_t code);
zqtw_block_t *libqtw_set(zqtw_block_t *p, uint32_t code);
int libqtw_generator(uint32_t *code);
int libqtw_bits(void);

extern int libvbt_state;

struct libvbt_list_t
{
    char addr[18];
    char name[248];  
    char device[256];
    struct libvbt_list_t *next;  
};

struct libvbt_t 
{
    int s;
    int sock;
    struct sockaddr_rc address;

    char addr[18];
    char name[248];
    char device[256];

    inquiry_info *ii;
    
    int status;

    unsigned int count;
    struct libvbt_list_t *list; 
};


typedef int (*callback_handle_t)(char *buffer, unsigned int len);

int libvbt_info(struct libvbt_t *st);
int libvbt_list(struct libvbt_t *st);
int libvbt_init(struct libvbt_t *st);
int libvbt_select(struct libvbt_t *st,int fx);
int libvbt_write(struct libvbt_t *st, char *buffer, unsigned int len);
int libvbt_read(struct libvbt_t *st, char *buffer, unsigned int len);
int libvbt_callback(struct libvbt_t *st, callback_handle_t cb);
int libvbt_close(struct libvbt_t *st);
int libvbt_update(struct libvbt_t *st, unsigned int p);
int libvbt_test(struct libvbt_t *st, unsigned int p);
int libvbt_micp(struct libvbt_t *st, unsigned int list);


int libvbt_smcl(struct libvbt_t *st);

#endif

