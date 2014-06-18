
//This machine works and printed desireable result in HC12 machine

//set the global variable
int store[15];//storage for array( this is used for storing binary value 16 bit=0-15
int i;//initial for loop counter
int j=16;// number of repetation


//display status function will print the speed(in HEX),temperature in(HEX), proximity(Showing direction and information about obstacle) and keyPressed(user input in 16 bits)
//the use of char is for storing character in 1 byte of memory
char displaySystemStatus(char speed, char temperature, unsigned int proximity, unsigned int keyPressed){
	 char obstacle;//store value for obstacle
	 char direction; // forward(1) or backward(1)
	 unsigned int temp=proximity;//temp is used for temporary storage for proximity and keypressed before the value is processed
 
 
 
 	  //convert int into binary to get direction
 	  //get the reminder from division by 2( to get 1 or 0)
	  //divide the proximity by 2
	  
	  for(i=0;i<j;i++){
	  
			//get the 7 and 15 bit	and set the direction(15 bit) and obstacle(7 bit)  
	   		  if(i==7) 
			  		direction=temp%2;
			  if(i==15) 				
			  		obstacle=temp%2; 
			
		temp=temp/2;
	
		}				  
		
   
	//display speed and temperature in HC12 machine
 	 printf("Speed(HEX):%x\n",speed);
	 printf("Temperature(HEX):%x\n",temperature);
	 //check obstacle and display whether there is obstacle and show the direction
	 if(obstacle==1)
	 {
	  	printf("Proximity(HEX), Obstacle is near, Direction=%x\n",direction);
	  }
	  else
	  {
	     printf("Proximity: Obstacle is far, Direction=%x\n",direction);  
	  }
  
	//display keypressed in hex
	printf("KeyPressed(HEX):%x\n",keyPressed);
	return 1;		//returned value always 1
}  
	 
			
 
	 
void main()
{	 
	
	
	 
	 //set the initial condition for test function
 	 int speed=30;
	 int temperature=35;
	 unsigned int proximity=0xffff;
	 unsigned int keypressed=0x1234;
	
	//set the stack pointer
	 asm("lds #$3DFF");
 	
 

 	//call display status
 	 displaySystemStatus(speed,temperature,proximity,keypressed);
 	 asm("swi");

}
