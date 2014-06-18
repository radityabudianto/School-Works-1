;This program works in HC12 according to test function and the expected result





DDRK equ $33 ;port k data direction register
portk equ $32 ; port k data register
TestValue equ 2;Distance of TestValue is +2 of SP

	org $4000

main:

	
	lds #$3dff
	;Test Obstacle=1,Direction=000(North), Power=0(Power off)
	; Expected result is buzzer on, LED off
	ldd #%1000000000000001
	pshd
	
	jsr execute	
	puld	


;set extra delay for the test function
	movb #$00,portk ;clear port k
	JSR setDelay ;get Delay




	;Test Obstacle=1,Direction=001(South), Power=1(Power on)
	; Expected result is buzzer on, LED2 on
	ldd #%1000000001000001
	pshd
	jsr execute	
	puld	
	;


;set extra delay for the test function
	movb #$00,portk ;clear port k
	JSR setDelay ;get Delay




	;Test Obstacle=1,Direction=001(east), Power=1(Power on)
	; Expected result is buzzer on, LED3 on
	ldd #%1000000001000010
	pshd
	jsr execute	
	puld	



;set extra delay for the test function
	movb #$00,portk ;clear port k
	JSR setDelay ;get Delay



	;Test Obstacle=1,Direction=001(West), Power=1(Power on)
	; Expected result is buzzer on, LED4 on
	ldd #%1000000001000011
	pshd
	jsr execute	
	puld	




;set extra delay for the test function
	movb #$00,portk ;clear port k
	JSR setDelay ;get Delay


	;Test Obstacle=0,Direction=001(South), Power=1(Power on)
	; Expected result is buzzer off, LED2 on
	ldd #%00000000001000001
	pshd
	jsr execute	
	puld	
	

	

	swi





;Sequence of program
;check obstacle
;if obstacle=1, set buzzer on, set delay for buzzer, then check Power(bit6)
;if obstacle=0, checkPower
		;if power=0, return to caller
		;if power=1, check direction
			;isolate bit 0-2 to get the direction
			;if direction is N or S or W or E, turn on the LED according to direction
			;set the delay for LED,l then return
			

execute:

	;check obstacle 1 or 0
	;when its 1, buzzer on
	;when its 0, check motor
	

	checkObstacle:				
		ldd TestValue,sp;location of TestValue
		lsld
		bcs Obstacle1
		
		bra checkPower ;LED is off when motor is off, I assume motor=power

		;when obstacle is 1 turn on buzzer
		Obstacle1:
	
			movb #$ff,DDRK;setup port K 
			movb #$00,portk ;clear port k
			bset portk,#$20 ;buzzer on
		
			ldx #$ffff;set counter to make delay
			JSR setDelay ;get Delay

			;set extra delay for the test function
			movb #$00,portk ;clear port k
			JSR setDelay ;get Delay
			bra checkPower
			
	;when power is off, all LED off

	checkPower:
		
		ldd TestValue,sp
		;get bit 6 and check it
		lsrd
		lsrd
		lsrd
		lsrd
		lsrd
		lsrd
		lsrd
		;lsrd
	
		
		bcs PowerIs1
		
		rts;means power is off and return

			PowerIs1:
			
			
			ldd TestValue,sp
			
			;clean Register A
			;use register B for processing the first 8 bits
			;becomes 00000xxx
			
			ldaa #0	
			
			lslb
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
			ldaa #0
			;direction is North, turn on LED 1
			cmpb #0
			beq LED1
			;direction is South, turn on LED 2
			cmpb #1
			beq LED2
			;direction is East, turn on LED 3
			cmpb #2
			beq LED3
			;direction is West, turn on LED 4
			cmpb #3
			beq LED4
			
	



			
				LED1:
					movb #$ff,DDRK;setup port K 
					movb #$00,portk ;clear port k
					bset portk,#%00000001 ;buzzer on
					ldx #$ffff;set counter to make delay
					JSR setDelay
					rts ;Done exit to main
							
				LED2:
					movb #$ff,DDRK;setup port K 
					movb #$00,portk ;clear port k
					ldx #$ffff;set counter to make delay
					bset portk,#%00000010 ;buzzer on
					JSR setDelay
					rts ;Done exit to main

						
				LED3:
					movb #$ff,DDRK;setup port K 
					movb #$00,portk ;clear port k
					bset portk,#%00000100 ;buzzer on
					ldx #$ffff;set counter to make delay
					JSR setDelay
					rts ;Done exit to main
							
				LED4:
					movb #$ff,DDRK;setup port K 
					movb #$00,portk ;clear port k
					bset portk,#%00001000 ;buzzer on
					ldx #$ffff;set counter to make delay
					JSR setDelay
					rts ;Done exit to main
					
			

	;get delay from push and pull process in memory

	
	setDelay:
	pshy
	puly
	

	dex
		
	cpx #0
	beq retDelay; Return to main program
	
	bra setDelay;au	
;
		
	
		;Return to PowerIs1
		retDelay:
		movb #$00,portk ;clear port k

		rts



	
	
