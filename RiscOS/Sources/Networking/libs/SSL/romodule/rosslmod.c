
#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"


extern _kernel_oserror *mod_init(char *cmdtail, int podbase, void *pw)
{
	printf("RO SSL Module initialise\n");

	return NULL;
}

extern void mod_service(int scnum, _kernel_swi_regs *rp, void *pw)
{

	return;
	
}


extern _kernel_oserror *mod_swi(int swinum, _kernel_swi_regs *rp, void *pw)
{
    extern _kernel_oserror no_such_swi;
    extern _kernel_oserror *patch_user_table(int base, int end);
    _kernel_oserror *ep;

    switch (swinum)
    {
    case 0:
	ep = patch_user_table(rp->r[0], rp->r[1]);
    if (0)
    {
	char buf[100];
	sprintf(buf, "memoryi %x %x\n", rp->r[0], rp->r[1]);
	_kernel_oscli(buf);
    }
	rp->r[0] = (int)ep;
	break;

    default:
	ep = &no_such_swi;
	break;
    }

	return ep;
}

