/*
 * Title:   alarm.c
 * Purpose: alarm facilities for wimp programs, using non-busy waiting
 *          for idle events
 * Author:  IDJ
 * Status:  under development
 * History:   20-Mar-89 IDJ created
 *            21-Mar-89 IDJ modified kludge to alarm_timedifference
 *            21-Mar-89 IDJ added alarm__laterthan macro
 *            08-May-91 ECN #ifndefed out unused ROM functions
 *            09-May-91 ECN use swi names instead of nos.
 *            14-Jun-91 IDJ timedifference put back in
 */

#include <stdlib.h>
#include <limits.h>
#include "alarm.h"
#include "werr.h"
#include "os.h"
#include "msgs.h"
#include "swis.h"

#include "memwatch.h"

#define alarm__laterthan(t1,t2) (t1 > t2)


typedef struct alarm__str
        {
           struct alarm__str *next;
           int at;
           alarm_handler proc;
           void *handle;
        } ALARM;

/* the list of pending alarms */
static ALARM *alarm__pending_list = 0;

int ncalarm_timenow(void)
{
  os_regset r;
  os_swix(OS_ReadMonotonicTime, &r);
  return (r.r[0]);
}

void ncalarm_set(int at, alarm_handler proc, void *handle)
{
  ALARM *p, *save_p, *new_p;

  p = alarm__pending_list;
  save_p = p;

  /* find where to put the alarm (in time) */
  while (p!=0 && alarm__laterthan(at, p->at))
  {
    save_p = p;
    p = p->next;
  }

  /* insert new pending alarm */
  new_p = mm_malloc(sizeof(ALARM));
  new_p->next = p;
  new_p->proc = proc;
  new_p->handle = handle;
  new_p->at = at;
  if(save_p == p)
    alarm__pending_list = new_p;
  else
    save_p->next = new_p;

/*   STBDBG(("alarm: set  %d: %x %p\n", at, proc, handle)); */
}



BOOL ncalarm_next(int *result)
{
  if (alarm__pending_list != 0)
  {
    /* if there's a pending alarm say when it's for */
    *result = alarm__pending_list->at;
    return TRUE;
  }
  else
    return FALSE;
}



void ncalarm_callnext(void)
{
  ALARM *next_alarm;
  alarm_handler proc_to_call;
  int called_at;
  void *handle_to_pass;

  if (alarm__pending_list != 0)
  {
    /* save details of next alarm */
    proc_to_call = alarm__pending_list->proc;
    called_at = alarm__pending_list->at;
    handle_to_pass = alarm__pending_list->handle;

/*     STBDBG(("alarm: call %d: %x %p\n", called_at, proc_to_call, handle_to_pass)); */

    /* farewell and adieu to the next pending alarm */
    next_alarm = alarm__pending_list;
    alarm__pending_list = alarm__pending_list->next;
    mm_free(next_alarm);

    /* call supplied routine last (in case it goes bang!) */
    proc_to_call(called_at, handle_to_pass);
  }
}

void ncalarm_removeall(void *handle)
{
  ALARM *p, *save_p;

  p = alarm__pending_list;
  save_p = p;

  while(p != 0)
    if(p->handle == handle)
    {
      if(save_p == p)
      {
        alarm__pending_list = p->next;
        p = alarm__pending_list;
        mm_free(save_p);
        save_p = p;
      }
      else
      {
        save_p->next = p->next;
        mm_free(p);
        p = save_p->next;
      }
    }
    else
    {
      save_p = p;
      p = p->next;
    }
}

BOOL ncalarm_anypending(void *handle)
{
  ALARM *p = alarm__pending_list;

  while(p != 0)
  {
    if(p->handle == handle) return TRUE;
    p = p->next;
  }

  return FALSE;
}

/* eof alarm.c */
