/* Simple stub kernel.h header file required by defmod builds */

#ifndef kernel_H
#define kernel_H

typedef struct {
  int errnum;
  char errmess[252];
} _kernel_oserror;

#define __swi(n)

extern _kernel_oserror *_kernel_last_oserror(void);
extern _kernel_oserror *_kernel_set_perror(void);

# endif

