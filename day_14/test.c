/* 
test appliction to invoke driver
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
    int fd,n;
    char buff[60];

    fd=open("/dev/sample_driver",O_RDWR);
    if(fd < 0 ) {
        printf("failed to open the driver\n");
        return 0;
    } else {
       n=read(fd,&buff,60);
       printf("read data count =%d\n",n);
       printf("%s\n",buff);  
       close(fd);
       return 0;
    }
    return 0;
}
