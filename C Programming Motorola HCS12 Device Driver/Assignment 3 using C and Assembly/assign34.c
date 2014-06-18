//This program works and gives correct output for the test function


int i; //loop counter
int j=16; //counter limiter
int getDirection[3];
char store[15];
unsigned int bit8;
unsigned int bit9;
unsigned int modifiedBits;


void main()
{
//Test function

//Variable for Test1-4
char speed=10;
unsigned int proximity1;//proximity for test 1
unsigned int proximity2;//proximity for test 2
unsigned int proximity3;//proximity for test 3
unsigned int proximity4;//proximity for test 4

//Test1, check north and obstacle=1
proximity1 = 0b1000111111110000;
collision_detection(speed, proximity1);

//Test2, check south and obstacle=1
proximity2 = 0b1000111111110001;
//printf("Current  Proximity Value(HEX)=%x\n\n\n",proximity3);
collision_detection(speed, proximity2);

//Test3, check other than N,S,NE,or SW and obstacle=1
proximity3 = 0b1000011111111011;
collision_detection(speed, proximity3);

//Test4, Obstacle=0(no obstacle)
proximity4 = 0b0000011111111011;
collision_detection(speed, proximity4);
asm("swi"); 	 
}	

char collision_detection(char speed, unsigned int proximity)
{
 int obstacle;
 int direction;
 unsigned int temp=proximity;

	  printf("Speed(HEX)=%x\n",speed);
	  printf("Current  Proximity Value(HEX)=%x\n",proximity);
	  
	  //convert int into binary to get direction
 	  //get the reminder from division by 2( to get 1 or 0)
	  //divide the proximity by 2
	  for(i=0;i<j;i++){
	   	
			
			//Store bit 0-2 for direction
			if(i<3) getDirection[i]=temp%2; 
		
			//get the 7 and 15 bit	and set the direction(15 bit) and obstacle(7 bit)  	 
			  if(i==7) 
			  		direction=temp%2;
			 		obstacle=temp%2; 
			 		temp=temp/2;
					
					 if(i)
					 { 				
			  		
					//printf("Obstacle=%d\n",obstacle);
					}
		}
		
		
		//if obstacle=0 multiply speed by 2	
	if(obstacle==0)	{
			printf("No Obstacle(Obstacle=0), New Value of speed(HEX)=%x\n",speed*2);
	}	
	
	//check North and North East
	// if its North update the proximity bits to turn left
	
	else{
	   	  if((getDirection[0]==0 && getDirection[1]==0 && getDirection[2]==0 && obstacle==1)) 
		  {		
				printf("test");	
			//clear the bits 8 and 9 is for bit location	
					proximity &= ~( (1 << 8) | (1 << 9) );
			//set the bits 8 and 9 is for bit location
				  proximity |= ( (1 << 8) | (0 << 9) );
			//proximity = proximity |=temp;
						printf("Current Direction is North, New Value of Proximity(HEX)=%x\n",proximity);
			
 			
	 	
		}
		 	else{
			 // if its North East update the proximity bits to turn left	
			 	 if(getDirection[0]==1 && getDirection[1]==0 && getDirection[2]==1 && obstacle==1)
				     {
	 			 //clear the bits 8 and 9 is for bit location	
				   proximity &= ~( (1 << 8) | (1 << 9) );
				  //set the bits 8 and 9 is for bit location
				   proximity |= ( (1 << 8) | (0 << 9) );
				   //proximity = proximity |=temp;
				 	printf("Current Direction is North East, New Value of Proximity(HEX)=%x\n",proximity);
					}
	 		else{ 	
	//check South and SouthWest
	// if its South or SouthWest, update the proximity bits to turn right
	     	  	 if(getDirection[0]==1 && getDirection[1]==0 && getDirection[2]==0 && obstacle==1){
	//clear the bits 8 and 9 is for bit location	
				    proximity &= ~( (1 << 8) | (1 << 9) );
			//set the bits 8 and 9 is for bit location
				  	proximity |= ( (0 << 8) | (1 << 9) );
			//proximity = proximity |=temp;
					printf("Current Direction is South, New Value of Proximity(HEX)=%x\n",proximity);
					}
			
			else{		
			 //check SouthWest
			// if its South or SouthWest, update the proximity bits to turn right
					if(	getDirection[0]==1 && getDirection[1]==1 && getDirection[2]==1 &&obstacle==1)	{
				//clear the bits 8 and 9 is for bit location	
					proximity &= ~( (1 << 8) | (1 << 9) );
				 //set the bits 8 and 9 is for bit location
					proximity |= ( (0 << 8) | (1 << 9) );
				//proximity = proximity |=temp;
					printf("Current Direction is South West, New Value of Proximity(HEX)=%x\n",proximity);
					 }
			 else
				{//Condition for not N,S,NE,SW and Obstacle==1)
					printf("Current Direction is not N,S,NW,SW, New Value of speed(HEX)=%x\n",speed/8);
				}
				}
				}
	 			}
				}


return 1;

}		