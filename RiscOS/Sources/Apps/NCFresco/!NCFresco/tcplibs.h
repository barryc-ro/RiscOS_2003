
#ifdef STBWEB_BUILD

#include "sys/types.h"
#include "sys/socket.h"
#include "errno.h"

#include "net/if.h"
#include "netinet/in.h"
#include "sys/ioctl.h"
#include "netdb.h"
#include "netns/ns.h"
#include "arpa/inet.h"
#include "socklib.h"

#else

#include <errno.h>
#include "../include/socket.h"
#include "../include/netdb.h"
#include "../include/in.h"
#include "../include/ioctl.h"
#include "../include/sock_errno.h"

#endif
