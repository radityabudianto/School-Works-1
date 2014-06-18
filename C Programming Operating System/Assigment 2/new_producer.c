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

	srand((unsigned int)getpid());

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

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) 
	{
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);

    shared_stuff = (struct shared_use_st *)shared_memory;

	shared_stuff->in = 0;
	shared_stuff->out = 0;

	while(running) 
	{
		number = (rand()%40);
		
		wait(sem_E);
		wait(sem_S);

		
		shared_stuff->some_number[shared_stuff->in] = number;
		printf("pid %d puts number %d at position %d\n",getpid(),number,shared_stuff->in);
		if(shared_stuff->in == TEXT_SZ)
		{
			shared_stuff->in = 0;
		}else{
			shared_stuff->in = (shared_stuff->in+1)%TEXT_SZ;
		}
		
		signal(sem_S);
		signal(sem_N);
		
		if(number == -1)
		{
			running = 0;
		}
		sleep(4);
	}

    if (shmdt(shared_memory) == -1) 
	{
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}