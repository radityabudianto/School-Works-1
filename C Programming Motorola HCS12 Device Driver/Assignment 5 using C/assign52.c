//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017

#include <hcs12dp256.h>
#include <stdio.h>
#include "vectors_dp256_NoICE.c"

//prototype for interrupt
#pragma interrupt_handler pacA_isr()
#pragma interrupt_handler KISR()
#pragma interrupt_handler RTI()


//global variable
int optCount = 0;//count number of RPS per second
char key=0xff;//store value of keypressed from KISR
int printRPS=0;//store value of RPS to be printed into LCD
int RTIcounter=0;//counter for timer in RTI
int duty;//set value of duty cycle
int n=0;//change value of n to increase or decrease duty cycle when 'E' or 'D' is pressed




//function parameter for all function in this file
void KISR(void);//KISR
void RTI(void);//RTI
void pacA_isr(void);//DC motor sensor
void initDC(void);//initialize pacA_isr
void initRTI(void);//initialize RTI
void initKISR(void); //initialize KISR




void main(void) {
	 SPI1CR1=0x00;
	
	//function for initialize pac_ISR,RTI, and KISR
	initDC();
	initRTI();
	initKISR(); 
	
	
	//activate interrupt
	INTR_ON();
	 
	//infinite while loop to make program run forever
	 while(1) 		
	 {	
			 
	 }//end while	

}//end main



//interrupt for DC motor rotation checker sensor
void pacA_isr(void) {
	 INTR_OFF();//turn off interrupt when pacA_isr is running
	 optCount++;//counter for rotation
	 PAFLG |= 1;//clear out interrupt flag
	 INTR_ON();//reactivate interrupt
}//end pacA_isr



//Real Time Interrupt
void RTI(void){
	 RTIcounter++;
	 
	 
	 //for 4 tick of RTI is equal to 1 second, print RPS
	 if(RTIcounter==4)
	 {
	  	printf("%d RPS\n",optCount);
			if(optCount<10) n=n+1;//increase speed if RPS<10
			if(optCount>=30)n=n-1;//decrease speed if RPS>30
		
		RTIcounter=0;
		optCount=0;
	 }
		
		
	
		//activate DC motor
	  PTP = 0x4f; // Counter-Clockwise motor direction
	  for(duty = 20+n; duty <= 30+n; duty++)
	  {
		 		 PWMDTY7 = duty;
	  }

		
		
	  //if '0' is pressed, turn off motor	 
	  if(key=='0') 
		  {
		   			   printf("Turn off\n");
					   
					   // turn off motor and keypad
		   			   PWMDTY7=0; 
	 				   PTP=0x00;
					   
					   while(1){}
		  }

	CRGFLG=CRGFLG;//Acknowledge interrupt
}


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
								printf("E, increase duty cycle of motor\n");
								n=n+1;//increase speed everytime E is pressed
								
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
								}
						//option for 0 is pressed,port P is writting ,Port H reading the button pressed 	
						 if(PTH==0x28) 
						 		{	
								printf("0, Turn off motor and keypad\n");
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
								printf("D, decrease duty cycle of motor\n");
							
								PTM |= 0x08;	//U7_EN enable
								PTP = 0x0f;   //Write into all line(PP0-PP3)
			  		  		  	PTM &= ~0x08; //U7 disable	
								
								n=n-1;
								
								
								}
				
				
				//reinitialize interrupt for KISR				
				initKISR();
}//end KISR


//initialize DC motor
void initDC()
{
//DC motor initialization
	 PWMPOL = 0xFF; // Initial Polarity is high
	 PWMCLK &= 0x7F; //Select Clock B for channel 7
	 PWMPRCLK = 0x70; //Prescale ClockB : busclock/128
	 PWMCAE &= 0x7F; //Channel 7 : left aligned
	 PWMCTL &= 0xF3; //PWM in Wait and Freeze Modes
	 PWMPER7 = 100; //Set period for PWM7
	 PWME = 0x80; //Enable PWM Channel 7
	 PAFLG |= 1; //Clear out the interrupt flag
	 PACTL = 0x51; //Enable PACA for Optical Sensor
}
//initialize KISR
void initKISR()
{
	
	//KISR initialization
	DDRP |= 0x4f; // bitset PP0-3 as outputs (rows) & motor direection control
	DDRH &= 0x0f; // bitclear PH4..7 as inputs (columns)
	PIFH = 0xFF; // Clear previous interrupt flags
	PPSH = 0xF0; // Rising Edge
	PERH = 0x00; // Disable pulldowns
	PIEH |= 0xF0; // Local enable on columns inputs
	PTP = 0x4f; // Counter-Clockwise & activate keypad port P
	
}

//initialize RTI

void initRTI()
{
 //RTI initialization
	 CRGINT|=0x80;
	 RTICTL|=0x7f;	
}
