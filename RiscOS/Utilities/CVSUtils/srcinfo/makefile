#/************************************************************************/
#/*                  Copyright 1996 Acorn Computers Ltd                  */
#/*                                                                      */
#/*  This material is the confidential trade secret and proprietary      */
#/*  information of Acorn Computers. It may not be reproduced, used      */
#/*  sold, or transferred to any third party without the prior written   */
#/*  consent of Acorn Computers. All rights reserved.                    */
#/*                                                                      */
#/************************************************************************/
#
# 11 Aug 1997
#
CC 	= gcc
LINK	= gcc
RM     	= rm -f

INCDIRS = 
LIBDIRS = 

OBJS   = srcinfo.o md5.o
HDRS   = md5.h
TARGET = srcinfo

# Static dependencies:
#
.c.o:;        $(CC) -c $(CFLAGS) $<

all: $(TARGET)
	@echo $(TARGET) built

$(TARGET): $(OBJS) $(HDRS)
	$(LINK) $(OBJS) -o $@

clean:
	$(RM) *.o
	$(RM) $(TARGET)
	@echo $(TARGET) cleaned
