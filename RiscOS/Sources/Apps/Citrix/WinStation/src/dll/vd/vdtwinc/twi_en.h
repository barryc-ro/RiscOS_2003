#if defined(WIN32) && !defined(WINCE)

#ifndef TWI_INTERFACE_ENABLED
#define TWI_INTERFACE_ENABLED 1
#endif

#else

#ifdef TWI_INTERFACE_ENABLED
#undef TWI_INTERFACE_ENABLED
#endif

#endif
