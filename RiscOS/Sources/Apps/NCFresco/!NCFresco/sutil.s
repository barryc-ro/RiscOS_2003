; > sutil.s
;

 [ {FALSE}
        GET     Hdr:ListOpts
        GET     Hdr:Macros
        GET     Hdr:System
 ]

; extern void sutil_get_relocation_offsets(int *stack_limit);

		AREA	|C$$CODE|,CODE,READONLY

		EXPORT	sutil_get_relocation_offsets
sutil_get_relocation_offsets
		SUB	r1, sl, #540
		LDMIA	r1, {r2,r3}
		STMIA	r0, {r2,r3}
		MOVS	pc, lr

 [ {TRUE}

pc_store	DCD	0

nostack_err	DCD	0
		DCB	"The Network Computer is out of stack and must restart", 0
		ALIGN

prog_name	DCB	"NCFresco", 0
		ALIGN


		EXPORT	stack_extensions
stack_extensions	DCD	0
					
; extern void *my_kernel_alloc(int size);
	
		EXPORT	my_kernel_alloc
		IMPORT	malloc
my_kernel_alloc	
		STR	lr, pc_store

		BL	malloc

		CMP	r0, #0

		LDRNE	r1, stack_extensions
		ADDNE	r1, r1, #1
		STRNE	r1, stack_extensions
		LDRNE	pc, pc_store	

		ADR	r0, nostack_err
		MOV	r1, #1
		ADR	r2, prog_name
		SWI	&440DF		;; 	Wimp_ReportError	
		LDR	pc, pc_store

 ]	
        	END
