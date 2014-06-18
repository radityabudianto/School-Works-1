; SYSC 2003 - Intro to Real-Time Systems - Winter 2010
; Print on terminal template





;This program is working and display output in HC12 machine
;The output is either 0 or 1





		ORG	$1000			;data area


firstString: fcc "abcdefghij";10 bytes first string declaration
secondString: fcc "abcdefghij";10 bytes second string declaration

message:	FCC	"Return 0 if the strings are different, and 1 if both the same:"		; string: display message
Result: db 0; store result either 1 or 0
TerminateString: DB      0


		ORG	$4000			;program area
start:
		LDS	#$3DFF			;initialize stack pointer (details later)



		;set counter n=10 counter and set the Array pointer
		
		ldx #firstString; array pointer for firstString in register X
		ldy #10; counter =10, because it 10bits



;we compare firstString in Register A and secondString in Register B
;we subtract those two value and if the result is zero, then the bytes are the same
;if the program find the value 1 byte different, then set the value to 1 without need to check the next bit
compare:
	;max offset is 8, so need to inx 2x to get 10 offset
	ldaa ,x
	ldab 10,x
	inx



	;check if it is not 0 means its different, because byte in A and B are supposedly same
	sba ;subtract the byte in reg A and B
	cmpa #$0; if is not 0
	bne different; then set value in D into 1
	
	
	
	dbne y, compare; unless counter =0, jump to compare
	


same:
	clra 
	ldab #1 
	jmp done	 
different: 
	clra;clear A
	ldab #0
	jmp done
done:
		; YOUR PROGRAM GOES HERE
	
	
	;convert to Ascii and store it in Result
	;since we know the result only 0 or 1, we just add the result with 30 to do ASCII conversion
	addd #$30
	stb Result	
		
		LDD	#BAUD19K		;program SCI0's baud rate
		JSR	setbaud

		LDY	#message		;output the message
		JSR	putStr_sc0
	

		BRA *

#include "DP256reg.asm"
#include "sci0api.asm"

		END
	