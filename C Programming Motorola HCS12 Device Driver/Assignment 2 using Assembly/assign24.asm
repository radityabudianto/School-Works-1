; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template




;This program works and display the result in ASCII in HC12 machine




		ORG	$1000			;data area
message: fcc "Total number of 1 after toggle:" ;display message
TerminateString db 0; terminate print String here
VAR: dw %1010101001111101 ; declare VAR = 1010 1010 0111 1101
prtResult dw 0; put memory address of result here

	org $1070 ;I want to separate the address to store the reminder backwards
Result:         DW	0  ;Storage for Result
TerminateResult: DB     0  ; TerminateResult as indicator to finish print the Result


		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)
	

		; YOUR PROGRAM GOES HERE
		


		ldd var; load VAR to register D

		;flip the bit in register D
		coma
		comb
		
		;initialize counter, 16 from number of bits

		ldx #16



;check value of every single bits		
countBit:	
		
			
		
	;shift left 16 bit, check value of Carry and if its 1 branch to "get1"
	asld 
	bcs get1 
	
	dbeq x,done ;dec X until X reach 0 and done
	jmp countBit
		

	
	;count number of 1
	get1:
		;register Y is the counter, increment counter everytime Carry is 1
		iny
		dbeq x,done ;dec X until X reach 0 and done
		jmp countBit


done:		
		;put the result in register D
		tfr y,d	
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


		;print the result
		ldy prtResult 
		JSR	putStr_sc0

		BRA *

#include "assign27.asm"
#include "DP256reg.asm"
#include "sci0api.asm"

		END
	