//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017



#include "hcs12dp256.h"
#include "stdio.h"
#include "vectors_dp256_NoICE.c"


//instruction of robot 
		//Buzzer & first LED will be on when temprature>=85'F
		//LCD will automatically refreshed to display speed and temperature every 3 second
		//heat temperature reading is done every 5 second
		//Refresh time for LCD and NoICE output are different
		
			
			//Keypad instruction
			  //'4' to turn on heater
			  //'5' to turn on obstacle
			  //'B' to turn off heater
			  //'E' to increase DC motor speed
			  //'D' to decrease DC motor speed
			  //'0' to reinitialize robot into initial condition,all LED on for a while
			  



//initialize global variable

char heaterOn=0;//when heaterOn=0, heater is off. When heaterOn=1, heater is on
char SetOptCount=0;//count number of RPS
char temperature=0;//store value of temperature from heat sensor reading
int RTIcounter;//count clock Tick of RTI, everytime RTI function is used, RTI counter will increase
int optCount=0;//count RPS dc motor
int counter=0;//another counter to count RTI clock tick, will be used for temperature checking inside RTI
char obstacle=0;//when user press 5, obstacle=1 (means obstacle is detected)
int duty=20;//duty cycle for DC motor


//prototype for this file
void convertSpeedToChar(char *string, int number);//convert speed and value of speed into char
void convertTempToChar(char *string, int number);//convert temperature and value of temperature into char
void printString(int speedValue, int temp);//display speed and temperature into LCD
void KISR(void);//interrupt for KISR
void RTI(void);//interrupt for RTI
void pacA_isr(void);//interrupt for DC motor sensor
void ATDo(void);//interrupt for Analog to Digital channel 0 in this case we use it for temperature sensor
void runMotor(void);//initialize pacA_isr 
void runRTI(void);//initialize RTI
void runTemperature(void);//initialize temperature
void runKeypad(void);//initialize KISR
void clockwise(char n);//robot direction move clockwise
void anticlockwise(char n);//robot direction move anticlockwise

//from basicLCD.s
void Lcd2PP_Init();//initialize LCD
void LCD_display(char);//display to LCD
void LCD_instruction(char);//command for LCD instruction



//for compiler to use interrupt_handler in HCS12
#pragma interrupt_handler RTI()//Priority1 
#pragma interrupt_handler KISR()//Priority2
#pragma interrupt_handler pacA_isr()//Priority3
#pragma interrupt_handler ATDo()//Priority4





void main( void ) {
	 SPI1CR1=0x00;
	
	//initialize LCD, RTI,KISR,pac_isr,and ATDo interrupt
	Lcd2PP_Init(); 			
	runKeypad();
	runMotor();
	runTemperature();
	runRTI();
	 
	//activate interrupt 
	INTR_ON(); 

	//infinite loop to run program forever
	for (;;){
	
			}//end for loop

}//end main




