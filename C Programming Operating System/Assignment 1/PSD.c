#include "PSD.h"

/******************************************************************************************************
 * 
 *  intfifo has function to create FIFO file to connect Controller and Monitor
 * 
 *******************************************************************************************************/


void intfifo(char name_of_fifo_file[256])
{
	int			status;
	struct stat	fileStatus;
	
	if (access(name_of_fifo_file, F_OK) == -1)
	{
		status = mkfifo(name_of_fifo_file, 0777);
		if (status != 0)
		{
			fprintf(stderr, "Error %s: Could not create fifo %s\n", strerror (errno), name_of_fifo_file);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (stat(name_of_fifo_file, &fileStatus) == -1)
			perror("File doesn't exist.\n");
		if ((fileStatus.st_mode & S_IFMT) != S_IFIFO)
		{
			printf("Removing file.\n");
			status = unlink(name_of_fifo_file);
			if (status != 0)
			{
				fprintf (stderr, "Error %s: Couldn't delete file %s;\n", strerror (errno), name_of_fifo_file);
				exit (EXIT_FAILURE);
			}
			printf("Creating FIFO file.\n");
			status = mkfifo(name_of_fifo_file, 0777);
			if (status != 0)
			{
				fprintf(stderr, "Error %s: Could not create fifo %s\n", strerror (errno), name_of_fifo_file);
				exit(EXIT_FAILURE);
			}
		}
	}
}