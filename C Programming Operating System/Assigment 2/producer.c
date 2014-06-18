#include "header.h"

int main()
{
	int number;
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    char buffer[BUFSIZ];
    int shmid;

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    if (shmid == -1) 
	{
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) 
	{
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);

    shared_stuff = (struct shared_use_st *)shared_memory;
	
    while(running) 
	{
		shared_stuff->written_by_you = 1;
		
        while(shared_stuff->written_by_you == 1) 
		{
            sleep(1);            
            printf("waiting for client...\n");
        }
		
		number = (rand()%5)-1;
		shared_stuff->some_number = number;
		printf("pid %d wrote number %d\n",getpid(),number);
		
		if (number == -1)
		{
			running = 0;
		}
    }

    if (shmdt(shared_memory) == -1) 
	{
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}