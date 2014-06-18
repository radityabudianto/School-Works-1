#include "PSD.h"

int parentProcess(int msgid);

int childProcess(char* nameOfPatient, int msgid);

int main()
{
	//struct message 	msg;
	pid_t 			pid;
	int 			status;
	char			*name[] = {"John","Bob","Rob","Kate","Henry","Wilson","Craig","Alex","Brendon","Greg"};
	int				patientNumber;
	char			*patientsName;

	int msgid = msgget((key_t)1238, 0666 | IPC_CREAT);

	//msgid = msgget(ftok(CONTROLLER_FIFO_NAME,'S'), 0666 | IPC_CREAT);

	srand(getpid());
	patientNumber = rand() % 10;
	patientsName = name[patientNumber];

	
	
	pid = fork();

	if(pid == -1)
	{
		perror("Failed at creating a child process.");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{
		printf("pid=%d\n",pid);
		status = childProcess(patientsName, msgid);
		if(status == -1)
		{
			perror("Problem in the child process.");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		status = parentProcess(pid);
		if(status == -1)
		{
			perror("Problem in the parent process.");
			exit(EXIT_FAILURE);
		}
	}
	//}
	exit(EXIT_SUCCESS);
}

int parentProcess(pid_t pid)
{
	int		controller_fifo_fd, monitor_fifo_fd;
	int		read_size;		

	int		status;
	char	string_buffer[BUFFER_SIZE];
	char	monitor_fifo[BUFFER_SIZE];
	char 	message[] = "stop";


	struct data_to_pass_st data;

	data.monitor_pid = getppid();
	sprintf(monitor_fifo, MONITOR_FIFO_NAME, data.monitor_pid);
	strcpy (data.path_of_fifo, monitor_fifo);
	intfifo(monitor_fifo);

	controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_WRONLY);
	if (controller_fifo_fd == -1) 
	{
		fprintf(stderr, "Sorry, no server\n");
		return 1;
	}

	status = write(controller_fifo_fd, &data, sizeof(data));
	if (status == -1)
	{
		fprintf(stderr, "Couldnt write to controller fifo.\n");
		return 1;
	}
	monitor_fifo_fd = open(monitor_fifo, O_RDONLY);
	if (monitor_fifo_fd == -1) 
	{
		fprintf(stderr, "Sorry, no monitor\n");
		return 1;
	}
	while(1)
	{	
		read_size = read(monitor_fifo_fd, string_buffer, sizeof(string_buffer));
		if (read_size > 0)
		{
			if(strcmp(string_buffer,message)==0)
			{
				break;
			}
		}
	}
	kill(pid,SIGKILL);
	return 0;
}

int childProcess(char* nameOfPatient, int msgid)
{
	struct msgbuf msg;
	int pid = getpid();
	msg.mtype = pid;
	while(1)
	{
		int r = rand() % 40 + 60;
		sprintf(msg.mtext,"pid = %d, name = %s => hearrate = %d\n",pid,nameOfPatient,r);
		printf("%s\n",msg.mtext);
		if (msgsnd(msgid, (void *)&msg, MAX, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
		if (msgrcv(msgid, (void *)&msg, MAX, pid, 0) == -1) 
		{
    		fprintf(stderr, "msgrcv failed with error: %d %s\n", errno,strerror(errno));
    		exit(EXIT_FAILURE);
    	}
		printf("%s\n",msg.mtext);
		sleep(2.5);
	}
	return 0;
}