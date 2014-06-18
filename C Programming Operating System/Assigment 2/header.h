#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>

#define TEXT_SZ 9

#define BUFFER_SIZE 4

struct shared_use_st {
	int some_number[TEXT_SZ];
	int in;
	int out;
};
