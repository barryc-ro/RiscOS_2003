#
#         Example Makefile for Omnitel Data Link entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -Ic: -I^.services -I^.ph

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

TARGETS = mc.o\
	mdl.o\
	mdl_net.o\
	mdl_user.o\
	dl.o\
	dl_accss.o\
	dl_mfo.o\
	dl_misc.o\
	dl_pb128.o\
	dl_pb8.o\
	dl_pd128.o\
	dl_pd8.o\
	dl_queue.o\
	dl_rcv.o\
	dl_snd.o\
	dl_state.o\
	dl_switc.o\
	dl_to.o\
	dl_ub128.o\
	dl_ub8.o\
	dl_ud128.o\
	dl_ud8.o\
	dl_u_st.o\
	dl_relay.o\
	xid.o\
    	dl_pic.o\
	v120_xid.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.dl:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ -VIA Targets 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.mc:	c.mc
o.mc:	h.dl
o.mc:	^.services.h.services
o.mc:	^.^.par.h.serv_par
o.mc:	^.^.par.h.consts
o.mc:	^.^.par.h._product
o.mc:	c:h.string
o.mc:	^.^.par.h.serv_por
o.mc:	c:sys.h.types
o.mc:	c:h.kernel
o.mc:	c:h.swis
o.mc:	h.dl_int
o.mc:	^.^.par.h.dl_par
o.mc:	^.ph.h.ph_int
o.mc:	^.^.par.h.ph_par
o.mc:	h.v120_int
o.mc:	h.xid_int
o.mc:	^.^.par.h.xid_par
o.mc:	^.^.par.h.dl_par
o.mc:	^.services.h.ctx_ext
o.mc:	h.xid_int
o.mdl:	c.mdl
o.mdl:	h.dl
o.mdl:	^.services.h.services
o.mdl:	^.^.par.h.serv_par
o.mdl:	^.^.par.h.consts
o.mdl:	^.^.par.h._product
o.mdl:	c:h.string
o.mdl:	^.^.par.h.serv_por
o.mdl:	c:sys.h.types
o.mdl:	c:h.kernel
o.mdl:	c:h.swis
o.mdl:	h.dl_int
o.mdl:	^.^.par.h.dl_par
o.mdl:	^.ph.h.ph_int
o.mdl:	^.^.par.h.ph_par
o.mdl:	h.v120_int
o.mdl:	h.xid_int
o.mdl:	^.^.par.h.xid_par
o.mdl:	^.^.par.h.dl_par
o.mdl_net:	c.mdl_net
o.mdl_net:	h.dl
o.mdl_net:	^.services.h.services
o.mdl_net:	^.^.par.h.serv_par
o.mdl_net:	^.^.par.h.consts
o.mdl_net:	^.^.par.h._product
o.mdl_net:	c:h.string
o.mdl_net:	^.^.par.h.serv_por
o.mdl_net:	c:sys.h.types
o.mdl_net:	c:h.kernel
o.mdl_net:	c:h.swis
o.mdl_net:	h.dl_int
o.mdl_net:	^.^.par.h.dl_par
o.mdl_net:	^.ph.h.ph_int
o.mdl_net:	^.^.par.h.ph_par
o.mdl_net:	h.v120_int
o.mdl_net:	h.xid_int
o.mdl_net:	^.^.par.h.xid_par
o.mdl_net:	^.^.par.h.dl_par
o.mdl_net:	^.services.h.ctx_ext
o.mdl_user:	c.mdl_user
o.mdl_user:	h.dl
o.mdl_user:	^.services.h.services
o.mdl_user:	^.^.par.h.serv_par
o.mdl_user:	^.^.par.h.consts
o.mdl_user:	^.^.par.h._product
o.mdl_user:	c:h.string
o.mdl_user:	^.^.par.h.serv_por
o.mdl_user:	c:sys.h.types
o.mdl_user:	c:h.kernel
o.mdl_user:	c:h.swis
o.mdl_user:	h.dl_int
o.mdl_user:	^.^.par.h.dl_par
o.mdl_user:	^.ph.h.ph_int
o.mdl_user:	^.^.par.h.ph_par
o.mdl_user:	h.v120_int
o.mdl_user:	h.xid_int
o.mdl_user:	^.^.par.h.xid_par
o.mdl_user:	^.^.par.h.dl_par
o.mdl_user:	^.services.h.ctx_ext
o.dl:	c.dl
o.dl:	h.dl
o.dl:	^.services.h.services
o.dl:	^.^.par.h.serv_par
o.dl:	^.^.par.h.consts
o.dl:	^.^.par.h._product
o.dl:	c:h.string
o.dl:	^.^.par.h.serv_por
o.dl:	c:sys.h.types
o.dl:	c:h.kernel
o.dl:	c:h.swis
o.dl:	h.dl_int
o.dl:	^.^.par.h.dl_par
o.dl:	^.ph.h.ph_int
o.dl:	^.^.par.h.ph_par
o.dl:	h.v120_int
o.dl:	h.xid_int
o.dl:	^.^.par.h.xid_par
o.dl:	^.^.par.h.dl_par
o.dl_accss:	c.dl_accss
o.dl_accss:	h.dl
o.dl_accss:	^.services.h.services
o.dl_accss:	^.^.par.h.serv_par
o.dl_accss:	^.^.par.h.consts
o.dl_accss:	^.^.par.h._product
o.dl_accss:	c:h.string
o.dl_accss:	^.^.par.h.serv_por
o.dl_accss:	c:sys.h.types
o.dl_accss:	c:h.kernel
o.dl_accss:	c:h.swis
o.dl_accss:	h.dl_int
o.dl_accss:	^.^.par.h.dl_par
o.dl_accss:	^.ph.h.ph_int
o.dl_accss:	^.^.par.h.ph_par
o.dl_accss:	h.v120_int
o.dl_accss:	h.xid_int
o.dl_accss:	^.^.par.h.xid_par
o.dl_accss:	^.^.par.h.dl_par
o.dl_accss:	h.dl_comm
o.dl_accss:	^.services.h.ctx_ext
o.dl_mfo:	c.dl_mfo
o.dl_mfo:	h.dl
o.dl_mfo:	^.services.h.services
o.dl_mfo:	^.^.par.h.serv_par
o.dl_mfo:	^.^.par.h.consts
o.dl_mfo:	^.^.par.h._product
o.dl_mfo:	c:h.string
o.dl_mfo:	^.^.par.h.serv_por
o.dl_mfo:	c:sys.h.types
o.dl_mfo:	c:h.kernel
o.dl_mfo:	c:h.swis
o.dl_mfo:	h.dl_int
o.dl_mfo:	^.^.par.h.dl_par
o.dl_mfo:	^.ph.h.ph_int
o.dl_mfo:	^.^.par.h.ph_par
o.dl_mfo:	h.v120_int
o.dl_mfo:	h.xid_int
o.dl_mfo:	^.^.par.h.xid_par
o.dl_mfo:	^.^.par.h.dl_par
o.dl_mfo:	h.dl_comm
o.dl_mfo:	^.services.h.ctx_ext
o.dl_misc:	c.dl_misc
o.dl_misc:	h.dl
o.dl_misc:	^.services.h.services
o.dl_misc:	^.^.par.h.serv_par
o.dl_misc:	^.^.par.h.consts
o.dl_misc:	^.^.par.h._product
o.dl_misc:	c:h.string
o.dl_misc:	^.^.par.h.serv_por
o.dl_misc:	c:sys.h.types
o.dl_misc:	c:h.kernel
o.dl_misc:	c:h.swis
o.dl_misc:	h.dl_int
o.dl_misc:	^.^.par.h.dl_par
o.dl_misc:	^.ph.h.ph_int
o.dl_misc:	^.^.par.h.ph_par
o.dl_misc:	h.v120_int
o.dl_misc:	h.xid_int
o.dl_misc:	^.^.par.h.xid_par
o.dl_misc:	^.^.par.h.dl_par
o.dl_misc:	h.dl_comm
o.dl_misc:	^.services.h.ctx_ext
o.dl_pb128:	c.dl_pb128
o.dl_pb128:	h.dl
o.dl_pb128:	^.services.h.services
o.dl_pb128:	^.^.par.h.serv_par
o.dl_pb128:	^.^.par.h.consts
o.dl_pb128:	^.^.par.h._product
o.dl_pb128:	c:h.string
o.dl_pb128:	^.^.par.h.serv_por
o.dl_pb128:	c:sys.h.types
o.dl_pb128:	c:h.kernel
o.dl_pb128:	c:h.swis
o.dl_pb128:	h.dl_int
o.dl_pb128:	^.^.par.h.dl_par
o.dl_pb128:	^.ph.h.ph_int
o.dl_pb128:	^.^.par.h.ph_par
o.dl_pb128:	h.v120_int
o.dl_pb128:	h.xid_int
o.dl_pb128:	^.^.par.h.xid_par
o.dl_pb128:	^.^.par.h.dl_par
o.dl_pb128:	h.dl_comm
o.dl_pb128:	^.services.h.ctx_ext
o.dl_pb8:	c.dl_pb8
o.dl_pb8:	h.dl
o.dl_pb8:	^.services.h.services
o.dl_pb8:	^.^.par.h.serv_par
o.dl_pb8:	^.^.par.h.consts
o.dl_pb8:	^.^.par.h._product
o.dl_pb8:	c:h.string
o.dl_pb8:	^.^.par.h.serv_por
o.dl_pb8:	c:sys.h.types
o.dl_pb8:	c:h.kernel
o.dl_pb8:	c:h.swis
o.dl_pb8:	h.dl_int
o.dl_pb8:	^.^.par.h.dl_par
o.dl_pb8:	^.ph.h.ph_int
o.dl_pb8:	^.^.par.h.ph_par
o.dl_pb8:	h.v120_int
o.dl_pb8:	h.xid_int
o.dl_pb8:	^.^.par.h.xid_par
o.dl_pb8:	^.^.par.h.dl_par
o.dl_pb8:	h.dl_comm
o.dl_pb8:	^.services.h.ctx_ext
o.dl_pd128:	c.dl_pd128
o.dl_pd128:	h.dl
o.dl_pd128:	^.services.h.services
o.dl_pd128:	^.^.par.h.serv_par
o.dl_pd128:	^.^.par.h.consts
o.dl_pd128:	^.^.par.h._product
o.dl_pd128:	c:h.string
o.dl_pd128:	^.^.par.h.serv_por
o.dl_pd128:	c:sys.h.types
o.dl_pd128:	c:h.kernel
o.dl_pd128:	c:h.swis
o.dl_pd128:	h.dl_int
o.dl_pd128:	^.^.par.h.dl_par
o.dl_pd128:	^.ph.h.ph_int
o.dl_pd128:	^.^.par.h.ph_par
o.dl_pd128:	h.v120_int
o.dl_pd128:	h.xid_int
o.dl_pd128:	^.^.par.h.xid_par
o.dl_pd128:	^.^.par.h.dl_par
o.dl_pd128:	h.dl_comm
o.dl_pd128:	^.services.h.ctx_ext
o.dl_pd128:	h.dl_pck
o.dl_pd8:	c.dl_pd8
o.dl_pd8:	h.dl
o.dl_pd8:	^.services.h.services
o.dl_pd8:	^.^.par.h.serv_par
o.dl_pd8:	^.^.par.h.consts
o.dl_pd8:	^.^.par.h._product
o.dl_pd8:	c:h.string
o.dl_pd8:	^.^.par.h.serv_por
o.dl_pd8:	c:sys.h.types
o.dl_pd8:	c:h.kernel
o.dl_pd8:	c:h.swis
o.dl_pd8:	h.dl_int
o.dl_pd8:	^.^.par.h.dl_par
o.dl_pd8:	^.ph.h.ph_int
o.dl_pd8:	^.^.par.h.ph_par
o.dl_pd8:	h.v120_int
o.dl_pd8:	h.xid_int
o.dl_pd8:	^.^.par.h.xid_par
o.dl_pd8:	^.^.par.h.dl_par
o.dl_pd8:	h.dl_comm
o.dl_pd8:	^.services.h.ctx_ext
o.dl_queue:	c.dl_queue
o.dl_queue:	h.dl
o.dl_queue:	^.services.h.services
o.dl_queue:	^.^.par.h.serv_par
o.dl_queue:	^.^.par.h.consts
o.dl_queue:	^.^.par.h._product
o.dl_queue:	c:h.string
o.dl_queue:	^.^.par.h.serv_por
o.dl_queue:	c:sys.h.types
o.dl_queue:	c:h.kernel
o.dl_queue:	c:h.swis
o.dl_queue:	h.dl_int
o.dl_queue:	^.^.par.h.dl_par
o.dl_queue:	^.ph.h.ph_int
o.dl_queue:	^.^.par.h.ph_par
o.dl_queue:	h.v120_int
o.dl_queue:	h.xid_int
o.dl_queue:	^.^.par.h.xid_par
o.dl_queue:	^.^.par.h.dl_par
o.dl_queue:	h.dl_comm
o.dl_queue:	^.services.h.ctx_ext
o.dl_queue:	^.services.h.it
o.dl_queue:	^.^.par.h.it_par
o.dl_rcv:	c.dl_rcv
o.dl_rcv:	h.dl
o.dl_rcv:	^.services.h.services
o.dl_rcv:	^.^.par.h.serv_par
o.dl_rcv:	^.^.par.h.consts
o.dl_rcv:	^.^.par.h._product
o.dl_rcv:	c:h.string
o.dl_rcv:	^.^.par.h.serv_por
o.dl_rcv:	c:sys.h.types
o.dl_rcv:	c:h.kernel
o.dl_rcv:	c:h.swis
o.dl_rcv:	h.dl_int
o.dl_rcv:	^.^.par.h.dl_par
o.dl_rcv:	^.ph.h.ph_int
o.dl_rcv:	^.^.par.h.ph_par
o.dl_rcv:	h.v120_int
o.dl_rcv:	h.xid_int
o.dl_rcv:	^.^.par.h.xid_par
o.dl_rcv:	^.^.par.h.dl_par
o.dl_rcv:	h.dl_comm
o.dl_rcv:	^.services.h.ctx_ext
o.dl_snd:	c.dl_snd
o.dl_snd:	h.dl
o.dl_snd:	^.services.h.services
o.dl_snd:	^.^.par.h.serv_par
o.dl_snd:	^.^.par.h.consts
o.dl_snd:	^.^.par.h._product
o.dl_snd:	c:h.string
o.dl_snd:	^.^.par.h.serv_por
o.dl_snd:	c:sys.h.types
o.dl_snd:	c:h.kernel
o.dl_snd:	c:h.swis
o.dl_snd:	h.dl_int
o.dl_snd:	^.^.par.h.dl_par
o.dl_snd:	^.ph.h.ph_int
o.dl_snd:	^.^.par.h.ph_par
o.dl_snd:	h.v120_int
o.dl_snd:	h.xid_int
o.dl_snd:	^.^.par.h.xid_par
o.dl_snd:	^.^.par.h.dl_par
o.dl_snd:	h.dl_comm
o.dl_snd:	^.services.h.ctx_ext
o.dl_state:	c.dl_state
o.dl_state:	h.dl
o.dl_state:	^.services.h.services
o.dl_state:	^.^.par.h.serv_par
o.dl_state:	^.^.par.h.consts
o.dl_state:	^.^.par.h._product
o.dl_state:	c:h.string
o.dl_state:	^.^.par.h.serv_por
o.dl_state:	c:sys.h.types
o.dl_state:	c:h.kernel
o.dl_state:	c:h.swis
o.dl_state:	h.dl_int
o.dl_state:	^.^.par.h.dl_par
o.dl_state:	^.ph.h.ph_int
o.dl_state:	^.^.par.h.ph_par
o.dl_state:	h.v120_int
o.dl_state:	h.xid_int
o.dl_state:	^.^.par.h.xid_par
o.dl_state:	^.^.par.h.dl_par
o.dl_state:	h.dl_comm
o.dl_state:	^.services.h.ctx_ext
o.dl_switc:	c.dl_switc
o.dl_switc:	h.dl
o.dl_switc:	^.services.h.services
o.dl_switc:	^.^.par.h.serv_par
o.dl_switc:	^.^.par.h.consts
o.dl_switc:	^.^.par.h._product
o.dl_switc:	c:h.string
o.dl_switc:	^.^.par.h.serv_por
o.dl_switc:	c:sys.h.types
o.dl_switc:	c:h.kernel
o.dl_switc:	c:h.swis
o.dl_switc:	h.dl_int
o.dl_switc:	^.^.par.h.dl_par
o.dl_switc:	^.ph.h.ph_int
o.dl_switc:	^.^.par.h.ph_par
o.dl_switc:	h.v120_int
o.dl_switc:	h.xid_int
o.dl_switc:	^.^.par.h.xid_par
o.dl_switc:	^.^.par.h.dl_par
o.dl_switc:	h.dl_comm
o.dl_switc:	^.services.h.ctx_ext
o.dl_to:	c.dl_to
o.dl_to:	h.dl
o.dl_to:	^.services.h.services
o.dl_to:	^.^.par.h.serv_par
o.dl_to:	^.^.par.h.consts
o.dl_to:	^.^.par.h._product
o.dl_to:	c:h.string
o.dl_to:	^.^.par.h.serv_por
o.dl_to:	c:sys.h.types
o.dl_to:	c:h.kernel
o.dl_to:	c:h.swis
o.dl_to:	h.dl_int
o.dl_to:	^.^.par.h.dl_par
o.dl_to:	^.ph.h.ph_int
o.dl_to:	^.^.par.h.ph_par
o.dl_to:	h.v120_int
o.dl_to:	h.xid_int
o.dl_to:	^.^.par.h.xid_par
o.dl_to:	^.^.par.h.dl_par
o.dl_to:	h.dl_comm
o.dl_to:	^.services.h.ctx_ext
o.dl_ub128:	c.dl_ub128
o.dl_ub128:	h.dl
o.dl_ub128:	^.services.h.services
o.dl_ub128:	^.^.par.h.serv_par
o.dl_ub128:	^.^.par.h.consts
o.dl_ub128:	^.^.par.h._product
o.dl_ub128:	c:h.string
o.dl_ub128:	^.^.par.h.serv_por
o.dl_ub128:	c:sys.h.types
o.dl_ub128:	c:h.kernel
o.dl_ub128:	c:h.swis
o.dl_ub128:	h.dl_int
o.dl_ub128:	^.^.par.h.dl_par
o.dl_ub128:	^.ph.h.ph_int
o.dl_ub128:	^.^.par.h.ph_par
o.dl_ub128:	h.v120_int
o.dl_ub128:	h.xid_int
o.dl_ub128:	^.^.par.h.xid_par
o.dl_ub128:	^.^.par.h.dl_par
o.dl_ub128:	h.dl_comm
o.dl_ub128:	^.services.h.ctx_ext
o.dl_ub8:	c.dl_ub8
o.dl_ub8:	h.dl
o.dl_ub8:	^.services.h.services
o.dl_ub8:	^.^.par.h.serv_par
o.dl_ub8:	^.^.par.h.consts
o.dl_ub8:	^.^.par.h._product
o.dl_ub8:	c:h.string
o.dl_ub8:	^.^.par.h.serv_por
o.dl_ub8:	c:sys.h.types
o.dl_ub8:	c:h.kernel
o.dl_ub8:	c:h.swis
o.dl_ub8:	h.dl_int
o.dl_ub8:	^.^.par.h.dl_par
o.dl_ub8:	^.ph.h.ph_int
o.dl_ub8:	^.^.par.h.ph_par
o.dl_ub8:	h.v120_int
o.dl_ub8:	h.xid_int
o.dl_ub8:	^.^.par.h.xid_par
o.dl_ub8:	^.^.par.h.dl_par
o.dl_ub8:	h.dl_comm
o.dl_ub8:	^.services.h.ctx_ext
o.dl_ud128:	c.dl_ud128
o.dl_ud128:	h.dl
o.dl_ud128:	^.services.h.services
o.dl_ud128:	^.^.par.h.serv_par
o.dl_ud128:	^.^.par.h.consts
o.dl_ud128:	^.^.par.h._product
o.dl_ud128:	c:h.string
o.dl_ud128:	^.^.par.h.serv_por
o.dl_ud128:	c:sys.h.types
o.dl_ud128:	c:h.kernel
o.dl_ud128:	c:h.swis
o.dl_ud128:	h.dl_int
o.dl_ud128:	^.^.par.h.dl_par
o.dl_ud128:	^.ph.h.ph_int
o.dl_ud128:	^.^.par.h.ph_par
o.dl_ud128:	h.v120_int
o.dl_ud128:	h.xid_int
o.dl_ud128:	^.^.par.h.xid_par
o.dl_ud128:	^.^.par.h.dl_par
o.dl_ud128:	h.dl_comm
o.dl_ud128:	^.services.h.ctx_ext
o.dl_ud128:	h.dl_unpck
o.dl_ud8:	c.dl_ud8
o.dl_ud8:	h.dl
o.dl_ud8:	^.services.h.services
o.dl_ud8:	^.^.par.h.serv_par
o.dl_ud8:	^.^.par.h.consts
o.dl_ud8:	^.^.par.h._product
o.dl_ud8:	c:h.string
o.dl_ud8:	^.^.par.h.serv_por
o.dl_ud8:	c:sys.h.types
o.dl_ud8:	c:h.kernel
o.dl_ud8:	c:h.swis
o.dl_ud8:	h.dl_int
o.dl_ud8:	^.^.par.h.dl_par
o.dl_ud8:	^.ph.h.ph_int
o.dl_ud8:	^.^.par.h.ph_par
o.dl_ud8:	h.v120_int
o.dl_ud8:	h.xid_int
o.dl_ud8:	^.^.par.h.xid_par
o.dl_ud8:	^.^.par.h.dl_par
o.dl_ud8:	h.dl_comm
o.dl_ud8:	^.services.h.ctx_ext
o.dl_u_st:	c.dl_u_st
o.dl_u_st:	h.dl
o.dl_u_st:	^.services.h.services
o.dl_u_st:	^.^.par.h.serv_par
o.dl_u_st:	^.^.par.h.consts
o.dl_u_st:	^.^.par.h._product
o.dl_u_st:	c:h.string
o.dl_u_st:	^.^.par.h.serv_por
o.dl_u_st:	c:sys.h.types
o.dl_u_st:	c:h.kernel
o.dl_u_st:	c:h.swis
o.dl_u_st:	h.dl_int
o.dl_u_st:	^.^.par.h.dl_par
o.dl_u_st:	^.ph.h.ph_int
o.dl_u_st:	^.^.par.h.ph_par
o.dl_u_st:	h.v120_int
o.dl_u_st:	h.xid_int
o.dl_u_st:	^.^.par.h.xid_par
o.dl_u_st:	^.^.par.h.dl_par
o.dl_u_st:	h.dl_comm
o.dl_u_st:	^.services.h.ctx_ext
o.dl_relay:	c.dl_relay
o.dl_relay:	h.dl
o.dl_relay:	^.services.h.services
o.dl_relay:	^.^.par.h.serv_par
o.dl_relay:	^.^.par.h.consts
o.dl_relay:	^.^.par.h._product
o.dl_relay:	c:h.string
o.dl_relay:	^.^.par.h.serv_por
o.dl_relay:	c:sys.h.types
o.dl_relay:	c:h.kernel
o.dl_relay:	c:h.swis
o.dl_relay:	h.dl_int
o.dl_relay:	^.^.par.h.dl_par
o.dl_relay:	^.ph.h.ph_int
o.dl_relay:	^.^.par.h.ph_par
o.dl_relay:	h.v120_int
o.dl_relay:	h.xid_int
o.dl_relay:	^.^.par.h.xid_par
o.dl_relay:	^.^.par.h.dl_par
o.dl_relay:	h.dl_comm
o.dl_relay:	^.services.h.ctx_ext
o.dl_relay:	^.services.h.it
o.dl_relay:	^.^.par.h.it_par
o.xid:	c.xid
o.xid:	h.xid
o.xid:	^.services.h.services
o.xid:	^.^.par.h.serv_par
o.xid:	^.^.par.h.consts
o.xid:	^.^.par.h._product
o.xid:	c:h.string
o.xid:	^.^.par.h.serv_por
o.xid:	c:sys.h.types
o.xid:	c:h.kernel
o.xid:	c:h.swis
o.xid:	h.xid_int
o.xid:	^.^.par.h.xid_par
o.xid:	^.^.par.h.dl_par
o.xid:	^.services.h.ctx_ext
o.dl_pic:	c.dl_pic
o.dl_pic:	h.dl
o.dl_pic:	^.services.h.services
o.dl_pic:	^.^.par.h.serv_par
o.dl_pic:	^.^.par.h.consts
o.dl_pic:	^.^.par.h._product
o.dl_pic:	c:h.string
o.dl_pic:	^.^.par.h.serv_por
o.dl_pic:	c:sys.h.types
o.dl_pic:	c:h.kernel
o.dl_pic:	c:h.swis
o.dl_pic:	h.dl_int
o.dl_pic:	^.^.par.h.dl_par
o.dl_pic:	^.ph.h.ph_int
o.dl_pic:	^.^.par.h.ph_par
o.dl_pic:	h.v120_int
o.dl_pic:	h.xid_int
o.dl_pic:	^.^.par.h.xid_par
o.dl_pic:	^.^.par.h.dl_par
o.dl_pic:	h.dl_comm
o.dl_pic:	^.services.h.ctx_ext
o.dl_pic:	h.xid_pic
o.dl_pic:	h.v120_xid
o.v120_xid:	c.v120_xid
o.v120_xid:	h.dl
o.v120_xid:	^.services.h.services
o.v120_xid:	^.^.par.h.serv_par
o.v120_xid:	^.^.par.h.consts
o.v120_xid:	^.^.par.h._product
o.v120_xid:	c:h.string
o.v120_xid:	^.^.par.h.serv_por
o.v120_xid:	c:sys.h.types
o.v120_xid:	c:h.kernel
o.v120_xid:	c:h.swis
o.v120_xid:	h.dl_int
o.v120_xid:	^.^.par.h.dl_par
o.v120_xid:	^.ph.h.ph_int
o.v120_xid:	^.^.par.h.ph_par
o.v120_xid:	h.v120_int
o.v120_xid:	h.xid_int
o.v120_xid:	^.^.par.h.xid_par
o.v120_xid:	^.^.par.h.dl_par
o.v120_xid:	h.dl_comm
o.v120_xid:	^.services.h.ctx_ext
o.v120_xid:	h.v120_xid
