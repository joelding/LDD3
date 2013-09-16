#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MODULE_NAME "/dev/cfake"

static void test_read(void)
{
	int fd = open(MODULE_NAME, O_RDONLY);
	
	printf("%s\n", __func__);

	if (fd < 0) {
		printf("%s\n", strerror(errno));
		return;
	}
	
	sleep(60);
#if 1 
	if (close (fd) < 0) {
		printf("%s\n", strerror(errno));
	}
#endif
}

static void test_write(void)
{
	int fd = open(MODULE_NAME, O_WRONLY);

	printf("%s\n", __func__);
	if (fd < 0) {
		printf("%s\n", strerror(errno));
		return;
	}

	if (close (fd) < 0) {
		printf("%s\n", strerror(errno));
	}
}

int main(int argc, char *argv[])
{
	char *s = strrchr(argv[0], '/');
	s += 1;
	printf("%s\n", s);
	if (!strcmp(s, "write"))
		test_write();

	if (!strcmp(s, "read"))
		test_read();


	return 0;
}
