/* -*-c-*- */

#ifndef __stdlib_h
#include <stdlib.h>
#endif

#ifndef __kernel_h
#include "kernel.h"
#endif

#ifndef __setjmp_h
#include "setjmp.h"
#endif

typedef struct {
    _kernel_stack_chunk sc;
    int stck_reserve[7];
} my_stack_chunk;

typedef int(*main_function)(int argc, char **argv);

typedef enum {
    thread_ALIVE = 1,
    thread_DEAD = 2
    } thread_status;

typedef struct {
    volatile thread_status status;
    volatile int rc;
    volatile my_stack_chunk *stack;
    jmp_buf exec_point;
    volatile char *halt_point;
} thread_struct;

typedef thread_struct *thread;

typedef void(*atexit_fn)(void);


extern void thread_exit(int rc);
extern void thread_starter(void);
extern thread thread_start(main_function fn, int argc, char **argv, int stack_size);
extern thread thread_run(thread t);
extern void thread_destroy(thread t);
extern void thread_wait(char *);

