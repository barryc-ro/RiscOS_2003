;;  > upcall.s


        GET     ListOpts
        GET     Macros

        GET     SWIs		;  these four replace the old System header
        GET     CPU/Generic26
        GET     IO/GenericIO
        GET     RISCOS

	GET     ModHand
        GET     EnvNumbers
        GET     Proc
        GET     FSNumbers
        GET     NewErrors
        GET     UpCall

        AREA    |C$CODE|,CODE,READONLY

	EXPORT	print_upcall_claim
	EXPORT	print_upcall_release

upcall_handler
	CMP	r0, #UpCall_MediaSearchEndMessage
	CMPNE	r0, #UpCall_MediaNotPresent
	CMPNE	r0, #UpCall_MediaNotKnown
	MOVNES	pc, lr		; pass it on

	MOV	r0, #-1		; cancel the upcall
	LDMFD	sp!, {pc}	; intercept
		
		
print_upcall_claim	
	Entry	"r4-r9"

	MOV	r0, #UpCallV
	ADR	r1, upcall_handler
	MOV	r2, #0
	SWI	XOS_Claim
	MOVVC	r0, #0

	EXITS
	

print_upcall_release
	Entry	"r4-r9"

	MOV	r0, #UpCallV
	ADR	r1, upcall_handler
	MOV	r2, #0
	SWI	XOS_Release
	MOVVC	r0, #0

	EXITS
	

	END

;; eof upcall.s
	