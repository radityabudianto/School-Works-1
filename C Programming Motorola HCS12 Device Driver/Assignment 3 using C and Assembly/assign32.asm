;This program works and output the exspected result from the input test function


	org $1000

;Declare all that will be printed
StringSpeed: fcc " Speed:"
SpeedValue: rmb 8
enter1: db $0A
TerminateSpeed: db 0

StringProximity fcc" Proximity:"
ProximityValue: rmb 16
enter2: db $0A
TerminateProximity:db 0


;this is list of possibilities that can happened
case1:fcc " there is an obstacle detected, the robot is going to N(0) or NE(4). New Proximity after rotation is:"
ProximityValue1: rmb 16
enter3: db $0A
Terminatecase1: db 0
case2:fcc " there is an obstacle detected, the robot is going S(1) or SW:(7).New Proximity after rotation is:"
ProximityValue2: rmb 16
enter4: db $0A
Terminatecase2:db 0
case3:fcc"  there is obstacle detected, and the robot is not going to N,NE,S, or SW ,New speed after divided by 8 is:"
NewSpeed1:rmb 8
enter5: db $0A
TerminateCase3: db 0
case4:fcc" there is no obstacle detected, multiply Speed by 2. New Speed is:"
NewSpeed2: rmb 8
enter6: db $0A
TerminateCase4: db 0



;set offset variable from top of the stack
Speed equ 5; 5 offset from top SP
Proximity equ 4;4 offset from top SP
localVar equ 2; will be used for of SP-2 as local Variable
temp equ 0; located at SP for temporary storage


path rmb 1;store the path (N,S,E,W,etc)







	org $4000




start:

	lds #$3dff


;case4 Test Function( No obstacle, double the speed)
	
	;load speed (max 127) because variable speed is only 1 byte and case4(2x speed) happened, it will be overflow if its >127.
	ldaa #127; load 127
	psha
	jsr getSpeed
	
	;print the test result
	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringSpeed
	JSR putStr_sc0

	;load proximity address
	ldd #$00d0
	pshd
	jsr getProximity
	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringProximity
	JSR putStr_sc0



	jsr checkCollision
	
	LDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud
	
	tfr x,y	; Register Y is used for something else inside subroutine
	JSR putStr_sc0

	puld
	pula

;case1 Test Function (there is an obstacle detected, the robot is going to N(0) or NE(4).)
	ldaa #90
	psha
	jsr getSpeed

	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringSpeed
	JSR putStr_sc0

	ldd #$F0d0
	pshd
	
	jsr getProximity
	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringProximity
	JSR putStr_sc0



	jsr checkCollision

	LDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud
	
	tfr x,y	
	JSR putStr_sc0
	
	puld
	pula


;case2 Test Function (there is an obstacle detected, the robot is going S(1) or SW:(7)) 
	ldaa #210
	psha
	
	jsr getSpeed

	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringSpeed
	JSR putStr_sc0

	ldd #$F0d1
	pshd
	
	jsr getProximity
	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringProximity
	JSR putStr_sc0


	
	jsr checkCollision

	LDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud
	
	tfr x,y	
	JSR putStr_sc0

	puld
	pula

;case3 Test function (there is obstacle detected, and the robot is not going to N,NE,S, or SW ,New speed after divided by 8 is:)
	ldaa #110
	psha
	
	jsr getSpeed

	lDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud

	ldy #StringSpeed
	JSR putStr_sc0

	ldd #$F0d3
	pshd
	
	jsr checkCollision

	LDD	#BAUD19K		;program SCI0's baud rate
	JSR	setbaud
	
	tfr x,y	
	JSR putStr_sc0

	puld
	pula	


	;end program
	swi












