; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template





;The program is working and display the output in ASCII in HC12 machine





		ORG	$1000			;data area

message:	FCC	"The Total Sum of array is:"		; string: display message
TerminateString: DB      0
array: dw 47, 121, 114, 34, 44, 117,33 , 124, 255
Result:         DW	0  ;Storage for Result
prtResult dw 0; store starting address of the "Result" here

	org $1070 ;I want to separate the address to avoid memory overwritten

TerminateResult: DB     0  ; TerminateResult as indicator to finish print the Result
store:dw 0;store the SUM here

		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)
	

		; YOUR PROGRAM GOES HERE
		
		ldx #array; register X-> pointer for addition
		ldy #array; register  Y-> pointer for checking 255

sum: ;store is for storing value of sum and reg D do the operation and loading the address value from reg X
	
	

	; we use 2 as increment because we use DW (2bytes)

	;isolate value of array in y, we grab the value and increment the offset by 2, check the "grabbed" value with 255
	; and we decrement the offset again by 2 to retract the array pointer back to the initial position
	
	ldy 2,x+; load value of array and increment the pointer by 2
	sty store; store the array in memory
	cpy #255; compare with 255
	lbeq done; branch if the value is 255
	ldy 2,-x; decrement the array pointer by 2 and load the array
	
	addd 2,x+;do the sum of array
	
	
	jmp sum
	 
done:

	;Store result
	std Result
	

	;get address for TerminateResult and use it as array pointer
	;store array backwards from TerminateResult
	;contents of the Array is character that already been converted into ASCII byte by byte
	ldy #TerminateResult
	dey
	
	;convert result to Ascii
	jsr convertASCII
	
	;since Array Pointer is in Y, I transfer the address of the array to prtResult so I can know where to start the printing the "Result"
	iny
	sty prtResult


	
		LDD	#BAUD19K		;program SCI0's baud rate
		JSR	setbaud

		LDY	#message		;output the message
		JSR	putStr_sc0
	

		;print the result
		ldy prtResult
		jsr putStr_sc0

		BRA *


#include "assign27.asm"
#include "DP256reg.asm"
#include "sci0api.asm"

		END
	