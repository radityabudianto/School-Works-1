//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017

#include "hcs12dp256.h"
#include <stdio.h>



//This file is works
//there is no main function for this file
//this file is library for assign42.c
//run assign42.s19 to check this file






//function prototype for this file
int scanKeypad(int speed);
void delay(void);



//scan input Keypad from user
//DDRP=0x0f, status of DDRP is writting, means PTP bit will activate the row of the keypad
//DDRH=0x0f, status of DDRH is reading, means PTH receive location of column everytime user press the button   

int scanKeypad(int speed)
{
	char value=0;//initiate while loop
	while(value==0)
	{
	 	
		
		//read keypad line 1(1,2,3,A)
		PTP=0x01;
		
		
		delay();
						if(PTH==0x10 && PTP==0x01) 
						 		{		
								printf("Rotate Left\n");
								delay();
								}
		
						if(PTH==0x20 && PTP==0x01) 
								{
				 				printf("value=2");
								delay();
								}
						if(PTH==0x40 && PTP==0x01) 
								{
				 				printf("value=3");
								delay();
								}
						if(PTH==0x80 && PTP==0x01) 
								{
				 				printf("Rotate Right\n");
								delay();
								}
						
		delay();
		
		
		//read keypad line 2(4,5,6,B)														 
		PTP=0x02;
		
		delay();
				 		 if(PTH==0x10 && PTP==0x02) 
						 		{
								printf("value=4");
								delay();
								}
								
						 if(PTH==0x20 && PTP==0x02) 
						 		{	
								printf("Obstacle is Close\n");
								delay();
								}
						 if(PTH==0x40 && PTP==0x02) 
						 		{	
								printf("value=6");
									  delay();
								}
								
						 if(PTH==0x80 && PTP==0x02) 
						 		{	
								printf("value=B");
								delay();
								}
								
		delay();
		
		
		//read keypad line 3(7,8,9,C)
		PTP=0x04;
		delay();
		
				 		 if(PTH==0x10 && PTP==0x04) 
						 		{		
								printf("value=7");
								delay();
								}
								
						 if(PTH==0x20 && PTP==0x04) 
						 		{		
								printf("value=8");
								delay();
								}
						 if(PTH==0x40 && PTP==0x04) 
						 		{		
								printf("value=9");
								delay();
								}
								
						 if(PTH==0x80 && PTP==0x04) 
						 		{	
								printf("value=C");
								delay();
								}
								
								
		delay();
		
		
		//read keypad line 3(E,0,F,D)
		PTP=0x08;
		delay();
		
				 		 if(PTH==0x10 && PTP==0x08) 
						 		{		
								printf("Accelerate\n");
								delay();
								return speed+1;//increment speed when accelerate
								
								}
								
						 if(PTH==0x20 && PTP==0x08) 
						 		{	
								printf("Exit");
								delay();
								return speed;
								
								}
						 if(PTH==0x40 && PTP==0x08) 
						 		{		
								printf("Power on/off");
								delay();
								}
								
						 if(PTH==0x80 && PTP==0x08) 
						 		{		
								printf("Decelerate\n");
								delay();
								return speed-1;//decrement speed when decelerate
								}
		delay();
		
	}
	return speed;
} 


//delay for keypad reading to isolate each button properly 
void delay()
{	 int i;
 	 for(i=0;i<10000;i++);
}

