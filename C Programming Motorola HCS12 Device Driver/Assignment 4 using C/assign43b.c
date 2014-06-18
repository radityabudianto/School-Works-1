//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017

//This part works in HCS12 machine
//Press '5' to execute delay 3 second, rotate step motor 90 degree clockwise, delay 2 second, and continue straight

#include "hcs12dp256.h"
#include "vectors_dp256_NoICE.c"
#include <stdio.h>


#pragma interrupt_handler RTI()


//prototype for all function in assign43b
void RTI(void);//Real Time Interrupt tick
void scanKeypad(void);//Keypad input
void localDelay(void);//delay for read between each character for Keypad
void anticlockwise(char n);//for activate stepper motor, each n is approximately 65 degree anticlockwise
void clockwise(char n);//for activate stepper motor, each n is approximately 65 degree clockwise
void delay(int ms);//delay in milisecond, this function is particular for clockwise and anticlockwise delay


//global variable
char RTIcounter=0;//initialize counter for RTI when '5' keypad button is pressed to count 3 second and 2 second delay
char collision = 0;// set the value to 1 when '5' keypad button is pressed  

void main(){
	SPI1CR1=0x00;
	//initialize DDR for keypad
	DDRH=0x0F;
	DDRP=0x0F;
	
	PTM=0x08;//Load data into U7 set U7_EN high (PM3)
	
	//initialize frequency for RTI	
	CRGINT|=0x80;
	RTICTL|=0x7f;	
	
	//call global interrupt for RTI
	asm("cli");	
	
	//check keypad input and if 5 is pressed, motor will rotate 90 degree clockwise
	//if 0 is pressed then "Exit" end the keypad input
	scanKeypad();
	
}//end main



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



//function of this subroutine is reading the input from keypad
//when 5 is pressed, activate collision
//when 0 is pressed, exit
void scanKeypad(void)
{

 	
	char value=0;//initiate while loop 
	
	while(value==0)
	{
	 	
		PTP=0x01;//scan button for (1,2,3,A)
	
		localDelay();
						if(PTH==0x10 && PTP==0x01) 
						 		{
								printf("Rotate Left\n");
								localDelay();
								}
		
						if(PTH==0x20 && PTP==0x01) 
								{		
				 				 printf("value=2");
								localDelay();
								}
						if(PTH==0x40 && PTP==0x01) 
								{
				 				printf("value=3");
								localDelay();
								}
						if(PTH==0x80 && PTP==0x01) 
								{
				 				 printf("Rotate Right\n");
								localDelay();
								}
						
		localDelay();
																 
		PTP=0x02;//scan button for (4,5,6,B)
		
		localDelay();
				 		 if(PTH==0x10 && PTP==0x02) 
						 		{		
								printf("value=4");
								localDelay();
								}
								
						 if(PTH==0x20 && PTP==0x02) 
						 		{		
									 printf("Obstacle is Close\n");
									 collision = 1;
								localDelay();
								}
						 if(PTH==0x40 && PTP==0x02) 
						 		{		
									 printf("value=6");
									  localDelay();
								}
								
						 if(PTH==0x80 && PTP==0x02) 
						 		{		
									 printf("value=B");
								localDelay();
								}
								
		localDelay();
		PTP=0x04;//scan button for (7,8,9,C)
		localDelay();
				 		 if(PTH==0x10 && PTP==0x04) 
						 		{		
									 printf("value=7");
								localDelay();
								}
								
						 if(PTH==0x20 && PTP==0x04) 
						 		{		
									 printf("value=8");
								localDelay();
								}
						 if(PTH==0x40 && PTP==0x04) 
						 		{		
									 printf("value=9");
									  localDelay();
								}
								
						 if(PTH==0x80 && PTP==0x04) 
						 		{		
									 printf("value=C");
								localDelay();
								}
								
								
		localDelay();
		PTP=0x08;//scan button for (E,0,F,D)
		localDelay();
		
				 		 if(PTH==0x10 && PTP==0x08) 
						 		{		
									 printf("Accelerate\n");
					
								localDelay();
								}
								
						 if(PTH==0x20 && PTP==0x08) 
						 		{	
									 printf("Exit");
									 localDelay();
									 break;
								}
						 if(PTH==0x40 && PTP==0x08) 
						 		{		
									 printf("Power on/off");
									  localDelay();
								}
								
						 if(PTH==0x80 && PTP==0x08) 
						 		{		
									 printf("Decelerate\n");
						
								localDelay();
								}
		localDelay();
	
	}
} 


//this is delay function for scanKeypad reading
void localDelay(void)
{	 int i;
 	 for(i=0;i<10000;i++);
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


	



