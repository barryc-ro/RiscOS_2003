; lib/romodule/tabserv.s
;
; This defines the table inside the serving module that
; is used to fill in the clients call table.


                AREA    |C$$code|, CODE, READONLY

                IMPORT  |__main|

                B       |__main|

V_bit		*	1 :SHL: 28
XOS_WriteS	*	( 1 :SHL: 17) :OR: 1

                MACRO
$label          SSL_TABLE_ENTRY
		IMPORT	$label
                DCD     $label
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





;
; Patch the user-supplied jump table to point to our routines.
;
; Entry
;
; r0    start of user table
; r1    end of user table
;
; Exit
;
; Error is raised
;

; abs - server + client
; 8

		EXPORT	patch_user_table

patch_user_table ROUT
                BIC     lr, lr, #V_bit
                STMFD   sp!, {lr}
		SWI	XOS_WriteS
		DCB	"Server patching table",13,10,0
                ADRL    r2, jump_table_start
                ADRL    r3, jump_table_end
                SUB     ip, r3, r2
                SUB     lr, r1, r0
                CMP     lr, ip
                BNE     %ft00
;                SUB     r3, r2, r0              ; displacement

01              LDR     ip, [r2], #4
                SUB     ip, ip, r0
                SUB     ip, ip, #8              ; correct for PC
                MOV     ip, ip, LSR #2          ; measure in words
		BIC	ip, ip, #&ff000000
                ORR     ip, ip, #&ea000000               ; BAL instruction
                STR     ip, [r0], #4
                CMP     r0, r1
                BLT     %bt01

		MOV	r0, #0
                LDMFD   sp!, {pc}^

; Table sizes differ - messed up somewhere.   

00              ADR     r0, %ft99
                LDMFD   sp!, {lr}
                ORRS    pc, lr, #V_bit
99
                DCD     &80ef00                       ; use official number
                DCB     "Bad SSL table size", 0

		EXPORT	no_such_swi

no_such_swi
		DCD	&1e6
		DCB	"No such SSL SWI", 0

		END



