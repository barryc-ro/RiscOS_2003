/* splash.c */

#include <stdlib.h>

#include "os.h"
#include "bbc.h"
#include "wimp.h"
#include "alarm.h"

#include "debug.h"

#include "splash.h"

wimp_w splash_window = -1;

#define SPLASH_NAME "ncf_splash"

static wimp_wind sw =
    { 0,0,0,0,
      0,0,
      -1,
      (wimp_wflags)0x80000000,
      {0,0,0,255,0,0,0,0},
      0,-1200,1600,0,
      (wimp_iconflags)0,
      (wimp_iconflags)0,
      (void*)1,
      0,
      "",
      0
    };

static void splashalarm(int at, void *h)
{
    STBDBG(("splashalarm\n"));
    if ( splash_window != -1 )
    {
        wimp_delete_wind( splash_window );
        splash_window = -1;
    }
}

void splash(void)
{
    os_regset ors;
    wimp_openstr wos;
    int w,h,x,y;
    os_error *ep;
    char pixtrans[1024];

    ors.r[0] = 40;  /* read sprite info */
    ors.r[2] = (int)SPLASH_NAME;

    if ( wimp_spriteop_full( &ors ) )
    {
        STBDBG(("splash: wimp_spriteop_full reports error\n"));
        return;
    }

    w = ors.r[3] << bbc_modevar(ors.r[6], bbc_XEigFactor);
    h = ors.r[4] << bbc_modevar(ors.r[6], bbc_YEigFactor);

    x = (bbc_modevar(-1,bbc_XWindLimit)+1) << bbc_modevar(-1,bbc_XEigFactor);
    y = (bbc_modevar(-1,bbc_YWindLimit)+1) << bbc_modevar(-1,bbc_YEigFactor);

    x = (x-w)/2;
    y = (y-h)/2;

    ors.r[0] = (int)wimp_baseofsprites(); /* lets hope this returns ram area */
    ors.r[1] = (int)SPLASH_NAME;
    ors.r[2] = -1;
    ors.r[3] = -1;
    ors.r[4] = (int)pixtrans;
    ors.r[5] = 16;  /* return full entries */

    ep = os_swix( 0x60740, &ors );  /* XColourTrans_SelectTable */

    if ( ep )
        STBDBG(("splash: colourtrans gave %s\n", ep->errmess ));

    ors.r[0] = 52;
    ors.r[2] = (int)SPLASH_NAME;
    ors.r[3] = x;
    ors.r[4] = y;
    ors.r[5] = 0x30;
    ors.r[6] = 0;
    ors.r[7] = (int)pixtrans;

    STBDBG(("splash: plotting at (%d,%d)\n", x, y));

    ep = wimp_spriteop_full(&ors);

    if ( ep )
        STBDBG(("splash: plot gave %s\n", ep->errmess ));

    sw.box.x0 = x;
    sw.box.y0 = y;
    sw.box.x1 = x+w;
    sw.box.y1 = y+h;
    ep = wimp_create_wind( &sw, &splash_window );

    if ( ep )
        STBDBG(("splash: createwind gave %s\n", ep->errmess ));

    wos.w = splash_window;
    wos.box = sw.box;
    wos.x = 0;
    wos.y = 0;
    wos.behind = -1;
    ep = wimp_open_wind( &wos );

    if ( ep )
        STBDBG(("splash: openwind gave %s\n", ep->errmess ));

    alarm_set( alarm_timenow()+800, &splashalarm, NULL );
}

/* eof */
