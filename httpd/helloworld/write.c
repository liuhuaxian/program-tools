#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char *pc = "lhx test";
	int fd = open("lhx_server",O_WRONLY);
	write(fd,pc,sizeof(pc));
	close(fd);
	return 0;
}
