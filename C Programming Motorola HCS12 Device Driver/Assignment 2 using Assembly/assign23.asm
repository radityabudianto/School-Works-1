; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template




;This program works and shows the result in memory address 1030-1045
;Please, check the result using step by step approach and activate"Update after run" while the machine running to see the result



;In this program I use 0 as F0 
;Therefore the expected result is 0,1,1,2,3,5,8,13,21,24,55
;where 55(37h) is F10


;all the operation happens in register D


	ORG	$1000			;data area
message:	FCC	"The result is:"		; string: display message
TerminateString: DB      0 ;
F0:         DW  	0				; storage for F0
F1:	    DW		0				;storage for F1
F2:	    DW		0				;storage for F2 (F2=F1+F0)


	ORG $1030 ;store the result from register D to this address
prtResult: DB      0 ;start address for result
		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)


		; YOUR PROGRAM GOES HERE
		
		ldy #10 ;n=10 
		
		; set F0 to 0
		ldx #0
		stx F0
		
		;set X as new array pointer 
		ldx #prtResult
 

		;store F0 into X and increment array pointer
		ldd F0
		std prtResult
		
		;set F1 to 1
		ldd #1
		std 2,+X;

loop:		
		;currently D=F1, therefore F2=D+F0
		addd F0
		std F2
		std 2,X+
	
		;set F1 as next F0
		ldd F1
		std F0

		;set F2 as next F1
		ldd F2
		std F1

		
		
		dbeq Y,endloop ;counter for counting n=10 fibonacci
		jmp loop
endloop:
		
	

		
			
		
		LDD	#BAUD19K		;program SCI0's baud rate
		JSR	setbaud

		LDY	#message		;output the message
		JSR	putStr_sc0

		BRA *

#include "DP256reg.asm"
#include "sci0api.asm"

		END
	