/* remote.c */

/* "Remote control" of Fresco from a test harness
 * (C) ANT Limited 1997  All Rights Reserved
 */

BOOL RemoteControl_Open( void );

BOOL RemoteControl_Active( void );

void RemoteControl_Send( const char *string );

void RemoteControl_Poll( void );

void RemoteControl_Close( void );

#define remotecontrol_HOST "193.35.146.12"      /* gi */
#define remotecontrol_PORT 8880 + 2
