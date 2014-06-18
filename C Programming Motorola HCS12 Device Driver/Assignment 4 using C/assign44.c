//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017

//Keypad ISR and RTI working

#include "hcs12dp256.h"
#include "stdio.h"
#include "vectors_dp256_NoICE_for PArt_5.c"
#include "assign42.c"

#pragma interrupt_handler KISR()
#pragma interrupt_handler RTI()

void KISR(void);
void localDelay(void);
void RTI(void);
void anticlockwise(char n);
void clockwise(char n);
void delay (int ms);


void Lcd2PP_Init(void);//initialize LCD
void LCD_display(char);//Display to LCD
void LCD_instruction(char);//adjust the menu of LCD

char key;
char RTIcounter=0;
char collision=0;






void main( void ) {
	char waiting=1; 
	char next;
	key = 0xFF;
	DDRP |= 0x0f; // bitset PP0-3 as outputs (rows)
	DDRH &= 0x0f; // bitclear PH4..7 as inputs (columns)
	PTP =0x0f;
	PIFH = 0xFF; // Clear previous interrupt flags
	PPSH = 0xF0; // Rising Edge
	PERH = 0x00; // Disable pulldowns
	PIEH |= 0xF0; // Local enable on columns inputs
	asm("cli");
	for (;;){
			   	while (key == 0xFF );
				next = key;
				key = 0xFF;
				printf ( "%c\n", next );
				if(next=='5')
				{			 			 
			
			 							 CRGINT|=0x80;
										 RTICTL|=0x7f;
										 asm( "cli" );
	 									 collision=1;
					
					 }
			
			
			}//end for loop

}//end main

void KISR(void) {
	
	
	char value=0;//initiate while loop 
	
	PIEH = 0x00; // Local disable
	
	PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
	PTP = 0x00;   //Write Key1..4 via PP0..3
	PTM &= ~0x08;	//set U7_EN low (PM3)
	
	while(value==0)
	{
	 	
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x01;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;	//set U7_EN low (PM3)
		localDelay();
						if(PTH==0x11) 
						 		{
							
								localDelay();
								key='1';
								
								Lcd2PP_Init();//initialize LCD
								LCD_display('1');//Display to LCD
								
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;	
								}
		
						if(PTH==0x21) 
								{		
				 				localDelay();
								key='2';
								
								//Lcd2PP_Init();//initialize LCD
								//LCD_display('2');//Display to LCD
								
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;	
								}
						if(PTH==0x41) 
								{
				 				localDelay();
								key='3';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;	
								}
						if(PTH==0x81) 
								{
				 				localDelay();
								key='A';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;	
								}
						
		localDelay();
		
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x02;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;	//set U7_EN low (PM3)
		localDelay();
				 		 if(PTH==0x12) 
						 		{		
								localDelay();
								key='4';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
						 if(PTH==0x22) 
						 		{		
								localDelay();
								key='5';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
						 if(PTH==0x42) 
						 		{		
								localDelay();
								key='6';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
						 if(PTH==0x82) 
						 		{		
								localDelay();
								key='B';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
							
		localDelay();
		
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x04;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;	//set U7_EN low (PM3)
		localDelay();
				 		 if(PTH==0x14) 
						 		{		
								localDelay();
								key='7';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
						 if(PTH==0x24) 
						 		{		
								localDelay();
								key='8';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
						 if(PTH==0x44) 
						 		{		
									localDelay();
								key='9';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;;
								}
								
						 if(PTH==0x84) 
						 		{		
									localDelay();
								key='C';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
								
		localDelay();
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x08;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;	//set U7_EN low (PM3)
		localDelay();
		
				 		 if(PTH==0x18) 
						 		{		
								localDelay();
								key='E';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
						 if(PTH==0x28) 
						 		{	
								localDelay();
								key='0';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
						 if(PTH==0x48) 
						 		{		
								localDelay();
								key='F';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
								
						 if(PTH==0x88) 
						 		{		
								localDelay();
								key='D';
								PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
								PTP = 0x0f;   //Write Key1..4 via PP0..3
			  		  		  	PTM &= ~0x08;
								break;
								}
		localDelay();
	
	}
	
	PIFH = PIFH; // Acknowledge (all) interrupts
	PIEH |= 0xF0; // Local enable on columns inputs
}

	

//this is delay function for scanKeypad reading
void localDelay(void)
{	 int i;
 	 for(i=0;i<10000;i++);
}

//Activate Real Time Interrupt(RTI)
//when '5' in keypad is pressed, activate the RTIcounter to start count (4 tick of RTIcounter = 1 second)

void RTI(void)
{
	//when 5 is pressed
	if(collision==1)
	{
		RTIcounter++; //start count the RTI
	 	
		
		//Initialize 3 second delay
		if(RTIcounter==1)
		{
				 printf("DC motor off, Delay 3 second\n");
		}
		
		//After 3 second Rotate the stepper motor to the right
		//(12 RTI counter= 3 second)
		if(RTIcounter==12)
		{
				 printf("Rotate 90 degree right\n");					
				 clockwise(1);//assume its rotate 90 degree right
				 printf("wait 2 second\n");
		}
		
		//After 5 second reset RTI counter and collision value to 0, then continue straight
		//(20 RTI counter = 5 second)
		if(RTIcounter==20)//
		{
				 printf("Continue straight\n");
				 RTIcounter=0;//reset RTI timer set to 0
				 collision=0;//reset collision to 0
				 // clear RTI flag
		}
	}
	CRGFLG=CRGFLG;//Acknowledge interrupt
}

//each n will rotate stepper motor 90 degree anticlockwise
void anticlockwise(char n)
{
int i;
DDRT=0xff;
DDRP=0xff;
PTP=0b00100000;	
 for(i=0;i<=n;i++){
 PTT=0b01000000; 
delay(10);
 PTT=0b01100000; 
delay(10);
 PTT=0b00100000; 
delay(10);
 PTT=0b00000000; 
delay(10);
		  		 }
}



//each n will rotate stepper motor 90 degree clockwise
void clockwise(char n)
{
int i;
DDRT=0xff;
DDRP=0xff;
PTP=0b00100000;	
	
for(i=0;i<=n;i++){
 PTT=0b00000000; 
delay(10);
 PTT=0b00100000; 
delay(10);
 PTT=0b01100000; 
delay(10);
 PTT=0b01000000; 
delay(10);
		  		 }
}

//each ms is equal 1 milisecond
void delay (int ms){

	 const int delayConstant = 1234;
	 int i, j;
	 
	 for (i=0; i<ms; i++) {
	 	 for (j=0; j<delayConstant; j++) {
		 }
	}
	
}