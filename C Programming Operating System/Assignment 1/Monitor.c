/* 		100836539 Raditya Budianto
 * 		100772017 Volodymyr Sharovar
 * 		
 * 		The Monitor acts as a FIFO client. It creates a child process to echo messages
 * 		from a message queue.
 * 
 * 		The purpose of Monitor is to create random generated heart rate and send it to Controller
 * 		The Monitor can run as multiple Monitor in Parallel by using separate terminal
 */

#include "PSD.h"

int parentProcess(pid_t pid, int monitor_fifo_fd);

int childProcess(int msgid);

/******************************************************************************************************
 * 
 * The Monitor open WRITE FIFO pipe to establish connection with Controller
 * If the connection is successfully establised, the Monitor will create a parent and a child process
 *
 *******************************************************************************************************/
int main()
{
	pid_t 			pid;
	int 			status;
	int				controller_fifo_fd, monitor_fifo_fd;
	int				read_size;		
	char			string_buffer[BUFFER_SIZE];
	char			monitor_fifo[BUFFER_SIZE];

	struct data_to_pass_st data;

	int msgid = msgget((key_t)4321, 0666 | IPC_CREAT);
	
	data.monitor_pid = getppid();
	sprintf(monitor_fifo, MONITOR_FIFO_NAME, data.monitor_pid);
	strcpy (data.path_of_fifo, monitor_fifo);
	intfifo(monitor_fifo);

	controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_WRONLY);
	if (controller_fifo_fd == -1) 
	{
		fprintf(stderr, "Sorry, no server\n");
		exit(EXIT_FAILURE);
	}

	status = write(controller_fifo_fd, &data, sizeof(data));
	if (status == -1)
	{
		fprintf(stderr, "Couldnt write to controller fifo.\n");
		exit(EXIT_FAILURE);
	}
	close(controller_fifo_fd);
	monitor_fifo_fd = open(monitor_fifo, O_RDONLY);
	if (monitor_fifo_fd == -1) 
	{
		fprintf(stderr, "Sorry, no monitor\n");
		exit(EXIT_FAILURE);
	}
	read_size = read(monitor_fifo_fd, string_buffer, sizeof(string_buffer));
	if (read_size > 0)
	{
		if(strcmp(string_buffer,START)==0)
		{
			
			pid = fork();

			if(pid == -1)
			{
				perror("Failed at creating a child process.");
				exit(EXIT_FAILURE);
			}
			else if(pid == 0)
			{
				status = childProcess(msgid);
				if(status == -1)
				{
					perror("Problem in the child process.");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				status = parentProcess(pid, monitor_fifo_fd);
				if(status == -1)
				{
					perror("Problem in the parent process.");
					exit(EXIT_FAILURE);
				}
			}

		}
	}
	exit(EXIT_SUCCESS);
}

/******************************************************************************************************
 * 
 *  parentProcess get the string from FIFO to check "stop"
 *  if "stop" is not found, parentProcess will ignore the string
 * 	if "stop" is found parentProcess will close the FIFO and kill the child process
 * 
 *******************************************************************************************************/

int parentProcess(pid_t pid, int monitor_fifo_fd)
{
	int				read_size;
	char			string_buffer[BUFFER_SIZE];
	
	while(1)
	{	
		read_size = read(monitor_fifo_fd, string_buffer, sizeof(string_buffer));
		if (read_size > 0)
		{
			if(strcmp(string_buffer,STOP)==0)
			{
				break;
			}
		}
	}
	close(monitor_fifo_fd);
	printf("Killing child process. Pid = %d.\n",pid);
	kill(pid,SIGKILL);
	printf("Exiting monitor pid = %d\n",getpid());
	return 0;
}

/******************************************************************************************************
 * 
 *  childProcess is used to generate name (using ARGV!!!) to avoid same name is used
 *  after the name is generated, it will send string of message into Controller every 2.5 second
 *  the message consist PID, name of the patient, and random generated heartrate
 *  
 * 
 *******************************************************************************************************/


int childProcess(int msgid)
{
	char			*name[] = {"John","Bob","Rob","Kate","Henry","Wilson","Craig","Alex","Brendon","Greg"};
	char			*patientsName;
	int				patientNumber;
	int 			pid = getpid();
	struct msgbuf 	msg;

	srand(getpid());
	patientNumber = rand() % 10;
	patientsName = name[patientNumber];
	msg.mtype = pid;
	
	while(1)
	{
		int r = rand() % 40 + 60;
		sprintf(msg.mtext,"pid = %d, name = %s => heart-rate = %d\n",pid,patientsName,r);
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