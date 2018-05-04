#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "zc_cdev.h"

static void printHelp()
{
	printf("===============Usage===============\n" \
			"Argument options:\n" \
			"-i io(read and write) test\n" \
			"-c ioctl test\n");
}

int main(int argc,char *argv[])
{
	char buf[512] = {0};
	int i = 0, ret;
	int devfd;
	zc_ioctl_data data;
	int testmode = 0;

	for(i = 0; i < argc; i++)
	{
		sprintf(buf, "%sargument%d=%s;", buf, i, argv[i]);
	}
	printf("argc=%d, arguments: %s\n", argc, buf);

	if(argc > 1)
	{
		if(*argv[1] != '-')
		{
			printf("argument error!\n");
			printHelp();
			return 0;
		}
		switch (argv[1][1])
		{
		case 'i':
			printf("test option is -i, do io(read and write) test\n");
			testmode = 1;
			break;
		case 'c':
			printf("test option is -c, do ioctl test\n");
			testmode = 2;
			break;
		default:
			printf("argument error!\n");
			printHelp();
			return 0;
		}
	}

	devfd = open("/dev/" CDEVFILE, O_RDWR);
	printf("devfd=%d\n", devfd);
	if(devfd == -1)
	{
		printf("fopen error!\n");
		return -1;
	}
	switch(testmode)
	{
	case 1:
		memset(buf, 0, sizeof(buf));
		ret = read(devfd, buf, 512);
		printf("read returns %d, errno=%d, message: %s\n", ret, errno, buf);

		ret = lseek(devfd, 3, SEEK_CUR);
		printf("lseek returns %d, errno=%d \n", ret, errno);

		ret = write(devfd, buf, 512);
		printf("write returns %d, errno=%d \n", ret, errno);
		break;
	case 2:
		ret = ioctl(devfd, ZC_IOC, NULL);
		printf("ZC_IOC returns %d, errno=%d\n", ret, errno);
		ret = ioctl(devfd, ZC_IOCGET, &data);
		printf("ZC_IOCGET returns %d, errno=%d, data.length=%d\n", ret, errno, data.length);
		data.length = 512;
		ret = ioctl(devfd, ZC_IOCSET, &data);
		printf("ZC_IOCSET returns %d, errno=%d, data.length=%d\n", ret, errno, data.length);
		
		break;
	default:

		break;
	}
	

	close(devfd);
	return 0;
}
