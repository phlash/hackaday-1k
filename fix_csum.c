#include <stdio.h>

int main(int argc, char **argv) {
	char *rom = "rom.bin";
	FILE *fp;
	int n, c, s;
	if (argc>1)
		rom = argv[1];
	fp = fopen(rom, "r+");
	if (!fp) {
		perror("opening ROM file");
		return 1;
	}
	s = n = 0;
	while ((c = fgetc(fp)) != -1) {
		s = (s+c)%256;
		++n;
	}
	printf("read %d bytes, residual %d\n", n, s);
	fseek(fp, -1, SEEK_END);
	s = 256 - s;
	fputc(s, fp);
	fclose(fp);
	return 0;
}

