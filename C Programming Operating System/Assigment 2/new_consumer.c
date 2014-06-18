#include "header.h"

int main()
{
	int sem_S, sem_N, sem_E;
	int number;
	int shmid;
	int position = 0;
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;  

    shmid = shmget((key_t)1000, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) 
	{
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

	sem_S = semget((key_t)1111, 1, 0666 | IPC_CREAT);
	set_semvalue(sem_S, 1);
	printf("Sem S: %i\n",sem_S);
	
	sem_N = semget((key_t)2222, 1, 0666 | IPC_CREAT);
	set_semvalue(sem_N, 0);
	printf("Sem N: %i\n",sem_N);
	
	sem_E = semget((key_t)3333, 1, 0666 | IPC_CREAT);
	set_semvalue(sem_E, TEXT_SZ);
	printf("Sem E: %i\n",sem_E);

/* We now make the shared memory accessible to the program. */

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) 
	{
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);

/* The next portion of the program assigns the shared_memory segment to shared_stuff,
 which then prints out any text in written_by_you. The loop continues until end is found
 in written_by_you. The call to sleep forces the consumer to sit in its critical section,
 which makes the producer wait. */

    shared_stuff = (struct shared_use_st *)shared_memory;

    while(running) 
	{
		wait(sem_N);
		wait(sem_S);

		number = shared_stuff->some_number[shared_stuff->out];
		printf("pid %d takes number %d at position %d\n",getpid(),number,shared_stuff->out);
		if(shared_stuff->out == TEXT_SZ)
		{
			shared_stuff->out = 0;
		}else{
			shared_stuff->out = (shared_stuff->out+1)%TEXT_SZ;
		}

		signal(sem_S);
		signal(sem_E);

		if(number == -1)
		{
			running = 0;
		}
		if(position == TEXT_SZ)
		{
			position = 0;
		}
		sleep(1);
	}
		/*
        if (shared_stuff->written_by_you) 
		{
            printf("You wrote: %d\n", shared_stuff->some_number);
            sleep( rand() % 4 ); // make the other process wait for us ! 
            shared_stuff->written_by_you = 0;
            if (shared_stuff->some_number == -1) 
			{
                running = 0;
            }
        }
    }*/

/* Lastly, the shared memory is detached and then deleted. */

    if (shmdt(shared_memory) == -1) 
	{
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
