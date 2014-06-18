//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017
#include <hcs12dp256.h>
#include "stdio.h"
#include "vectors_dp256_NoICE.c"



//initialize global variable
char key;//save value of keypad pressed
int speed=500;//initialize speed to be displayed on LCD
char temperature=30;// initialize temperature to be displayed on LCD



//prototype for this file
void convertSpeedToChar(char *string, int number);// Convert "speed" into array 
void convertTempToChar(char *string, int number);//Convert "Temp" into array
void printString(int speedValue, int temp);// Print "Speed" + "Temp" includes its values into LCD
void KISR(void);// interrupt for keypad pressed

//prototype from basicLCD.s
void Lcd2PP_Init(void);
void LCD_instruction(char);
void LCD_display(char);

//prototype for interrupt
#pragma interrupt_handler KISR()



void main( void ) {
	int i;//initialize loop counter
	char next;//store value of keypressed

     	 		SPI1CR1=0x00;
	 			
				
				//Display initial value of speed and temperature into LCD
				Lcd2PP_Init();
				LCD_instruction(0x01);
				Lcd2PP_Init();
				printString(speed,temperature);
								 
				
				
				//initialize KISR
				key = 0xFF;//refresh value of Key into 0xFF
				DDRP |= 0x0f; // bitset PP0-3 as outputs (rows)
				DDRH &= 0x0f; // bitclear PH4..7 as inputs (columns)
				PTP =0x0f;
				PIFH = 0xFF; // Clear previous interrupt flags
				PPSH = 0xF0; // Rising Edge
				PERH = 0x00; // Disable pulldowns
				PIEH |= 0xF0; // Local enable on columns inputs
				
				DDRM=0x00;
	
				asm("cli");		
	
	for (;;){
	
			}//end for loop

}//end main



