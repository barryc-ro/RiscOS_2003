/* ssldesp.c - Despatch incoming SWI calls onto actual SSL routines. */
/* (C) ANT Limited 1995. All rights reserved. */


#include "kernel.h"


extern void service_call_handler(register int sn, _kernel_swi_regs *r)
{
 

}


/*
 * Entry:
 * r0	call number
 * r1	1st parameter
 * r2	2nd parameter
 * ..
 * r8	8th parameter

extern _kernel_oserror *ssl_swi_despatch(uint swinum, _kernel_swi_regs *regs, void *pw)
{
    _kernel_oserror *ep;
    
    switch (swinum)
    {
    
        default:
            
    }

}