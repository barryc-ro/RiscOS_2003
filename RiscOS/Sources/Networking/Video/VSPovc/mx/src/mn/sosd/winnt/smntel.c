/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * smntel.c - OMN Telnet Tunneling Interface
 *
 * DESCRIPTION
 * smnTelnet() implements a very primitive TELNET protocol.  This allows
 * Media Net to be run over TCP tunnelled through an authenticated TELNET
 * connection.  It operates in an interactive mode, using stdin and stdout
 * for keyboard input and echo output, until it recognizes that it has final
 * connected to a Media Net address server on the other side.  This is
 * indicated by the receipt of the TELNET-compliant string
 * "IAC WILL Media Net".
 *
 * It also understands the basic TELNET options: ECHO and SUPPRESS GO AHEAD.
 * It encourages the other side to do both of these things.
 * The Telnet NIO is a Media Net TCP/IP NIO (see smntcp.c) that is
 * run through an authenticated channel.
 */

#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef SMNTCP_ORACLE
#include <smntcp.h>
#endif

#include <ysl.h>

static boolean ssmnIac(int fd, int td);

/*
 * smnTelnet - the TELNET engine
 */
boolean smnTelnet(CONST char *proxypa, int fd)
{
  fd_set fds;
  int kb, sts, td;
  char ch;
  boolean done;

  /* initialize our file descriptor set for the SELECT */
  FD_ZERO(&fds);
  kb = fileno(stdin);                            /* keyboard file descriptor */
  td = fileno(stdout);                           /* terminal file descriptor */
  done = FALSE;

  /* things we want the other side to do
   * IAC DO ECHO, IAC DO SUPPRESS GO AHEAD
   */
  send(fd, "\377\375\001\377\375\003", 6, 0);

  /* now loop, shuffling bytes back and forth, until we are done or we
   * get an error
   */
  while (!done)
    {
      /* select for more I/O */
      FD_SET(fd, &fds);
      FD_SET(kb, &fds);

      if (select(fd + 1, &fds, 0, 0, 0) == -1)
	{
	  yslError("unexpected error during telnet select; errno=%d", errno);
	  return FALSE;
	}

      /* is there keyboard input */
      if (FD_ISSET(kb, &fds))
	{
	  if (recv(kb, &ch, 1, 0) <= 0)
	    {
	      yslError("unexpected error or end of file on keyboard read;"
		       " errno=%d", errno);
	      return FALSE;
	    }
	  else
	    {
	      if (ch == '\n')
		sts = send(fd, "\015\012", 2, 0);         /* special for CR LF */
	      else
		sts = send(fd, &ch, 1, 0);               /* send the character */

	      if (sts < 0)
		{
		  yslError("unexpected error on telnet write; error=%d",
			   errno);
		  return FALSE;
		}
	    }
	}

      /* is there socket input */
      if (FD_ISSET(fd, &fds))
	{
	  if (recv(fd, &ch, 1, 0) <= 0)
	    {
	      yslError("unexpected error or end of file on telnet read;"
		       " errno=%d", errno);
	      return FALSE;
	    }
	  else if (ch == '\015')         /* special work for carriage return */
	    {
	      recv(fd, &ch, 1, 0);
	      if (ch == '\012')
		send(td, "\n", 1, 0);
	      else
		send(td, "\015", 1, 0);
	    }
	  else if (ch == '\377')
	    done = ssmnIac(fd, td);
	  else
	    send(td, &ch, 1, 0);                         /* echo the character */
	}
    }

  return TRUE;
}

/*
 * ssmnIac - interpret the TELNET IAC options
 */
static boolean ssmnIac(int fd, int td)
{
  char ch, optcode, buf[3];
  boolean done;

  /* IAC recognized; now process command */
  done = FALSE;
  ch = '\377';
  recv(fd, &ch, 1, 0);

  switch (ch)
    {
    case '\377':  /* passthrough of 255 */
      send(td, "\377", 1, 0);
      break;
    case '\373':  /* WILL */
      /* we recognize but don't reply to TRANSMIT-BINARY & ECHO;
       * we recognize and reply affirmatively to MEDIA NET;
       * we recognize and reply negatively to anything else
       */
      recv(fd, &optcode, 1, 0);
      switch (optcode)
	{
	case 1: /* echo */
	case 3: /* suppress go ahead */
	  break;
	case '\144': /* will MEDIA NET */
	  send(fd, "\377\375\144", 3, 0);
	  done = TRUE;
	  break;
	default:
	  buf[0] = '\377', buf[1] = '\376', buf[2] = optcode;
	  send(fd, buf, 3, 0);
	  break;
	}
      break;
    case '\374':  /* WONT */
      /* we recognize and insist that the other side ECHO & SUPPRESS GO AHEAD;
       * we don't care about anything else
       */
      recv(fd, &optcode, 1, 0);
      switch (optcode)
	{
	case 1: /* echo */
	  send(fd, "\377\375\001", 3, 0);
	  break;
	case 3: /* suppress go ahead */
	  send(fd, "\377\375\003", 3, 0);
	  break;
	default:
	  break;
	}
      break;
    case '\375':  /* DO */
      /* we recognize and deny that we will do anything besides SUPPRESS
       * GO AHEAD; we recognize SUPPRESS GO AHEAD and say nothing
       */
      recv(fd, &optcode, 1, 0);
      switch (optcode)
	{
	case 3: /* suppress go ahead */
	  break;
	default:
	  buf[0] = '\377', buf[1] = '\374', buf[2] = optcode;
	  send(fd, buf, 3, 0);
	  break;
	}
      break;
    case '\376':  /* DONT */
      /* we already don't do anything.  so we agree with whatever we
       * are told.
       */
      recv(fd, &optcode, 1, 0);
      break;
    default:
      yslError("IAC code %d not recognized", ch);
      break;
    }

  return done;
}
