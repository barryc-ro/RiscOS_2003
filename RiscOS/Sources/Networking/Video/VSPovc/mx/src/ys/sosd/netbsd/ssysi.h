/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    ssysi.h
  DESCRIPTION
    System-specific, not callable from generic code hooks to sys.
    
  PUBLIC FUNCTIONS

    ssysAddFd -- import FD to wake up sysiPause from select/poll on
		 readable or connectable input.

    ssysAddWrFd -- import FD to wakt up sysiPause from select/poll on
		writable.  Should only be installed when the FD would
		block, and be removed after it is writable.

    ssysRemFd -- remove FD from those known to sysiPause.

    ssysGetFds -- Export FDs known to sysiPause for use by some other
		  pause/select/poll mechanism.  Note that using this
		  has complications with timers and interrupts!

  NOTES
    <x>
  MODIFIED   (MM/DD/YY)
    dbrower   10/ 3/96 -  created.
*/

#ifndef SSYSI_ORACLE
#define SSYSI_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

/* PUBLIC TYPES AND CONSTANTS */

typedef ub4 ssysFdFlags;
#define SSYS_FD_INPUT	0x1
#define SSYS_FD_OUTPUT	0x2

typedef struct
{
  int		fd;
  ssysFdFlags	op;
} ssysRegFd;

/* PUBLIC FUNCTIONS */

/*
 * ssysAddFd, ssysRemFd - add and remove a file descriptor
 *
 * DESCRIPTION
 * These routines, private to the OSD layer, are used by the network
 * drivers to register their file descriptors so that sysiPause() may
 * appropriately wait on I/O for any active file descriptor.
 *
 * Input activity includes a pending connection if the socket has
 * a "listen" outstanding.
 *
 * Output activity means the FD can accept output without blocking.
 * FDs should not be left registered for output for long periods of time,
 * as this is likely to cause process spinning
 *
 * ssysRemFd must be called with the same op set given when it was added,
 * along with the value that was returned.
 */
int  ssysAddFd(dvoid *osdp, int fd, ssysFdFlags op );
void ssysRemFd(dvoid *osdp, int slot, ssysFdFlags op );

/* ---------------------------- ssysGetFds ---------------------------- */
/*
  NAME
    ssysGetFds
  DESCRIPTION
    Return the number of, and an array containing, the file
    descriptors currently known and used by ys to poll against
    for activity; activity on these fds should stop any
    process blocking activity.
    
    The caller is responsible for calling ysmGlbFree() against the
    returned array to release the memory.

  PARAMETERS

    nfds    -- a pointer to an sword that will contain the length
	       of the allocated array of file descriptors. [OUT]

    fds    -- a pointer to a pointer to ssysRegFd structures, with each
	      entry defining the state of a registerd file descriptor.
	      An array will be allocated and the values set; the caller
	      must release the memory with a call to ysmGlbFree. [OUT]
  RETURNS
    none.
*/
void ssysGetFds( ub4 *nfds, ssysRegFd **fds );

#endif