;get the path from bit 0-2
;convert those 3 bits into decimal
;0=N
;1=S
;2=E
;3=W
;4=NE
;5=NW
;6=SE
;7=SW
checkCollision:

	;open local variable
	leas -localVar,sp

	;get MSB of proximity if its 1 then obstacle detected, if its 0 then no obstacle
	checkObstacle:
	ldd Proximity,sp
	lsld
	std temp,sp
	bcs Obstacle1
	bcc Obstacle0

		
	;if obstacle is 0, multpily speed by 2
		Obstacle0:
		ldaa speed,sp
		ldab #2
		mul
		
		;set counter for newSpeed
		ldx #8
		ldy #NewSpeed2
		
		;wrote the NewSpeed bits for case 4
		jsr write8Bits

		;close local variable
		leas localVar,sp
		
		;set the print address for case4
		ldx #case4
		rts;return to caller


	;if Obstacle is 0, then check another case
		Obstacle1:
			;clear register A
			ldaa #0


			;separate bits and get bit0-3 becomes 00000xxx
			lslb 
			lslb 
			lslb 
			lslb 
			lslb 
			lsrb
			lsrb
			lsrb
			lsrb
			lsrb
			lsrb 
			
			;Hex version of the last 3 bits(current direction), store it in temp(current position of Top SP)
			stab temp,sp

			
			
			;check the condition of case 1
			checkCase1:
				
				;check N(0) or NE(4)
				ldaa #0
				cmpa temp,sp;compare 0 to current direction
				beq goCase1 
				
				ldab #4
				cmpb temp,sp;compare 4 to current direction
				beq goCase1 
				
				bra checkCase2
					goCase1:
					;Case1 is change bit8=1 and bit9=0 and store it in &Proximity
						ldd Proximity,sp
						std temp,sp
						
						lsrd
						lsrd
						ldab #64;or in Binary %10000000 need last 2 MSB to be shifted into LSB of register A
						lsld
						lsld
					
						;put previous value of B register
						ldab temp+1,sp

				
						;set new &Proximity Value
						std Proximity,sp
						std temp,sp
						

						;set counter for new Proximity case 1
						ldx #16
						ldy #ProximityValue1
						jsr write16Bits
						leas localVar,sp

						;set the print address for case1
						ldx #case1	
						rts
			

			;if it does not fulfill case1 then check case 2			
			checkCase2:
				;check S(1) or SW(7)
				ldaa #1
				cmpa temp,sp
				beq goCase2 
				
				ldab #7
				cmpb temp,sp
				beq goCase2 
				
				bra goCase3; last case if it does not fulfill case 2

					



					;modify bit 8 and 9 follows the required condition
					goCase2:
					;Case2 is change bit8=0 and bit9=1 and store it in &Proximity
						ldd Proximity,sp
						stab temp,sp	
					
							

						lsrd
						lsrd	
						ldab #128
						lsld	
						lsld



						;load the real value of Register B and recombine it into new &Proximity
						; after we get expected bit 8 and 9, we know that register B(bit 0-7) is not the original value
						; previously I already store it in temp, therefore, I load it to overwrite the wrong value with the original value 
						ldab temp,sp
						
		

				
						;set new &Proximity Value
						std Proximity,sp
						std temp,sp

						;set counter for new Proximity case 2
						ldx #16
						ldy #ProximityValue2
						jsr write16Bits
						leas localVar,sp	


						;set the print address for case2
						ldx #case2	
						rts
			
			
			


					;last case if all the cases above cannot be fulfilled
					goCase3:
						ldd #$0000;clear register D

						;load speed
						ldab speed+1,sp
						ldx #8
						idiv
						tfr x,d
						
						;prepare counter for case3
						ldx #8
						ldy #NewSpeed1
				
						;write binary in memory for case 3
						jsr write8Bits
						leas localVar,sp

						;set the print address for case3
						ldx #case3
						rts
					
		

		;convert 8bits into binary and store it in allocated memory 	
			write8Bits:
				leas -localVar,sp		
			
			StartWrite8Bits:
				lslb
				stab temp,sp
				bcs CarryWrite8Bits1
				bcc CarryWrite8Bits0
					
					CarryWrite8Bits1:
					ldaa #$31
					staa 1,y+
					dex
					beq FinishWrite8Bits
					bra Startwrite8Bits
					
					CarryWrite8Bits0:
					ldaa #$30
					staa 1,y+
					dex
					beq FinishWrite8Bits
					bra Startwrite8Bits
					
			FinishWrite8Bits:
				leas localVar,sp
				rts




		;convert 16bits into binary and store it in allocated memory 		
			write16Bits:
				leas -localVar,sp	
				
				;because there is calling address in stack temp=new temp
				ldd temp+4,sp
				std temp,sp
			
			StartWrite16Bits:
				lsld
				std temp,sp
				bcs CarryWrite16Bits1
				bcc CarryWrite16Bits0
					
					CarryWrite16Bits1:
					ldaa #$31
					staa 1,y+
					dex
					beq FinishWrite16Bits
					ldd temp,sp
					bra Startwrite16Bits
					
					CarryWrite16Bits0:
					ldaa #$30
					staa 1,y+
					dex
					beq FinishWrite16Bits
					ldd temp,sp
					bra Startwrite16Bits
					
			FinishWrite16Bits:
				leas localVar,sp
				rts

		
				
	
			
	

		
	

;convert 8bits into binary and store it in allocated memory 
getSpeed:
	
	leas -localVar,sp;allocate localVariable in stack
	
			ldaa speed,sp; 
			ldx #8; decrement counter
			ldy #SpeedValue; address to store the bit
			bra convertToBin



			convertToBin:;Shift left get carry and convert the carry to ascii then store in Reserved words
				lsla 
				staa temp,sp
				bcs Carry1
				bcc Carry0
		
					Carry1:
					ldaa #$31
					staa 1,y+
					dex
					beq DoneSpeed
					ldaa temp,sp
					bra convertToBin
			
					Carry0:
					ldaa #$30
					staa 1,y+		
					dex
					beq DoneSpeed
					ldaa temp,sp	
					bra convertToBin

	
	DoneSpeed:
	leas localVar,sp;deallocate local variable in stack
	rts



;convert 16bits into binary and store it in allocated memory 			
getProximity:
	leas -localVar,sp
		ldd Proximity,sp; 
		ldx #16; decrement counter
		ldy #ProximityValue; address to store the bit
		bra convertProximity



		convertProximity:;Shift left get carry and convert the carry to ascii then store in Reserved words
			lsld 
			std temp,sp
			bcs Carry1Proximity
			bcc Carry0Proximity
			
			Carry1Proximity:
			ldaa #$31
			staa 1,y+
			dex
			beq DoneProximity
			ldd temp,sp
			bra convertProximity
			
			Carry0Proximity:
			ldaa #$30
			staa 1,y+		
			dex
			beq DoneProximity
			ldd temp,sp	
			bra convertProximity


			DoneProximity:
			leas localVar,sp
			rts




#include "sci0api.asm"
#include "DP256reg.asm"