
;This part is working and can be integrated with part 1 and some other part of the assignment







convertAscii:
pshd
	ldx #10; division by 10 to get reminder
	
splits:	
	
			
	idiv	;divide by 10 to get reminder
	addb #$30; add the reminder by 30
	stab ,Y; store the (reminder+30) into address in register Y(reg Y is used for array pointer)
	tfr x,d ;reminder in D, the value of X need to be transfer to D again to do another division
	ldx #10; X is used for division in previous command and we need X for divisor(10)
	dey;move the array pointer backward
	   ;fill the reminder in backward order, because putStr_sc0 print the value in forward order
	
	cpd #$0; when we get the Quotient is 0, it means there is no more character left
	beq finishConversion;
	
	jmp splits;keep looping until get TerminateString

finishConversion:

puld

rts ;return from jsr	