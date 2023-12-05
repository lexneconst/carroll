#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>

int d30_libmod_init(char *path_dev,  struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int d30_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int d30_libmod_update(char *buffer, unsigned int length); 
int d30_libmod_find(char *str);
int d30_libmod_xread(unsigned int id, cstring_t *str);
int d30_libmod_xwrite(unsigned int id, cstring_t *str);
int d30_libmod_xfree(cstring_t *str);

extern
int stack_core_state;

int libdbm_runstate(void);
int libdbm_poweroff(void);

int libdbm_create(struct mod_args_t *args);

#endif

