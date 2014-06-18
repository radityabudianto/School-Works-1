//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017


//This program works
//main function for assign41 uses the main function from this program.
//after open assign42.s19 press "E" or "D" to increase and decrease the speed, temperature is static value from main function(we don't integrate this program with temperature sensor)
//LCD will display the Speed and temperature.

#include "hcs12dp256.h"
#include "assign41.c"
asm(".include 'basicLCD.s'");



//function prototype

	//basicLCD.s
	extern void Lcd2PP_Init(void);//initialize LCD
	extern void LCD_display(char);//Display to LCD
	extern void LCD_instruction(char);//adjust the menu of LCD 

	//assign41.c
	extern int scanKeypad(int speed);//function for Keypad input from assign41.c

	//prototype for this file
	void convertSpeedToChar(char *string, int number);
	void convertTempToChar(char *string, int number);
	void printString(int speedValue, int temp);


	


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






/*
void main(){
	int speed = 500;//initialize speed
	int temperature = 30;//initialize temperature(temperature does not change)
	char i =0;
	
	
	
	SPI1CR1=0x00;
	//initialize DDR for port P and porth H
	DDRP=0x0F;//output 0-3, when bit 0-3 is 1, means write into Data Direction Register PortP(DDRP)
	DDRH=0x0F;//input 4-7, when bit 4-7 is 0, means read from Data Direction Register PortH(DDRH)

	
	//infinite while loop for keypad reading
	//this loop also convert value of speed and temperature into array and display it on LCD
	//speed will be incremented when user press 'E',speed will be decremented when user press 'D'
	//new value of speed is updated after user press 'E' or 'D'
	//reinitialize LCD to refresh new value of speed everytime speed is updated
	while(i==0)
	{
	    printString(speed,temperature);
		DDRM=0x00;
	    PTM=0x08;
	 	speed = scanKeypad(speed);
		LCD_instruction(0x01);
		Lcd2PP_Init();
	}
}*/