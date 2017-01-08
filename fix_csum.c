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
	if (argc>2) {
		n /= 512;
		s = (s+n)%256;
		s = 256 - s;
		printf("writing pages %d, csum %d\n", n, s);
		fseek(fp, 2, SEEK_SET);
		fputc(n, fp);
		fseek(fp, -1, SEEK_END);
		fputc(s, fp);
	}
	fclose(fp);
	return 0;
}

