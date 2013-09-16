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
	char buf[80];
	ssize_t size;
	int i, cnt = 0,  
	    fd = open(MODULE_NAME, O_RDONLY);
	
	printf("%s\n", __func__);

	if (fd < 0) {
		printf("%s\n", strerror(errno));
		return;
	}

	while (cnt < 30) {
		if ((size = read(fd, buf, 80)) > 0) {
			for (i = 0; i < size; ++i)
				printf("%c", buf[i]);

			printf("\n");
		} 
		++cnt;
		sleep(1);
	}

	if (close (fd) < 0) {
		printf("%s\n", strerror(errno));
	}
}

static void test_write(void)
{
	char buf[2][80] = {
		"The LORD is my shepherd, I shall not be in want.",
		"Love is patient, love is kind. Love never fails."
	};
	int cnt = 0, sec, 
	    fd = open(MODULE_NAME, O_WRONLY);

	printf("%s\n", __func__);
	if (fd < 0) {
		printf("%s\n", strerror(errno));
		return;
	}

	srand(time(NULL));
	while (cnt < 30) {
		write(fd, buf[cnt % 2], strlen(buf[cnt % 2]) + 1);
		++cnt;
		sec = rand() % 2; /* 0 ~ 1 seconds */
		sleep(sec);
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
