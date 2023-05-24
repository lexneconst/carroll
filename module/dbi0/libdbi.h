#ifndef LIBCOR0_H
#define LIBCOR0_H

#include <libcrt.h>
#include <md5.h>

int d00_libmod_init(char *path_dev,  struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int d00_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int d00_libmod_update(char *buffer, unsigned int length); 
int d00_libmod_find(char *str);
int d00_libmod_xread(unsigned int id, cstring_t *str);
int d00_libmod_xwrite(unsigned int id, cstring_t *str);
int d00_libmod_xfree(cstring_t *str);

extern
int stack_core_state;

int libdbi_runstate(void);
int libdbi_poweroff(void);

int libdbi_create(struct mod_args_t *args);

#endif

