# PIE/PID demon ROM makefile
#
# RCS $Revision$
# Checkin $Date$
# Revising $Author$

ARMLINK= armlink
ARMASM = armasm
HOSTCC = wcl386
AFLAGS = -li -apcs 3/32bit

#FPOBJS = init_fpe.o fpe_32l.o gee_32l.o
#mrm51496
FPOBJS = 

# FPA support code is not provided in this release
# FPOBJS = init_fpa.o fpe_32l.o gee_32l.o fps_32l.o ges_32l.o

noarg:
    @echo make (target)   where (target) = pie, pid

#crc.exe: crc.c
#     $(HOSTCC) crc.c

#makemmu.exe: makemmu.c
#     $(HOSTCC) makemmu.c

#split.exe: split.c
#     $(HOSTCC) split.c

pie : level0_h.s level0.s level1_h.s level1.s checkmem.s init_rom.s pie\driver.s crc.exe makemmu.exe
    echo $(AFLAGS) -Ipie,. >av
    echo -PD 'TARGET SETS "PIE"' -PD "ForROM SETL {FALSE}"   >>av
    $(ARMASM) -via av level0.s -o pie\level0.o
    $(ARMASM) -via av level1.s -o pie\level1.o
#mrm51496

#    $(ARMASM) -via av init_rom.s -o init_fpe.o
#    $(ARMASM) -via av -PD "Support_FPA SETL {FALSE}" init_rom.s -o init_fpa.o
    echo -base 0x7A000 -last level0.o(LastArea) -bin -o pie\pie >lv
    echo pie\level0.o pie\level1.o $(FPOBJS) >>lv
    $(ARMLINK) -map -via lv
    crc pie\pie
#mrm51496
#    makemmu pie\pie

pid : level0_h.s level0.s level1_h.s level1.s checkmem.s init_rom.s pid\driver.s pid\driver_h.s crc.exe makemmu.exe split.exe
    echo $(AFLAGS) -Ipid,. >av
    echo -PD 'TARGET SETS "PID"' -PD "ForROM SETL {TRUE}" >>av
    $(ARMASM) -via av level0.s -o pid\level0.o
    $(ARMASM) -via av level1.s -o pid\level1.o
    $(ARMASM) -via av init_rom.s -o init_fpe.o
    $(ARMASM) -via av -PD "Support_FPA SETL {TRUE}" init_rom.s -o init_fpa.o
    echo -base 0x70000 -last level0.o(LastArea) -bin -o pid\pid >lv
    echo pid\level0.o pid\level1.o $(FPOBJS) >>lv
    $(ARMLINK) -via lv
    crc pid\pid
    makemmu pid\pid
    split pid\pid
