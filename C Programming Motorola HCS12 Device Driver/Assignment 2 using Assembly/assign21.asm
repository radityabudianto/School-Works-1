; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template



;This program works and show the output in HC12 machine
;


;the printed result is already converted in ASCII integrated with part 7

;only can print positive result




		ORG	$1000	;data area 
X:		dw	 4 ;X=4
Y:		dw	 45;Y=45
message:	FCC	"The result is:"		; string: display message
TerminateString: DB     0  ; TerminateString as indicator to end print String
prtResult dw 0; store starting address of the "Result" here
	

	org $1070 ;I want to separate the address to avoid memory overwritten
Result:         DW	0  ;Storage for Result
TerminateResult: DB     0  ; TerminateResult as indicator to finish print the Result

		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)
; YOUR PROGRAM GOES HERE

		

		
		;All the operation done in 16Bit in register D
		ldd Y; load value of Y
		addd #$0007;Y+7
		subd X;Y+7-X
		std Result;store result(Result=Y+7-X) in D
			
		;get address for TerminateResult and use it as array pointer
		;store array backwards from TerminateResult
		;contents of the Array is character that already been converted into ASCII byte by byte
		ldy #TerminateResult	
		dey
		
		;convert result to Ascii	
		JSR convertAscii

		

		;since Array Pointer is in Y, I transfer the address of the array to prtResult so I can know where to start the printing the "Result"
		iny
		sty prtResult	
		
		
	LDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud		
	
		
	LDY	#message		;output the message
	JSR	putStr_sc0
	

	;output the result
	LDY	prtResult		
	JSR	putStr_sc0

	
	BRA *
#include "assign27.asm"
#include "DP256reg.asm"
#include "sci0api.asm"
		END
	