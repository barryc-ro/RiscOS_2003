/* Copyright (c) 1996 by Oracle Corporation. All Rights Reserved.
 *
 * srvstart.c - win32 service start program
 *
 * MODIFIED
 * syen    02/20/96 - Creation.
 */

#include <windows.h>
#include <stdio.h>

static void ErrorHandler(char *s, int err)
{
  char errmsg[128];
  
  if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0x0409,
                     errmsg, sizeof(errmsg), NULL))
    sprintf(errmsg, "GetLastError() = %d", err);
  printf("%s", errmsg);
  exit (err);
}

void main(int argc, char **argv)
{
  SC_HANDLE service, scm;
  int exitcode;

  if (argc < 2)
  {
    printf("usage: svcstart service_name [arguments]\n");
    printf("         the arguments are passed as startup arguments to the\
 service\n");
    printf("\n                or\n");
    printf("usage: svcstart -e service_name\n");
    printf("         svcstart exit code is set to 1 if service_name\
 exists\n");
    printf("         svcstart exit code is set to 0 if service_name does\
 not exist\n");
    return;
  }

  scm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS | GENERIC_WRITE);
  if (!scm)
    ErrorHandler("In OpenScManager", GetLastError());

  if (!strcmp(argv[1],"-e"))
  { /* user wants us to check for existance of this service */

    /* try to open service */
    service = OpenService(scm, argv[2], SERVICE_ALL_ACCESS);
                           
    if (service)
      exitcode = 1; /* set exitcode to show service exists */
    else
      exitcode = 0; /* set exitcode to show service does not exist */

    CloseServiceHandle(service);
    CloseServiceHandle(scm);

  }
  else
  {
    /* in this section the user wants us to start the service */

    /* open the service */
    service = OpenService(scm, argv[1], SERVICE_ALL_ACCESS);
                             
    if (!service)
      ErrorHandler("In OpenService", GetLastError());
  
    /* start the service, passing it startup parameters */
    if (StartService(service, argc-2, &argv[2]))
      printf("Service started\n");
    else
      ErrorHandler("In StartService", GetLastError());
  
    /* clean up */
cleanup:
    CloseServiceHandle(service);
    CloseServiceHandle(scm);

    exitcode = 0;

  }

  exit(exitcode);

}
