; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template



;This program works and show the output in HC12 machine
;the printed result is already converted in ASCII
;only can print positive result
; X and Y must lesser than 256







		ORG	$1000			;data area
X:		db 20 ;define  variable X
Y:		db 2	; define variable Y
message:	FCC	"The result is:"		; string: display message
TerminateString DB      0 ;indicate end of print string
prtResult dw 0; starting address of the print Result will be stored here 

		org 	$1070	;separate memory address to avoid memory overwritten
Result:         DW  	0	; your result variable is this one; store the result here
TerminateResult db 0 ;indicate end of printResult


		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)


		; YOUR PROGRAM GOES HERE
		
		; define X
		ldab X		
	
		;X-Y-Y
		subb Y	
		subb Y	
	
		;store in (x-2y)
		stab Result
		
		
		; register D =Result+Result+Result
		addb Result
		addb Result
			
		;store result
		std Result
		

		;get address for TerminateResult and use it as array pointer
		;store array backwards from TerminateResult
		;contents of the Array is character that already been converted into ASCII byte by byte
		ldy #TerminateResult	
		dey
		
		
		;convert Result to ascii
		jsr convertAscii	
		
		;since Array Pointer is in Y, I transfer the address of the array to prtResult so I can know where to start the printing the "Result"
		iny ;increment pointer to make prtResult go to right address
		sty prtResult	;store the address of "Result" in prtResult before start printing
		
		

		LDD	#BAUD19K		;program SCI0's baud rate
		JSR	setbaud

		LDY	#message		;output the message
		JSR	putStr_sc0

		LDY	prtResult		;output the Result
		JSR	putStr_sc0


		BRA *
#include "assign27.asm"
#include "DP256reg.asm"
#include "sci0api.asm"


		END
	