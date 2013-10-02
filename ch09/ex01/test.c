#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>
#include <signal.h>

#define BASE 0x378

static int running = 1;

void signal_handler(int sig)
{
	running = 0;
}

int main(int argc, char *argv[])
{
	int result = 0;
	unsigned char uc = 0x01;

	printf("Parallel Port Interface Test (Base: 0x%x)\n", BASE);

	signal(SIGINT, signal_handler);

	result = ioperm(BASE, 4, 1);
	if (result) {
		printf("ERROR: could not set permissions on ports\n");
		return result;
	}

	while (running) {
		outb(uc, BASE);
		uc <<= 1;
		if (uc > 4) uc = 1;
		sleep(1);
	}

	outb(0x00, BASE);
	result = ioperm(BASE, 4, 0);
	if (result) {
		printf("ERROR: could not clear permissions on ports\n");
		return result;
	}

	return 0;
}
