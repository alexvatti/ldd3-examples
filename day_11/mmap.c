/* 
Shows how to use mmap sys call
*/

#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <assert.h>

int main(){
	int fd,i;
	unsigned char *filedata=NULL,*temp="welcome to hello world\n";
	unsigned int size=100;

	fd = open("/dev/sample_cdrv",0666);
	if(fd<0){
		printf("file open failed\n");
		return 0;
	}
	filedata = (char *) mmap((void*)0,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);	
	perror("\n MMAP STATUS: ");
	if(filedata == NULL){
		perror("Maping Failed");
		close(fd);
		exit(1);
	}
	write(fd,temp,strlen(temp));
	write(1,filedata,strlen(temp));
	i = munmap(filedata,size);
	assert(i == 0);
	if( i != 0)
		printf(" failed to unmap\n");
	close(fd);
}
