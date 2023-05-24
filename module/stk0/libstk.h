#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>


extern
uint32_t stkx_state;

extern
pthread_t threadid;

void thread_stk_procv(void*args);

int libstk_init(void);
int libstk_runstate(void);
int libstk_poweroff(void);

int s10_libmod_init(char *path_dev,struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int s10_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int s10_libmod_update(char *buffer, unsigned int length); 
int s10_libmod_find(char *str);
int s10_libmod_xread(unsigned int id, cstring_t *str);
int s10_libmod_xwrite(unsigned int id, cstring_t *str);
int s10_libmod_xfree(cstring_t *str);

int s10_libmod_append(char *code, char *buffer, unsigned int length);

int s10_libmod_xset(char *code, char *buffer, unsigned int length);
int s10_libmod_search(char *code);
struct cargv_stack_t  *s10_libmod_select(struct cargv_stack_t  *ptr);

#endif

