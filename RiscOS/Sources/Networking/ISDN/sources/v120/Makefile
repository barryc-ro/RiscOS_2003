#
#         Example Makefile for V120 entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.^.par -I^.services -I^.dl 

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM -ff

LINK = link
NOTMODULE_LINKFLAGS = -AOF
MODULE_LINKFLAGS = $(NOTMODULE_LINKFLAGS) 

CMHG = cmhg
CMHGFLAGS =

OBJASM = objasm 

AR = libfile
AR_FLAGS = -c -o

WIPE = -wipe
WIPEFLAGS = F~C~R~V

MODSQZ = modsqz -fv

# ----------------------------------------------------------------- #
# Rules for making things - do not change.
# ----------------------------------------------------------------- #
	   
.SUFFIXES:	.o .c .cmhg .s
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(MODULE_CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

# ----------------------------------------------------------------- #
# Targets to be compiled
# ----------------------------------------------------------------- #

TARGETS = vs.o\
 app_sm.o\
 v120.o\
 vs_acc.o\
 vs_cfg.o\
 vs_ctrl.o\
 vs_err.o\
 vs_evnt.o\
 vs_exe.o\
 vs_inp.o\
 vs_outp.o\
 vs_parm.o\
 vs_proc.o\
 vs_tab.o\
 vs_term.o\
 vs_time.o\
 vs_util.o

 
#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.v120:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.vs:	c.vs
o.vs:	h.vs
o.vs:	^.services.h.services
o.vs:	^.^.par.h.serv_par
o.vs:	^.^.par.h.consts
o.vs:	^.^.par.h._product
o.vs:	c:h.string
o.vs:	^.^.par.h.serv_por
o.vs:	c:sys.h.types
o.vs:	c:h.kernel
o.vs:	c:h.swis
o.vs:	h.vs_unit
o.vs:	^.^.par.h.v120_par
o.vs:	^.dl.h.v120_int
o.vs:	h.vs_int
o.vs:	h.vs_util
o.vs:	h.vs_parm
o.vs:	h.app_sm
o.vs:	h.v120
o.app_sm:	c.app_sm
o.app_sm:	^.services.h.services
o.app_sm:	^.^.par.h.serv_par
o.app_sm:	^.^.par.h.consts
o.app_sm:	^.^.par.h._product
o.app_sm:	c:h.string
o.app_sm:	^.^.par.h.serv_por
o.app_sm:	c:sys.h.types
o.app_sm:	c:h.kernel
o.app_sm:	c:h.swis
o.app_sm:	h.vs
o.app_sm:	^.services.h.services
o.app_sm:	h.vs_unit
o.app_sm:	^.^.par.h.v120_par
o.app_sm:	^.dl.h.v120_int
o.app_sm:	h.vs_int
o.app_sm:	h.vs_util
o.app_sm:	h.vs_parm
o.app_sm:	h.app_sm
o.app_sm:	h.v120
o.app_sm:	h.app_sm
o.app_sm:	^.dl.h.dl_int
o.app_sm:	^.^.par.h.dl_par
o.v120:	c.v120
o.v120:	h.vs
o.v120:	^.services.h.services
o.v120:	^.^.par.h.serv_par
o.v120:	^.^.par.h.consts
o.v120:	^.^.par.h._product
o.v120:	c:h.string
o.v120:	^.^.par.h.serv_por
o.v120:	c:sys.h.types
o.v120:	c:h.kernel
o.v120:	c:h.swis
o.v120:	h.vs_unit
o.v120:	^.^.par.h.v120_par
o.v120:	^.dl.h.v120_int
o.v120:	h.vs_int
o.v120:	h.vs_util
o.v120:	h.vs_parm
o.v120:	h.app_sm
o.v120:	h.v120
o.v120:	^.dl.h.dl_int
o.v120:	^.^.par.h.dl_par
o.vs_acc:	c.vs_acc
o.vs_acc:	h.vs
o.vs_acc:	^.services.h.services
o.vs_acc:	^.^.par.h.serv_par
o.vs_acc:	^.^.par.h.consts
o.vs_acc:	^.^.par.h._product
o.vs_acc:	c:h.string
o.vs_acc:	^.^.par.h.serv_por
o.vs_acc:	c:sys.h.types
o.vs_acc:	c:h.kernel
o.vs_acc:	c:h.swis
o.vs_acc:	h.vs_unit
o.vs_acc:	^.^.par.h.v120_par
o.vs_acc:	^.dl.h.v120_int
o.vs_acc:	h.vs_int
o.vs_acc:	h.vs_util
o.vs_acc:	h.vs_parm
o.vs_acc:	h.app_sm
o.vs_acc:	h.v120
o.vs_cfg:	c.vs_cfg
o.vs_cfg:	h.vs
o.vs_cfg:	^.services.h.services
o.vs_cfg:	^.^.par.h.serv_par
o.vs_cfg:	^.^.par.h.consts
o.vs_cfg:	^.^.par.h._product
o.vs_cfg:	c:h.string
o.vs_cfg:	^.^.par.h.serv_por
o.vs_cfg:	c:sys.h.types
o.vs_cfg:	c:h.kernel
o.vs_cfg:	c:h.swis
o.vs_cfg:	h.vs_unit
o.vs_cfg:	^.^.par.h.v120_par
o.vs_cfg:	^.dl.h.v120_int
o.vs_cfg:	h.vs_int
o.vs_cfg:	h.vs_util
o.vs_cfg:	h.vs_parm
o.vs_cfg:	h.app_sm
o.vs_cfg:	h.v120
o.vs_ctrl:	c.vs_ctrl
o.vs_ctrl:	h.vs
o.vs_ctrl:	^.services.h.services
o.vs_ctrl:	^.^.par.h.serv_par
o.vs_ctrl:	^.^.par.h.consts
o.vs_ctrl:	^.^.par.h._product
o.vs_ctrl:	c:h.string
o.vs_ctrl:	^.^.par.h.serv_por
o.vs_ctrl:	c:sys.h.types
o.vs_ctrl:	c:h.kernel
o.vs_ctrl:	c:h.swis
o.vs_ctrl:	h.vs_unit
o.vs_ctrl:	^.^.par.h.v120_par
o.vs_ctrl:	^.dl.h.v120_int
o.vs_ctrl:	h.vs_int
o.vs_ctrl:	h.vs_util
o.vs_ctrl:	h.vs_parm
o.vs_ctrl:	h.app_sm
o.vs_ctrl:	h.v120
o.vs_ctrl:	^.dl.h.dl_int
o.vs_ctrl:	^.^.par.h.dl_par
o.vs_err:	c.vs_err
o.vs_err:	h.vs
o.vs_err:	^.services.h.services
o.vs_err:	^.^.par.h.serv_par
o.vs_err:	^.^.par.h.consts
o.vs_err:	^.^.par.h._product
o.vs_err:	c:h.string
o.vs_err:	^.^.par.h.serv_por
o.vs_err:	c:sys.h.types
o.vs_err:	c:h.kernel
o.vs_err:	c:h.swis
o.vs_err:	h.vs_unit
o.vs_err:	^.^.par.h.v120_par
o.vs_err:	^.dl.h.v120_int
o.vs_err:	h.vs_int
o.vs_err:	h.vs_util
o.vs_err:	h.vs_parm
o.vs_err:	h.app_sm
o.vs_err:	h.v120
o.vs_evnt:	c.vs_evnt
o.vs_evnt:	h.vs
o.vs_evnt:	^.services.h.services
o.vs_evnt:	^.^.par.h.serv_par
o.vs_evnt:	^.^.par.h.consts
o.vs_evnt:	^.^.par.h._product
o.vs_evnt:	c:h.string
o.vs_evnt:	^.^.par.h.serv_por
o.vs_evnt:	c:sys.h.types
o.vs_evnt:	c:h.kernel
o.vs_evnt:	c:h.swis
o.vs_evnt:	h.vs_unit
o.vs_evnt:	^.^.par.h.v120_par
o.vs_evnt:	^.dl.h.v120_int
o.vs_evnt:	h.vs_int
o.vs_evnt:	h.vs_util
o.vs_evnt:	h.vs_parm
o.vs_evnt:	h.app_sm
o.vs_evnt:	h.v120
o.vs_evnt:	^.dl.h.dl_int
o.vs_evnt:	^.^.par.h.dl_par
o.vs_exe:	c.vs_exe
o.vs_exe:	h.vs
o.vs_exe:	^.services.h.services
o.vs_exe:	^.^.par.h.serv_par
o.vs_exe:	^.^.par.h.consts
o.vs_exe:	^.^.par.h._product
o.vs_exe:	c:h.string
o.vs_exe:	^.^.par.h.serv_por
o.vs_exe:	c:sys.h.types
o.vs_exe:	c:h.kernel
o.vs_exe:	c:h.swis
o.vs_exe:	h.vs_unit
o.vs_exe:	^.^.par.h.v120_par
o.vs_exe:	^.dl.h.v120_int
o.vs_exe:	h.vs_int
o.vs_exe:	h.vs_util
o.vs_exe:	h.vs_parm
o.vs_exe:	h.app_sm
o.vs_exe:	h.v120
o.vs_exe:	^.dl.h.dl_int
o.vs_exe:	^.^.par.h.dl_par
o.vs_inp:	c.vs_inp
o.vs_inp:	h.vs
o.vs_inp:	^.services.h.services
o.vs_inp:	^.^.par.h.serv_par
o.vs_inp:	^.^.par.h.consts
o.vs_inp:	^.^.par.h._product
o.vs_inp:	c:h.string
o.vs_inp:	^.^.par.h.serv_por
o.vs_inp:	c:sys.h.types
o.vs_inp:	c:h.kernel
o.vs_inp:	c:h.swis
o.vs_inp:	h.vs_unit
o.vs_inp:	^.^.par.h.v120_par
o.vs_inp:	^.dl.h.v120_int
o.vs_inp:	h.vs_int
o.vs_inp:	h.vs_util
o.vs_inp:	h.vs_parm
o.vs_inp:	h.app_sm
o.vs_inp:	h.v120
o.vs_inp:	^.dl.h.dl_int
o.vs_inp:	^.^.par.h.dl_par
o.vs_outp:	c.vs_outp
o.vs_outp:	h.vs
o.vs_outp:	^.services.h.services
o.vs_outp:	^.^.par.h.serv_par
o.vs_outp:	^.^.par.h.consts
o.vs_outp:	^.^.par.h._product
o.vs_outp:	c:h.string
o.vs_outp:	^.^.par.h.serv_por
o.vs_outp:	c:sys.h.types
o.vs_outp:	c:h.kernel
o.vs_outp:	c:h.swis
o.vs_outp:	h.vs_unit
o.vs_outp:	^.^.par.h.v120_par
o.vs_outp:	^.dl.h.v120_int
o.vs_outp:	h.vs_int
o.vs_outp:	h.vs_util
o.vs_outp:	h.vs_parm
o.vs_outp:	h.app_sm
o.vs_outp:	h.v120
o.vs_outp:	^.dl.h.dl_int
o.vs_outp:	^.^.par.h.dl_par
o.vs_parm:	c.vs_parm
o.vs_parm:	h.vs
o.vs_parm:	^.services.h.services
o.vs_parm:	^.^.par.h.serv_par
o.vs_parm:	^.^.par.h.consts
o.vs_parm:	^.^.par.h._product
o.vs_parm:	c:h.string
o.vs_parm:	^.^.par.h.serv_por
o.vs_parm:	c:sys.h.types
o.vs_parm:	c:h.kernel
o.vs_parm:	c:h.swis
o.vs_parm:	h.vs_unit
o.vs_parm:	^.^.par.h.v120_par
o.vs_parm:	^.dl.h.v120_int
o.vs_parm:	h.vs_int
o.vs_parm:	h.vs_util
o.vs_parm:	h.vs_parm
o.vs_parm:	h.app_sm
o.vs_parm:	h.v120
o.vs_proc:	c.vs_proc
o.vs_proc:	h.vs
o.vs_proc:	^.services.h.services
o.vs_proc:	^.^.par.h.serv_par
o.vs_proc:	^.^.par.h.consts
o.vs_proc:	^.^.par.h._product
o.vs_proc:	c:h.string
o.vs_proc:	^.^.par.h.serv_por
o.vs_proc:	c:sys.h.types
o.vs_proc:	c:h.kernel
o.vs_proc:	c:h.swis
o.vs_proc:	h.vs_unit
o.vs_proc:	^.^.par.h.v120_par
o.vs_proc:	^.dl.h.v120_int
o.vs_proc:	h.vs_int
o.vs_proc:	h.vs_util
o.vs_proc:	h.vs_parm
o.vs_proc:	h.app_sm
o.vs_proc:	h.v120
o.vs_proc:	^.dl.h.dl_int
o.vs_proc:	^.^.par.h.dl_par
o.vs_tab:	c.vs_tab
o.vs_tab:	h.vs
o.vs_tab:	^.services.h.services
o.vs_tab:	^.^.par.h.serv_par
o.vs_tab:	^.^.par.h.consts
o.vs_tab:	^.^.par.h._product
o.vs_tab:	c:h.string
o.vs_tab:	^.^.par.h.serv_por
o.vs_tab:	c:sys.h.types
o.vs_tab:	c:h.kernel
o.vs_tab:	c:h.swis
o.vs_tab:	h.vs_unit
o.vs_tab:	^.^.par.h.v120_par
o.vs_tab:	^.dl.h.v120_int
o.vs_tab:	h.vs_int
o.vs_tab:	h.vs_util
o.vs_tab:	h.vs_parm
o.vs_tab:	h.app_sm
o.vs_tab:	h.v120
o.vs_tab:	^.dl.h.dl_int
o.vs_tab:	^.^.par.h.dl_par
o.vs_term:	c.vs_term
o.vs_term:	h.vs
o.vs_term:	^.services.h.services
o.vs_term:	^.^.par.h.serv_par
o.vs_term:	^.^.par.h.consts
o.vs_term:	^.^.par.h._product
o.vs_term:	c:h.string
o.vs_term:	^.^.par.h.serv_por
o.vs_term:	c:sys.h.types
o.vs_term:	c:h.kernel
o.vs_term:	c:h.swis
o.vs_term:	h.vs_unit
o.vs_term:	^.^.par.h.v120_par
o.vs_term:	^.dl.h.v120_int
o.vs_term:	h.vs_int
o.vs_term:	h.vs_util
o.vs_term:	h.vs_parm
o.vs_term:	h.app_sm
o.vs_term:	h.v120
o.vs_term:	^.dl.h.dl_int
o.vs_term:	^.^.par.h.dl_par
o.vs_time:	c.vs_time
o.vs_time:	h.vs
o.vs_time:	^.services.h.services
o.vs_time:	^.^.par.h.serv_par
o.vs_time:	^.^.par.h.consts
o.vs_time:	^.^.par.h._product
o.vs_time:	c:h.string
o.vs_time:	^.^.par.h.serv_por
o.vs_time:	c:sys.h.types
o.vs_time:	c:h.kernel
o.vs_time:	c:h.swis
o.vs_time:	h.vs_unit
o.vs_time:	^.^.par.h.v120_par
o.vs_time:	^.dl.h.v120_int
o.vs_time:	h.vs_int
o.vs_time:	h.vs_util
o.vs_time:	h.vs_parm
o.vs_time:	h.app_sm
o.vs_time:	h.v120
o.vs_util:	c.vs_util
o.vs_util:	h.vs
o.vs_util:	^.services.h.services
o.vs_util:	^.^.par.h.serv_par
o.vs_util:	^.^.par.h.consts
o.vs_util:	^.^.par.h._product
o.vs_util:	c:h.string
o.vs_util:	^.^.par.h.serv_por
o.vs_util:	c:sys.h.types
o.vs_util:	c:h.kernel
o.vs_util:	c:h.swis
o.vs_util:	h.vs_unit
o.vs_util:	^.^.par.h.v120_par
o.vs_util:	^.dl.h.v120_int
o.vs_util:	h.vs_int
o.vs_util:	h.vs_util
o.vs_util:	h.vs_parm
o.vs_util:	h.app_sm
o.vs_util:	h.v120
