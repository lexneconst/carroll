#ifndef LIBRMC_H
#define LIBRMC_H

#include <libcrt.h>
#include <md5.h>

extern
pthread_t threadid;


extern
struct librmc_stack_t *sptr_core;

extern
unsigned int rmc_run_state;

void thread_proc_xsd(void*args);

#endif
