#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>


extern
uint32_t thix_state;

extern
pthread_t threadid;

void thread_thi_procv(void*args);

int libthi_init(void);
int libthi_runstate(void);
int libthi_poweroff(void);

int t00_libmod_init(char *path_dev,struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int t00_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int t00_libmod_update(char *buffer, unsigned int length); 
int t00_libmod_find(char *str);
int t00_libmod_xread(unsigned int id, cstring_t *str);
int t00_libmod_xwrite(unsigned int id, cstring_t *str);
int t00_libmod_xfree(cstring_t *str);

int t00_libmod_append(char *code, char *buffer, unsigned int length);

int t00_libmod_xset(char *code, char *buffer, unsigned int length);
int t00_libmod_search(char *code);
struct cargv_stack_t  *t00_libmod_select(struct cargv_stack_t  *ptr);

#endif

