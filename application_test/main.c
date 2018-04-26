#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "zc_cdev.h"

int main(int argc,char *argv[])
{
	char buf[512] = {0};
	int i = 0, ret;
	int devfd;
	zc_ioctl_data data;

	for(i = 0; i < argc; i++)
	{
		sprintf(buf, "%sargument%d=%s;", buf, i, argv[i]);
	}
	printf("argc=%d, arguments: %s\n", argc, buf);

	devfd = open("/dev/" CDEVFILE, O_RDWR);
	printf("devfd=%d\n", devfd);
	if(devfd == -1)
	{
		printf("fopen error!\n");
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	ret = read(devfd, buf, 512);
	printf("read returns %d, errno=%d, message: %s\n", ret, errno, buf);

	ret = lseek(devfd, 3, SEEK_CUR);
	printf("lseek returns %d, errno=%d \n", ret, errno);

	ret = write(devfd, buf, 512);
	printf("write returns %d, errno=%d \n", ret, errno);

	close(devfd);
	return 0;
}
