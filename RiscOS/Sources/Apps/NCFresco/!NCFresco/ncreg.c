/* > ncreg.c

 * NCRegistry support routines
 * SmartCard and config related stuff

 */


#include "swis.h"

#include "wimp.h"

#include "debug.h"
#include "memwatch.h"

#include "interface.h"
#include "stbfe.h"

#ifndef CHECK_IF_WITH_REGISTRY
#define CHECK_IF_WITH_REGISTRY 0
#endif

/* ------------------------------------------------------------------------------------------- */

/* ncregistry defines */

#define NCRegistry_EnumerateNetworkDrivers  0x4d380
#define NCRegistry_Enquiry                  0x4d382
#define NCRegistry_Write                    0x4d385

typedef struct dib
{
   unsigned int         dib_swibase;    /* Module's SWI chunk number */
   char                *dib_name;       /* Eg "ea" and "slip" */
   unsigned int         dib_unit;       /* Unit number. First is 0, then consecutive */
   unsigned char       *dib_address;    /* 6 bytes of hardware address */
   char                *dib_module;     /* Eg "Ether3" */
   char                *dib_location;   /* Eg "Network expansion slot" */
/*   struct slot          dib_slot;        See above defintion */
} dci4_dib;

typedef struct mydiblist {

  unsigned int flags;     /* flag bits - see below */
  dci4_dib *dib_ptr;      /* pointer to a DCI4 compliant driver information block */
  struct mydiblist *next; /* next entry, or NULL for end of list */
} mydci4_diblist;

#define DIB_FLAG_PRIMARY 0x1   /* this is the primary interface as far as the registry is concerned */
#define DIB_FLAG_STATS   0x2   /* device driver supports statistics */
#define DIB_FLAG_STATUS  0x4   /* interface is working OK */
#define DIB_FLAG_UP      0x8   /* interface is UP */
#define DIB_FLAG_PTP     0x10  /* interface is point2point */
#define DIB_FLAG_SCOK    0x20
#define DIB_FLAG_IGNORE  0x40

/* ------------------------------------------------------------------------------------------- */

#if CHECK_IF_WITH_REGISTRY

/*
 * Check with the registry to see if the interface is down.
 * Returns true if
  - primary interface is down
 */

BOOL ncreg_interface_is_down(void)
{
    mydci4_diblist *dib;
    _kernel_oserror *e;

    /* if no registry then assume interface is OK */
    if ((e = _swix(NCRegistry_EnumerateNetworkDrivers, _OUT(0), &dib)) != NULL)
    {
	STBDBG(("interface_is_down: registry error %x %s\n", e->errnum, e->errmess));
        return FALSE;
    }

    for (; dib; dib = dib->next)
    {
	STBDBG(("interface_is_down: if flags=%x\n", dib->flags));

	/* only concerned with the primary (ptp currently) interface  */
        if (dib->flags & DIB_FLAG_PTP)
        {
	    STBDBG(("interface_is_down: PTP if, flags %x, name='%s'\n",
		    dib->flags, dib->dib_ptr ? dib->dib_ptr->dib_name : "<none>"));

            /* if ppp and status OK and down */
            if (dib->dib_ptr && strcasecomp(dib->dib_ptr->dib_name, "ppp") == 0 &&
                (dib->flags & DIB_FLAG_STATUS) &&
                (dib->flags & DIB_FLAG_UP) == 0)
            {
                return TRUE;
            }

            break;
        }
    }

    STBDBG(("interface_is_down: no matching dib\n"));
    return FALSE;
}

#endif

/* ------------------------------------------------------------------------------------------- */

/* extra special hidden function... */

void ncreg_decode(void)
{
    void *buf = mm_malloc(40*1024);
    int error;

    STBDBG(( "registry decode\n"));

    _swix(NCRegistry_Write, _INR(0,2) | _OUT(0), "NCD_INFO", buf, 40*1024, &error);

    if (error == -2)
        frontend_open_url(buf, main_view, NULL, NULL, fe_open_url_NO_REFERER);

    mm_free(buf);
}

/* ------------------------------------------------------------------------------------------- */

char *ncreg_enquiry(const char *tag)
{
    _kernel_oserror *e;
    int size;
    char *ptr = NULL;

    e = _swix(NCRegistry_Enquiry, _INR(0,2) | _OUT(0), tag, NULL, 0, &size);
    if (!e && size && (ptr = mm_malloc(size)) != NULL)
	e = _swix(NCRegistry_Enquiry, _INR(0,2), tag, ptr, size);

    return e ? e->errmess : ptr;
}

/* ------------------------------------------------------------------------------------------- */

void ncreg_init(void)
{

}

/* ------------------------------------------------------------------------------------------- */

/* eof ncreg.c */
