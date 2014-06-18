//Raditya Budianto, 100836539
//Volodymyr Sharovar, 100772017

//This program works, execute assign43a.s19
//The main function will show demo for 2 full rotation anticlockwise and 2 full rotation clockwise stepper motor

#include "hcs12dp256.h"
void anticlockwise(char n);//each n is approximately 65 degree anticlockwise rotation(based on observation, it takes 11 steps to make 2 full steps) , for n=0 is the first 65 degree rotation
void clockwise(char n);//each n is approximately 65 degree clockwise rotation(based on observation, it takes 11 steps to make 2 full steps), for n=0 is the first 65 degree rotation
void delay(int ms);// n milisecond, for ms=n


void main()
{
	//anticlockwise 2 turn (n=0 until 10, 11 steps)
	anticlockwise(10);
	
	delay(300);//delay 3 second
	
	//clockwise 2 turn (n=0 until 10, 11 steps)
	clockwise(10);
}


//initialize DDRP and DDRT for writting
//use PTT to create anticlockwise movement with 10ms delay for each stepper motor move
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
}//end anticlockwise




//initialize DDRP and DDRT for writting
//use PTT to create clockwise movement with 10ms delay for each stepper motor move
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
}//end clockwise



//gives delay in milisecond
void delay (int ms){

	 const int delayConstant = 1234;
	 int i, j;
	 
	 for (i=0; i<ms; i++) {
	 	 for (j=0; j<delayConstant; j++) {
		 }
	}
	
}// end delay