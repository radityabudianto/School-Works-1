/* 		100836539 Raditya Budianto
 * 		100772017 Volodymyr Sharovar
 * 		
 * 		The Controller acts as a FIFO server. It creates a child process to echo messages
 * 		from a message que.
 */

#include "PSD.h"

//Global variable that represents if the Controller is ready to shutdown or not.
int ready_to_shutdown = 0;

//Parent process of Controller. Receives the process id of the child and the message queue id.
int parentProcess(pid_t pid, int msgid);

//Child process of Controller. Receives the message queue id.
int childProcess(int msgid);

//Used for signal handler
void signal_quit(int sig);

int main()
{
	pid_t 				pid;
	int 				status;
	char				string_to_echo[128];
	int 				msgid;
	struct sigaction 	act;

	//Set up segaction
	act.sa_handler = signal_quit;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

	//Creates a message queue
	msgid = msgget((key_t)4321, 0666 | IPC_CREAT);

	if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d. %s\n", errno,strerror(errno));
        exit(EXIT_FAILURE);
    }

	//Creates a child process
	pid = fork();
	if(pid == -1)
	{
		perror("Failed at creating a child process.");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{
		(void) signal (SIGINT, SIG_IGN);
		printf("Inside the Controller's child process. Pid = %d\n\n",getpid());
		status = childProcess(msgid);
		if(status == -1)
		{
			perror("Problem in the child process.\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		sigaction(SIGINT, &act, 0);
		printf("\nInside the Controller's parent process. Pid = %d\n\n",getpid());
		status = parentProcess(pid, msgid);
		if(status == -1)
		{
			perror("Problem in the parent process.\n");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

/*
 *  The job of the parent process is to act as a FIFO server. It first tries to create
 * 	a fifo file for controller and see if any client "connected" to it by writting the
 * 	clients information. It then tries to access the clients fifo file and tell it to
 * 	start the procedure by writing "start" message to it. It records pid's of each client
 *  that connect to the server. When the time comes to quit(when CTRL-C is pressed) it 
 *  writes "stop" to each client and cleans up the message queue.
 */
int parentProcess(pid_t pid, int msgid)
{
	int			controller_fifo_fd, monitor_fifo_fd;
	int			read_size, counter=0;
	char 		monitor_fifo[256];
	int			pids[256];
	
	struct data_to_pass_st data;

	//Sets up Controller's fifo file
	intfifo(CONTROLLER_FIFO_NAME);

	printf("Opening controller fifo file.\n\n");
	controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_RDONLY);
	if (controller_fifo_fd == -1)
	{
		fprintf(stderr, "open() failed with error: %d. %s\n", errno,strerror(errno));
        return -1;
	}
	printf("Starting controller fifo.\n");
	do
	{
		read_size = read(controller_fifo_fd, &data, sizeof(data));
		
		if (read_size > 0)
		{
			monitor_fifo_fd = open(data.path_of_fifo, O_WRONLY);
			if (monitor_fifo_fd == -1)
			{
				fprintf(stderr, "open() failed with error: %d. %s\n", errno,strerror(errno));
        		return -1;
			}
			else
			{
				// Controller is telling the specific monitor to start and keeps track of each pid
				pids[counter] = data.monitor_pid;
				counter++;
				if (write(monitor_fifo_fd, START, sizeof(START)) == -1)
				{
					fprintf(stderr, "write() failed with error: %d. %s\n", errno,strerror(errno));
        			return -1;
				}
			}
			close(monitor_fifo_fd);
		}
	}while(ready_to_shutdown != 1);
	
	for(counter-=1;counter>=0;counter--)
	{
		sprintf(monitor_fifo,MONITOR_FIFO_NAME,pids[counter]);
		monitor_fifo_fd = open(monitor_fifo, O_WRONLY);
		if (write(monitor_fifo_fd, STOP, sizeof(STOP)) == -1)
		{
			fprintf(stderr, "write() failed with error: %d. %s\n", errno,strerror(errno));
        	return -1;
		}
		close(monitor_fifo_fd);
	}
	close(controller_fifo_fd);
	
	printf("Killing child process. Pid = %d.\n",pid);
	kill(pid,SIGKILL);
	sleep(0.5);
	printf("Cleaning the message que.\n");
	if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed with error: %d. %s\n", errno,strerror(errno));
        return -1;
    }
	return 0;
}

/*
 * The job of child process is to read from the message queue, echo the read message
 * and send an ACK(acknowledge) message back to the client. 
 */
int childProcess(int msgid)
{
	struct msgbuf msg;
	while(1)
	{
		if (msgrcv(msgid, (void *)&msg, MAX, 0, 0) == -1) 
		{
    		fprintf(stderr, "msgrcv failed with error: %d. %s\n", errno,strerror(errno));
    		return -1;
    	}
		printf("%s\n",msg.mtext);
		strcpy(msg.mtext,ACK);
		if (msgsnd(msgid, (void *)&msg, MAX, 0) == -1) 
		{
    		fprintf(stderr, "msgsnd failed with error: %d. %s\n", errno,strerror(errno));
    		return -1;
    	}
	}
	return 0;
}

void signal_quit(int sig)
{
	printf("CTRL-C was pressed. Preparing to shutdown...\n\n");
	ready_to_shutdown = 1;
}