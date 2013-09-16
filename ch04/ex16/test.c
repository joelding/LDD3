#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	FILE *fp = fopen("/proc/testseq", "r");
	unsigned char c;

	for (i = 0; i < 256; ++i) {
		fread(&c, 1, 1, fp);
		printf("%x ", c);
	}

	printf("\n");
	fclose(fp);

	return 0;
}
