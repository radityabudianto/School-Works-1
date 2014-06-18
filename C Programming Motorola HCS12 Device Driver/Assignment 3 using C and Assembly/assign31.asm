;This program works and display the expected value from the input 

	org $1000

;Declare all the variable that will be used for printing

;Use 8 bit input
StringSpeed:fcc " Speed:"
SpeedValue rmb 8
enter1:db $0A
TerminateSpeed: db 0

;Use 16bit input
StringTemperature: fcc " Temperature: "
TemperatureValue rmb 8
enter2:db $0A
TerminateTemperature: db 0

;Use 16bit input
StringKeyPressed: fcc " KeyPressed: "
KeyPressedValue rmb 16
enter3: db $0A
TerminateKeyPressed: db 0

;Use8 bit input
StringProximity: fcc" Proximity: "
ProximityValue rmb 8
enter4:db $0A
TerminateProximity: db 0

;Distance with top of the stack
Speed equ 8
Temperature equ 7
KeyPressed equ 5
Proximity equ 4
localVar equ 2
temp equ 0

ReturnValue rmb 1;return value of the subroutine
	





		org $4000

Start:
	;initialize SP
	lds #$3dff
	
	
;Push all the stack to follow the subroutine policies and locating the pointer
	;initialize speed parameter and store push it in stack
	lda #200
	psha

	;initialize temperature parameter and push it in stack
	ldb #48
	pshb
	
	;initialize Keypressed parameter and push it in stack
	ldx #%1011111111111110
	pshx


	;initialize proximity parameter and push it in stack
	lda #11
	psha

	

	jsr displaySystemStatus
	

	;rebalance the push from top to clean the stack
	pula
	pulx
	pulb
	pula

	swi
	

displaySystemStatus:
	
; declare local variable, new top address of SP will be in SP-2	
	leas -localVar,sp 

	
	;Store speed into register A
	;Convert it into binary
	;Store it in allocated memory starting from #SpeedValue
	;Print the output
		getSpeedBit:
			ldaa speed,sp; 
			ldx #8; decrement counter
			ldy #SpeedValue; address to store the bit
			bra convertToBin
			
			convertToBin:;Shift left, get carry and convert the carry to ascii then store in memory starting from #SpeedValue
				lsla 
				staa temp,sp
				bcs Carry1
				bcc Carry0
		
					Carry1:
					ldaa #$31
					staa 1,y+
					dex
					beq prtSpeed
					ldaa temp,sp
					bra convertToBin
			
					Carry0:
					ldaa #$30
					staa 1,y+		
					dex
					beq prtSpeed
					ldaa temp,sp	
					bra convertToBin

						prtSpeed:
						LDD	#BAUD19K		;program SCI0's baud rate
						JSR	setbaud
		
						ldy #StringSpeed
	 					JSR putStr_sc0

		

	;get Temperature
	;convert Temperature into Bin and Store it in allocated memory address #TemperatureValue
	; Print the Temperature	
	getTemperatureBit:
		ldaa Temperature,sp; 
		ldx #8; decrement counter
		ldy #TemperatureValue; address to store the bit
		bra convertTemperatureToBin



		convertTemperatureToBin:;Shift left get carry and convert the carry to ascii then store in Reserved words
			lsla 
			staa temp,sp
			bcs Carry1forTemperature
			bcc Carry0forTemperature
			
			Carry1forTemperature:
			ldaa #$31
			staa 1,y+
			dex
			beq prtTemperature
			ldaa temp,sp
			bra convertTemperatureToBin
			
			Carry0forTemperature:
			ldaa #$30
			staa 1,y+		
			dex
			beq prtTemperature
			ldaa temp,sp	
			bra convertTemperatureToBin



				prtTemperature:

				
				LDD	#BAUD19K		;program SCI0's baud rate
				JSR	setbaud
	
				ldy #StringTemperature
				JSR putStr_sc0


		
		;get Proximity
		;convert Proximity into Bin and Store it in allocated memory address #ProximityValue
		; Print the Proximity

		getProximity:
		ldaa Proximity,sp; 
		ldx #8; decrement counter
		ldy #ProximityValue; address to store the bit
		bra convertProximity



		convertProximity:;Shift left get carry and convert the carry to ascii then store in Reserved words
			lsla 
			staa temp,sp
			bcs Carry1Proximity
			bcc Carry0Proximity
			
			Carry1Proximity:
			ldaa #$31
			staa 1,y+
			dex
			beq prtProximity
			ldaa temp,sp
			bra convertProximity
			
			Carry0Proximity:
			ldaa #$30
			staa 1,y+		
			dex
			beq prtProximity
			ldaa temp,sp	
			bra convertProximity



				prtProximity:

				
				LDD	#BAUD19K		;program SCI0's baud rate
				JSR	setbaud
	
				ldy #StringProximity
				JSR putStr_sc0



	;get KeyPressedBit
	;convert KeyPressedBit into Bin and Store it in allocated memory address #KeyPressedBitValue
	; Print the KeyPressedBit


	getKeyPressedBit:
		ldd KeyPressed,sp; 
		ldx #16; decrement counter
		ldy #KeyPressedValue; address to store the bit
		bra convertKeyPressedToBin

		convertKeyPressedToBin:;Shift left get carry and convert the carry to ascii then store in Reserved words
			lsld 
			std temp,sp
			bcs Carry1forKeyPressed
			bcc Carry0forKeyPressed
			
			Carry1forKeyPressed:
			ldaa #$31
			staa 1,y+
			dex
			beq prtKeyPressed
			ldd temp,sp
			bra convertKeyPressedToBin
			
			Carry0forKeyPressed:
			ldaa #$30
			staa 1,y+		
			dex
			beq prtKeyPressed
			ldd temp,sp	
			bra convertKeyPressedToBin


				prtKeyPressed:		
				LDD	#BAUD19K		;program SCI0's baud rate
				JSR	setbaud
	
				ldy #StringKeyPressed
				JSR putStr_sc0



	;Close local Variable
	leas localVar,sp
	puld

	;set Return Value
	ldab #1
	stab ReturnValue

	rts;return to caller


#include "sci0api.asm"
#include "DP256reg.asm"


	
 