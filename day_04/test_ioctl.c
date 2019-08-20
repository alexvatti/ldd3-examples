/*
test application
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "driver_ioctl_const.h"

int main()
{
	int fd;
	char my_buf[4000];
	char ch;
	
	
	fd = open( "/dev/sample_chrdrv", O_RDWR );
	if( fd < 0 ) {
		printf("\n\nDevice could not be opened\n\n");
		return 1;
	}

	printf("Device opened with ID [%d]\n", fd);
	   
	if(ioctl( fd, CLEAR_BUFFER, NULL)<0)
		printf("ioctl failed\n");

	c = 'B';
	if( ioctl( fd, FILL_BUFFFER, &ch ) < 0 )
		printf("ioctl failed\n");

      	/* test if it worked */
	read( fd, my_buf, 20 );
	printf("New contents\n%s\n", my_buf);
	
	close(fd);
	
	exit(0);
}


