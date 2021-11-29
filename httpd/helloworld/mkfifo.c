#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int test()
{
	char buf[256]={0};
	int res=0;
	int flag=0;
	mkfifo("lhx_server",0777);
	int fd = open("lhx_server",O_RDWR);
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(fd,&rset);
	fd_set wset;
	FD_ZERO(&wset);
	FD_SET(fd,&wset);
	while (1)
	{
		printf("In while\n");
		select(fd+1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(fd,&rset))
		{
			printf("can read!\n");
			if (res = read(fd,buf,256) < 0)
				exit(-2);
			printf("%s\n",buf);
			sprintf(buf,"%s","\0");
		}
		/*
                if(FD_ISSET(fd,&wset))
                {
                        printf("can write!\n");
			sleep(3);
                        if (res = read(fd,buf,256) < 0)
                                exit(-2);
                        printf("%s\n",buf);
                }
		*/

	}

}

int main()
{
	test();
	return 0;
}
