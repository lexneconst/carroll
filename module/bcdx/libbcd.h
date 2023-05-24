#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>


extern
uint32_t bcdx_state;

void thread_bcdx_procv(void*args);

int bcd_libmod_init(char *path_dev,  struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int bcd_libmod_exit(struct mod_struct_t* ext_ptr, struct sysnode_proc_t *str);
int bcd_libmod_update(char *buffer, unsigned int length); 
int bcd_libmod_find(char *str);
int bcd_libmod_xread(unsigned int id, cstring_t *str);
int bcd_libmod_xwrite(unsigned int id, cstring_t *str);
int bcd_libmod_xfree(cstring_t *str);

int bcd_libmod_append(char *code, char *buffer, unsigned int length);

int bcd_libmod_xset(char *code, char *buffer, unsigned int length);
int bcd_libmod_search(char *code);
struct cargv_stack_t  *bcd_libmod_select(struct cargv_stack_t  *ptr);

#endif