//keypad interrupt
void KISR(void) {
	

	
	PIEH = 0x00; // Local disable
	
	
	
	//get button pressed from line 1(key 1-A)
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x01;   //PP0 writting
		PTM &= ~0x08;	//U7_EN disable
	

		
			   			//option for 1 is pressed,port P is writting ,Port H reading the button pressed   
						if(PTH==0x11) 
						 		{
								printf("1\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
						//option for 2 is pressed,port P is writting ,Port H reading the button pressed   
						if(PTH==0x21) 
								{		
				 				
								printf("2\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}	
						//option for 3 is pressed,port P is writting ,Port H reading the button pressed   
						if(PTH==0x41) 
								{
				 				//localDelay();
								printf("3\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
							
								}
						//option for A is pressed,port P is writting ,Port H reading the button pressed   		
						if(PTH==0x81) 
								{
				 				//localDelay();
								printf("A\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
		
		//get button pressed from line 2(key 4-B)			
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x02;   //PP1 writting
		PTM &= ~0x08;	//U7_EN disable
		
		
			   			//option for 4 is pressed,port P is writting ,Port H reading the button pressed 
				 		 if(PTH==0x12) 
						 		{		
								printf("4\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
							//option for 5 is pressed,port P is writting ,Port H reading the button pressed 	
						 if(PTH==0x22) 
						 		{		
								printf("5\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
								//option for 6 is pressed,port P is writting ,Port H reading the button pressed 
						 if(PTH==0x42) 
						 		{		
							printf("6\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
								//option for B is pressed,port P is writting ,Port H reading the button pressed 
						 if(PTH==0x82) 
						 		{		
								printf("B\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
		
		
		//get button pressed from line 3(key 7-C)			
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x04;   //PP2 writting
		PTM &= ~0x08;	//U7_EN disable
		
			   			//option for 7 is pressed,port P is writting ,Port H reading the button pressed 
				 		 if(PTH==0x14) 
						 		{		
								printf("7\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable			
								}
						//option for 8 is pressed,port P is writting ,Port H reading the button pressed 		
						 if(PTH==0x24) 
						 		{		
								printf("8\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
							
								}
						//option for 9 is pressed,port P is writting ,Port H reading the button pressed 		
						 if(PTH==0x44) 
						 		{		
								printf("9\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
						//option for C is pressed,port P is writting ,Port H reading the button pressed 		
						 if(PTH==0x84) 
						 		{		
								printf("C\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable		
								}
								
								
		//get button pressed from line 4(key E-D)			
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x08;   //PP3 writting
		PTM &= ~0x08;	//U7_EN disable
		
		
			   			//option for C is pressed,port P is writting ,Port H reading the button pressed 
				 		 if(PTH==0x18) 
						 		{									
								printf("E\n");
								speed = speed+1;//increase speed everytime E is pressed
								
								//print new value of speed into LCD		  									
								Lcd2PP_Init();
								LCD_instruction(0x01);
								printString(speed,temperature);
								
								
								//reinitialize port M for U7_EN 
								DDRM=0x00;					
								}
						//option for 0 is pressed,port P is writting ,Port H reading the button pressed 	
						 if(PTH==0x28) 
						 		{	
								printf("0\n");
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
								
								//exit
								printf("Exit\n");
								while(1)
								{
								 PTP=0x00;//disable keypad
								}
								
								}
						//option for F is pressed,port P is writting ,Port H reading the button pressed 		
						 if(PTH==0x48) 
						 		{		
								printf("F\n");							
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
								}
						//option for D is pressed,port P is writting ,Port H reading the button pressed 		
						 if(PTH==0x88) 
						 		{		
								printf("D\n");
							
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
								
								 if(speed==0)
								  {
					  			   			 printf("Minimum speed limit is 0");
								   }
							   else
							    {
				 	   	  	   				speed=speed-1;//decrease speed
			
											//print new value of speed into LCD
											Lcd2PP_Init();
											LCD_instruction(0x01);
											printString(speed,temperature);
											
											//reinitialize DDRM for keypad
											DDRM=0x00;							
								}
								
								
								
								}
				
				
				//reinitialize interrupt for KISR				
				DDRP |= 0x0f; // bitset PP0-3 as outputs (rows)
				DDRH &= 0x0f; // bitclear PH4..7 as inputs (columns)
				PTP =0x0f;//write into all lines PP0-PP3 
				PIFH = 0xFF; // Clear previous interrupt flags
				PPSH = 0xF0; // Rising Edge
				PERH = 0x00; // Disable pulldowns
				PIEH |= 0xF0; // Local enable on columns inputs
}//end KISR



//convert value of Speed into char
//contains 4 digit value of speed that is splitted into 4 separate char and store it in array
void convertSpeedToChar(char *string, int number)
{
 	 int i=3;
 	 for(i;i>=0;i--)
	 {
	      string[i]=(number%10)+48;
		  number=number/10;
	 }

}


//convert value of temperature into char
//contains 2 digit value of speed that is splitted into 2 separate char and store it in array
void convertTempToChar(char *string, int number)
{
 	 int i=1;
	 for(i;i>=0;i--)
	 {
	      string[i]=(number%10)+48;
		  number=number/10;
	 }
}


//print array of character
//all character that will be printed into LCD will call this function
void printString(int speedValue, int temp){
	 
	 //All words below will be printed into LCD
	 char *speed="Speed: "; 
	 char *temperature="Temp: ";
	 char *kmperhour="km/h";
	 char *celsius="degrees";
	 
	 int i=0;//intitialize for loop
	 
	 //store digit of speed
	 char stringSpeed[4];
	 
	 //store digit of temperature
	 char stringTemp[2];
	 
	
	//fill stringSpeed[4] with value of speed
	 convertSpeedToChar(stringSpeed,speedValue);
	//fill stringTemp[2] with value of temperature
	 convertTempToChar(stringTemp,temp);
	 
	 
	 
	 
	 
//Start display into LCD


	//Initialize LCD	 
	 Lcd2PP_Init();
	 ;
	 
	 //print "Speed: ****km/h"
	 for(i=0;i<7;i++)
	 {
	   LCD_display(speed[i]);
	 }
	 
	 for(i=0;i<4;i++)
	 {
	  LCD_display(stringSpeed[i]);
	 }	
	 for(i=0;i<4;i++)
	 {
	    LCD_display(kmperhour[i]);
	 }
	 
	 
	 //moves next line of LCD
	 LCD_instruction(0xc0);
	 
	 
	  //print "Temp: **degrees"
	 for(i=0;i<6;i++)
	 {
	   LCD_display(temperature[i]);
	 }
	 for(i=0;i<2;i++)
	 {
	  	LCD_display(stringTemp[i]);
	 }
	 for(i=0;i<7;i++)
	 {
	   LCD_display(celsius[i]);
	 }

}
