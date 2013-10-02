#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/io.h>

#define DEV_FILE "/dev/ppfake"

static int is_running = 1;

static void signal_handler(int sig)
{
	is_running = 0;
}

int main(int argc, char *argv[])
{
	int fd = 0, retval = 0;
	unsigned char uc = 0x1;

	printf("Test program begins\n");
	signal(SIGINT, signal_handler);

	retval = ioperm(0x378, 4, 1);
	if (retval) {
		printf("Fail to get ioperm\n");
		return retval;
	}

	fd = open(DEV_FILE, O_WRONLY);
	if (fd < 0) {
		printf("fail to open %s\n", DEV_FILE);
		return -1;
	}

	while (is_running) {
		retval = write(fd, &uc, 1);
		if (retval < 0) break;

		uc <<= 1;
		if (uc > 4) uc = 1;
		sleep(1);
	}

	close(fd);

	retval = ioperm(0x378, 4, 0);
	if (retval) {
		printf("Fail to release ioperm\n");
		return retval;
	}

	printf("Test program ends\n");

	return 0;
}
