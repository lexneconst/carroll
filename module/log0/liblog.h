#ifndef LIBLOG0_H
#define LIBLOG0_H

#include <libcrt.h>

int l00_libmod_init(char *path_dev, struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int l00_libmod_exit(struct mod_struct_t *ext_ptr, struct sysnode_proc_t *str);
int l00_libmod_update(char *buffer, unsigned int length); 
int l00_libmod_find(char *str);
int l00_libmod_xread(unsigned int id, cstring_t *str);
int l00_libmod_xwrite(unsigned int id, cstring_t *str);
int l00_libmod_xfree(cstring_t *str);


#endif