//keypad interrupt
void KISR(void) {
	 
	PIEH = 0x00; // Local disable
	 	
	PTM |= 0x08;	//U7_EN enable
	PTP = 0x01;   //Write into line1(PP0)
	PTM &= ~0x08; //U7 disable		
	
	if(PTH==0x11) 
	{
	    printf("1\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}
	if(PTH==0x21) 
	{		
	    printf("2\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
		}
	if(PTH==0x41) 
	{
		printf("3\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}
	if(PTH==0x81) 
	{
		printf("A\n");
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x0f;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;
	}
		
	PTM |= 0x08;	//U7_EN enable
	PTP = 0x02;   //Write into line2(PP1)
	PTM &= ~0x08; //U7 disable	
	if(PTH==0x12) 
	{		
		printf("4, Heater On\n");
		PTM |= 0x08;	//Load data into U7 set U7_EN high (PM3)
		PTP = 0x0f;   //Write Key1..4 via PP0..3
		PTM &= ~0x08;	
		heaterOn=1;
	}		
	if(PTH==0x22) 
	{		
		printf("5\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable						
		obstacle=1;
	}
	if(PTH==0x42) 
	{		
		printf("6\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}
	if(PTH==0x82) 
	{		
		printf("B,Heater Off\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable								
		heaterOn=0;
	}

	PTM |= 0x08;	//U7_EN enable
	PTP = 0x04;   //Write into line3(PP2)
	PTM &= ~0x08; //U7 disable	
	if(PTH==0x14) 
	{		
		printf("7\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}						
	if(PTH==0x24) 
	{		
		printf("8\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}
	if(PTH==0x44) 
	{		
		printf("9\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}					
	if(PTH==0x84) 
	{		
		printf("C\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}

	PTM |= 0x08;	//U7_EN enable
	PTP = 0x08;   //Write into line4(PP3)
	PTM &= ~0x08; //U7 disable	
	if(PTH==0x18) 
	{		
		printf("E\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
		duty++;
		if(duty>=0)
		{
		    PWMDTY7 = duty;
		}
		DDRM=0x00;
		PTM=0x08;
		PTM=0x00;			
	}						
	if(PTH==0x28) 
	{	
		printf("0\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
		//exit
		printf("Restart\n");		
		
		//all LED is on
		DDRK=0xff;
		PORTK=0x0f;
					
		//refresh value of all global variable into initial value
		heaterOn=0;
		SetOptCount=0;
		temperature=0;
		RTIcounter;
		optCount=0;//count RPS dc motor
		
		counter=0;		
		obstacle=0;
		duty=20;				
	}
	if(PTH==0x48) 
	{
		printf("F\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable		
	}						
	if(PTH==0x88) 
	{		
		printf("D\n");
		PTM |= 0x08;	//U7_EN enable
		PTP = 0x0f;   //Write into all line(PP0-PP3)
		PTM &= ~0x08; //U7 disable				
		if(optCount==0)
		{
			printf("Minimum speed limit is 0");
		}
		else
		{
			duty--;
			DDRM=0x00;
	   		
		}
	}
	runKeypad();
}


	


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
 	 int i=2;
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
	 char *kmperhour="RPS";
	 char *celsius="'F";
	 
	 int i=0;//intitialize for loop
	 
	 //store digit of speed
	 char stringSpeed[4];
	 
	 //store digit of temperature
	 char stringTemp[3];
	 
	
	//fill stringSpeed[4] with value of speed
	 convertSpeedToChar(stringSpeed,speedValue);
	//fill stringTemp[2] with value of temperature
	 convertTempToChar(stringTemp,temp);
	 
	 
	 
	 
	 
//Start display into LCD


	//Initialize LCD	 
	 Lcd2PP_Init();
	 
	 //print "Speed: ****RPS"
	 for(i=0;i<7;i++)
	 {
	   LCD_display(speed[i]);
	 }
	 
	 for(i=0;i<4;i++)
	 {
	  LCD_display(stringSpeed[i]);
	 }	
	 for(i=0;i<3;i++)
	 {
	    LCD_display(kmperhour[i]);
	 }
	 
	 
	 //moves next line of LCD
	 LCD_instruction(0xc0);
	 
	 
	  //print "Temp: ***'F"
	 for(i=0;i<6;i++)
	 {
	   LCD_display(temperature[i]);
	 }
	 for(i=0;i<3;i++)
	 {
	  	LCD_display(stringTemp[i]);
	 }
	 for(i=0;i<2;i++)
	 {
	   LCD_display(celsius[i]);
	 }
	 

}

//1 ms delay
void delay (int ms){

	 const int delayConstant = 1234;
	 int i, j;
	 
	 for (i=0; i<ms; i++) {
	 	 for (j=0; j<delayConstant; j++) {
		 }
	}
	

	
}


void RTI()

{	
	int temp;//temporary storage
	
	
	
	//4 tick approximately 1 second
	counter++;//do clock ticking  
	SetOptCount++;//do clock ticking
	
	
	
	//obstacle when user press '5'
	if(obstacle==1)
	{
		RTIcounter++; //start count the RTI
		
		//Initialize 3 second delay
		if(RTIcounter==1)
		{
				duty=0;
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
		 		duty=20;
				printf("Continue straight\n");
		
				PWMDTY7 = duty;
				RTIcounter=0;//reset RTI timer set to 0
				obstacle=0;//reset collision to 0
				// clear RTI flag
		}
	}//end obstacle
	
	
	
	//when heater is on, first LED from top is on and turn heater on
	if(heaterOn==1)
	{	
	
		//heater on		   
		DDRM=0xff;
		PTM=0x81;//heater on
		delay(100);//delay 100ms allow heater to operate
		
		//LED on
		DDRK=0xff;
		PORTK=0x01;
		
		
	}
	
	else
	{
	 	//heater off
	 	DDRM=0xff;
		PTM=0x00;//heater off
		
		//first LED from top off
		DDRK=0xff;
		PORTK=0x00;
	}
	
	
	//for RTI, 4 tick is equal to 1 second
	if(SetOptCount==4)
	{
	optCount=0;
	}

	
	//refresh LCD every 3 second
	if(SetOptCount==12)
	{	
		//refresh LCD, then print motor speed and temperature			  
		LCD_instruction(0x00);						
		Lcd2PP_Init();
		printString(optCount,temperature);		
		SetOptCount=0;//reset RPS counter to 0
	}
	
	//get temperature every 5 second
	if(counter==20)
	{ 
	  //initialize interrupt for temperature sensor
	  runTemperature();
	
	  			//if temperature over 85, buzzer on, overheat LED on & turn off heater
				if(temperature>=85)
				{		//turn on buzzer
				 		DDRK=0xff;
						PORTK=0x22;
						delay(100);//give time to buzzer to operate
						heaterOn=0;
						
						//turn off buzzer
						PORTK=0x00;
						DDRK=0x00;
						
						//heater off automatically
						DDRM=0xff;
						PTM=0x00;
						
				}
	  				
						counter=0;//reset counter for RTI
	}
	 	DDRM=0x00;//set DDRM on, to read  from keypad 
		CRGFLG=CRGFLG;//Acknowledge interrupt
											 
}

//initialize RTI
void runRTI()
{
 	CRGINT|=0x80;
	RTICTL|=0x7f;	
}

//initialize interrupt for temperature sensor
void runTemperature()
{
 	ATD0CTL2=0xfA;
	ATD0CTL3=0x00;
	ATD0CTL4=0x60;
	ATD0CTL5=0x86;
	
	DDRM=0xff;//DDRM writting 

}

//initialize KISR
void runKeypad()
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
		
//initialize DC motor rotation sensor
void runMotor()
{	  int duty;

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

//function for rotation counting
void pacA_isr(void) {
	 INTR_OFF();
	 PTP=0x4f;	
 	 PWMDTY7 = duty;//duty cycle of DC motor
	 optCount++;
	 PAFLG |= 1;
	 INTR_ON();
}

//function for get value from temperature sensor
void ATDo(void){		
	 INTR_OFF();//deactivate global interrupt during temperature reading
	 
	 	ATD0CTL5=0x86;//new conversion
	 
		ATD0DR0 &=0x03ff;//10 bit reading from ATD0 data register 0
	
		temperature=ATD0DR0/8-5;//get temperature in farenheit
		
		delay(10);//delay 10 ms for heater to change temperature
		

	
		
		//turn off ATD0 controller
		ATD0CTL2=0x00;
		ATD0CTL3=0x00;
		ATD0CTL4=0x00;
		ATD0CTL5=0x00;
		
		//reinitialize DDRM to 0x00 to activate KISR
		DDRM=0x00;
		
	INTR_ON();//activate global interrupt		
}


//each n will rotate stepper motor 90 degree anticlockwise
void anticlockwise(char n)
{
 	int i;
	DDRT=0xff;
	DDRP=0xff;
	PTP=0b00100000;	
	for(i=0;i<=n;i++)
	{
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
	for(i=0;i<=n;i++)
	{
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
