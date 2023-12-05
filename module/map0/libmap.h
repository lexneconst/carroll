#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>


extern
uint32_t mapx_state;

extern
pthread_t threadid;

void thread_map_procv(void*args);

int libmap_init(void);
int libmap_runstate(void);
int libmap_poweroff(void);

int m10_libmod_init(char *path_dev,struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int m10_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int m10_libmod_update(char *buffer, unsigned int length); 
int m10_libmod_find(char *str);
int m10_libmod_xread(unsigned int id, cstring_t *str);
int m10_libmod_xwrite(unsigned int id, cstring_t *str);
int m10_libmod_xfree(cstring_t *str);

int m10_libmod_append(char *code, char *buffer, unsigned int length);

int m10_libmod_xset(char *code, char *buffer, unsigned int length);
int m10_libmod_search(char *code);
struct cargv_stack_t  *m10_libmod_select(struct cargv_stack_t  *ptr);

#endif

