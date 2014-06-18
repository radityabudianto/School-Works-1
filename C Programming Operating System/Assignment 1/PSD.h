#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define MONITOR_FIFO_NAME "/tmp/cli_%d_fifo"
#define CONTROLLER_FIFO_NAME "/tmp/serv_fifo"
#define BUFFER_SIZE 256
#define MAX 512

#define START "start"
#define STOP "stop"
#define ACK "ACK\n"

struct data_to_pass_st {
    pid_t  		monitor_pid;
	char		path_of_fifo[BUFFER_SIZE];
};

struct msgbuf {
    long mtype;       
    char mtext[MAX];    
};

void intfifo(char name_of_fifo_file[BUFFER_SIZE]);