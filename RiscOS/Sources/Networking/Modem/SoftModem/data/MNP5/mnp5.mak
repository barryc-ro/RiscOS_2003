#
#     Copyright, 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995.
#     All Rights Reserved by:
#        RSA
#        7701 Six Forks Road
#        Suite 201
#        Raleigh, NC  27615
#        (919) 846-7171
#
#    This document contains material confidential to RSA. Its contents
#    must not be revealed, used or disclosed to anyone or company without
#    written permission by RSA. The information contained herein is solely
#    for the use of RSA.
#
#	MAKE FILE FOR:
#     MNP5 
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f mnp5'        to add MNP5 data compression
#       type 'make -f mnp5 stub'   to stub MNP5
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# MNP5 objects
#
CL5 =	cl5_dat.o mnp_af.o

CL5STUB =   mnp_no5.o

CL5INC = cl5_ede.h

#
#  Tools Flags
#
LPATH  = data.mnp5.
HPATH  = data.v42.
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc $(LPATH) $(HPATH)

#
#  Main Executable
#
cl5code:    $(CL5)
            #@ echo $(CL5:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

cl5stub:    $(CL5STUB)
            #@ echo $(CL5STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: mnp5.mak $(CL5:S/obj/c) $(CL5INC)
      #@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(CL5STUB:S/obj/c) mnp5.mak 
      #@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
# MNP5 Source
#
mnp_af.o:	mnp_af.c sys_def.h edf.mnp_lib edf.dte mnp_def.h mnp_stru.h \
         mnp_ede.h cl5_ede.h 
		   $(CC) $(CFLGS) $(LPATH)mnp_af.c

cl5_dat.o:	cl5_dat.c sys_def.h
		   $(CC) $(CFLGS) $(LPATH)cl5_dat.c 

mnp_no5.o:	mnp_no5.c sys_def.h edf.mnp_lib edf.dte mnp_def.h mnp_stru.h \
		   mnp_ede.h edf.m10_if
		   $(CC) $(CFLGS) $(LPATH)mnp_no5.c 


# Dynamic dependencies:
o.cl5_dat:	data.mnp5.c.cl5_dat
o.cl5_dat:	inc.h.sys_def
o.cl5_dat:	inc.h.__config
o.cl5_dat:	inc.h.risc_os
o.mnp_af:	data.mnp5.c.mnp_af
o.mnp_af:	inc.h.sys_def
o.mnp_af:	inc.h.__config
o.mnp_af:	inc.h.risc_os
o.mnp_af:	data.v42.EDF.MNP_LIB
o.mnp_af:	inc.EDF.DTE
o.mnp_af:	data.v42.H.MNP_DEF
o.mnp_af:	data.v42.H.MNP_STRU
o.mnp_af:	data.v42.H.MNP_EDE
o.mnp_af:	data.mnp5.H.CL5_EDE
