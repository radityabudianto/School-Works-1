//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017



#include "stdio.h"
#include "hcs12dp256.h"
#include "vectors_dp256_NoICE.c"


//tell compiler to use interrupt handler
#pragma interrupt_handler ATD0()


//function parameter for this file
void ATD0(void);//analog to digital converter for heat sensor, capturing temperature from heat sensor
void delay(int ms);//delay 1milisecond for ms=1;

//global variable
int temperature;//global parameter to store temperature reading from heat sensor



void main (void)
{
   
   //initialize ATD0 controller  
  	ATD0CTL2=0xfA;
	ATD0CTL3=0x00;
	ATD0CTL4=0x60;
	ATD0CTL5=0x86;
	
	DDRM=0xff;//DDRM writting, will be used to turn on heater 

	//activate interrupt
	INTR_ON();
	

	
	//infinite loop to keep program running forever
	while(1)
	{
		   }


}//end main


//analog to digital converter for heat sensor, capturing temperature from heat sensor
void ATD0(void){		
	 INTR_OFF();//turn of interrupt when handling ATD0
	 
	
	 
	 //turn on heater when temperature is below 100'F
	 //turn it off when it reach 100'F
	 //we pick 99 because it will jumps between 99-100'F
	 if(temperature<=99)
	 {	 PTM|=0x80;//heater on
		}
		else
	{
	 	PTM=0x00;// heater off
	}
		


	ATD0DR0 &=0x03ff;//10 bit reading from ATD0DR0, for captured temperature value

	//convert captured temperature into farenheit
	temperature=ATD0DR0/8-5;
		
	//print temperature in Farenheit
	 printf("%d 'F\n",temperature);
		
	delay(10);//delay 10 second for heater to change temperature
		

	ATD0CTL5=0x86;//new conversion, get another temperature value from heat sensor
	

		
}//end ATD0


//delay for 1ms for each(ms)
void delay (int ms){

	 const int delayConstant = 1234;
	 int i, j;
	 
	 for (i=0; i<ms; i++) {
	 	 for (j=0; j<delayConstant; j++) {
		 }
	}
	
} //end delay