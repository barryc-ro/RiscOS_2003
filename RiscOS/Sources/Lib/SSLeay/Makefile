#
# SSLLeay/Makefile
#
# Top-level Makefile for RISC OS port of SSLeay.
#
# Author: SNB   30/03/98
#

AR=libfile
CP=copy
RM=remove
MKDIR=cdir
WIPE=-wipe

LIBDIR=<Lib$Dir>.SSLeay

CFLAGS=-throwback -Wpand -I${INCLUDES} ${DEFINES} ${EFLAGS}
CPFLAGS=FR~C~V~S~N~T~L
WPFLAGS=FR~C~V

# The following four macros are passed through to all sub-makefiles.  Therefore you can
# switch all the options just by changing them here, cleaning everything, and rebuilding.

INCLUDES = C:SSLeay,C:TCPIPLibs,C:
DEFINES  = -DRISCOS -DMFUNC -DFLAT_INC -DNO_DES -DNO_IDEA -DNO_MD2 -DNO_DSA ${DFLAGS}
EFLAGS   = -zM -zps1 -Wpand
CC       = cc



export_hdrs:
       ${MKDIR} ${LIBDIR}
       ${MKDIR} ${LIBDIR}.h
       Set Alias$SSLExportHeader ${CP} crypto.%0.h.%1 ${LIBDIR}.h.%1 ${CPFLAGS}
       SSLExportHeader asn1 asn1
       SSLExportHeader asn1 asn1_mac
       SSLExportHeader bio bio
       SSLExportHeader bf blowfish
       SSLExportHeader bn bn
       SSLExportHeader buffer buffer
       SSLExportHeader cast cast
       SSLExportHeader conf conf
       SSLExportHeader h.^ cryptall
       SSLExportHeader h.^ cryptlib
       SSLExportHeader h.^ crypto
       SSLExportHeader des des
       SSLExportHeader dh dh
       SSLExportHeader dsa dsa
       SSLExportHeader err err
       SSLExportHeader evp evp
       SSLExportHeader hmac hmac
       SSLExportHeader idea idea
       SSLExportHeader lhash lhash
       SSLExportHeader md md2
       SSLExportHeader md md5
       SSLExportHeader mdc2 mdc2
       SSLExportHeader objects objects
       SSLExportHeader pem pem
       SSLExportHeader pkcs7 pkcs7
       SSLExportHeader rand rand
       SSLExportHeader rc2 rc2
       SSLExportHeader rc4 rc4
       SSLExportHeader rc5 rc5
       SSLExportHeader ripemd ripemd
       SSLExportHeader rsa rsa
       SSLExportHeader sha sha
       SSLExportHeader ^.ssl ssl
       SSLExportHeader ^.ssl ssl2
       SSLExportHeader ^.ssl ssl23
       SSLExportHeader ^.ssl ssl3
       SSLExportHeader ^.ssl tls1
       SSLExportHeader stack stack
       SSLExportHeader txt_db txt_db
       SSLExportHeader x509 x509
       SSLExportHeader x509 x509_vfy
       SSLExportHeader h.^.^ e_os
       SSLExportHeader h.^.^ ro_options
       @Unset Alias$SSLExportHeader
       @Echo SSLeay headers exported to <Lib$Dir>.SSLeay
       @|

var:
       Set CryptBase$Dir <Obey$Dir>.crypto

crypto: var
       ${MKDIR} ${LIBDIR}
       ${MKDIR} ${LIBDIR}.o
       Dir <CryptBase$Dir>
       Obey !mkall "CC=${CC}" "INCLUDES=${INCLUDES}" "DEFINES=${DEFINES}" "EFLAGS=${EFLAGS}"

ssl:   var
       Dir <CryptBase$Dir>.^.SSL
       Obey !MkExport "CC=${CC}" "INCLUDES=${INCLUDES}" "DEFINES=${DEFINES}" "EFLAGS=${EFLAGS}"

export_libs: ssl crypto
	@echo SSLeay: exported libs
       
export: export_libs export_hdrs
       @Echo SSLeay: export complete

clean:
       Set CryptBase$Dir <Obey$Dir>.crypto
       ${WIPE} ${LIBDIR} ${WPFLAGS}
       Obey crypto.!mkclean
       Obey <CryptBase$Dir>.^.ssl.!MkClean
       @echo SSLeay: cleaned

all: export_libs export_hdrs
       @echo SSLeay build finishes at <Sys$Time>
