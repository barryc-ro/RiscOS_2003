; tabclient.s
;
; This file defines the SSL client object that is linked against
; by application programs. This is just a set of stubs pointing
; into the SLL module (after initialisation!)
;
; (C) ANT Limited 1995. All rights reserved.
;

XOS_WriteS	*	(1 :SHL: 17) :OR: &00001
XSSL_Initialise	*	(1 :SHL: 17) :OR: &4bf00
XOS_Write0	*	(1 :SHL: 17) :OR: &00002
XOS_SynchroniseCodeAreas    *   ( 1 :SHL: 17 ) :OR: &0006e

;-----------------------------------------------------------------------------

                AREA    |C$$code|, CODE, READONLY

                IMPORT  |__main|
		IMPORT	exit

                B       |__main|

;-----------------------------------------------------------------------------

		DCB	"SSLeay stubs (C) ANT Limited 1996.",13,10,0
		ALIGN

;-----------------------------------------------------------------------------

                MACRO
$label          SSL_TABLE_ENTRY
$label          B       not_initialised
                EXPORT  $label
                MEND

                MACRO
                SSL_TABLE_START
jump_table_start
                MEND

                MACRO
                SSL_TABLE_END
jump_table_end
                MEND

                GET     tabdata.s        ; Inclusion file that uses above macro

;---------------------------------------------------------------------------

not_initialised
                SWI     XOS_WriteS
                DCB     "SSL library not initialised - fatal program error", 13,10, 0
		MOV	r0, #1
		B	exit

;---------------------------------------------------------------------------




                EXPORT  SSL_Library_Initialise

; This routine gets called the first time any of the SSL routines
; gets called. It calls the SSL SWI to patch the jump table
;
; Returns NULL or an error pointer - calls exit directly now
;

SSL_Library_Initialise ROUT
		LDR	r0, %00
		LDR	r1, [r0]
		TEQ	r1, #0
		MOVNES	pc, lr
		STR	lr, [r0]
                STMFD   sp!, {lr}
		SWI	XOS_WriteS
		DCB	"Patching table", 13,10,0
                ADRL    r0, jump_table_start
                ADRL    r1, jump_table_end
                SWI     XSSL_Initialise
		BVS	failed
		SWI	XOS_WriteS
		DCB	"PATCHED TABLE",13,10,0
                MOV     r0, #0
                LDMFD   sp!, {pc}^
failed
		SWI	XOS_WriteS
		DCB	"Failed to patch table",13,10,0
		ADD	r0, r0, #4
		SWI	XOS_Write0
		MOV	r0, #1
		B	exit

00
		DCD	init_sema

                EXPORT SSL_LibraryPresent
SSL_LibraryPresent
                LDR     R0,%00
                LDR     R1,[R0]
                CMP     R1,#0
                MOVNE   R0,#0
                MOVNES  PC,LR
                STMFD   R13!,{R14}
                ADRL    R0, jump_table_start
                ADRL    R1, jump_table_end
                SWI     XSSL_Initialise
                LDMVSFD R13!,{PC}

                ; Call RiscOS-SA SWI to flush instruction cache
                MOV     R0,#1
                ADRL    R1, jump_table_start
                ADRL    R2, jump_table_end
                SWI     XOS_SynchroniseCodeAreas

                LDR     R0,%00
                STR     R0,[R0]
                MOV     R0,#0
                LDMFD   R13!,{PC}^

;-----------------------------------------------------------------------------
		AREA |C$$data|,DATA

init_sema	DCD	0

                END

