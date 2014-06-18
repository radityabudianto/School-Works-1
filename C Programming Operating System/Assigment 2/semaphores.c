#include <sys/sem.h>
#include <stdio.h>

#include "semun.h"

int wait(int semid);
int signal(int semid);
int set_semvalue(int sem_id, int num);
int del_semvalue(int sem_id);

int wait(int sem_id)
{
	struct sembuf p_buf;

	p_buf.sem_num = 0;
	p_buf.sem_op = -1; /* P() */
	p_buf.sem_flg = SEM_UNDO;
	if (semop(sem_id, &p_buf, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}

int signal(int sem_id)
{
	struct sembuf v_buf;

	v_buf.sem_num = 0;
	v_buf.sem_op = 1; /* P() */
	v_buf.sem_flg = SEM_UNDO;
	if (semop(sem_id, &v_buf, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}

int set_semvalue(int sem_id, int num)
{
    union semun sem_union;

    sem_union.val = num;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		fprintf(stderr, "Failed to set semaphore value.\n");
		return(0);
	}
	
    return(1);
}

int del_semvalue(int sem_id)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
        fprintf(stderr, "Failed to delete semaphore.\n");
		return(0);
	}

	return(1);
}