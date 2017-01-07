/* start bootloader with jump to main, has to be in a function for faucc to place in .text.. */
static void __attribute__((naked)) _start(void) {
	__asm__(
		"jmp $0, $main\n"
	);
}

/* BIOS-free character display - raw video RAM access assuming standard CGA/VGA mode 3 */
static void videoInit(void) {
	// set ES to video buffer, clear screen
	__asm__ volatile (
		"mov $0xB800, %%ax\n"
		"mov %%ax, %%es\n"
		"mov $2000, %%cx\n"
		"xor %%ax, %%ax\n"
		"mov %%ax, %%di\n"
		"rep stosw\n"
		: // no outputs
		: // no inputs
		: "ax","cx","di"
	);
}

static int _vpos = 0;
static void videoChar(const unsigned short c) {
	__asm__ volatile (
		"mov %%cx, %%bx\n"
		"imul $2, %%bx\n"
		"mov %%ax, %%es:(%%bx)\n"
		: // no outputs
		: "a"(c), "c"(_vpos)
		: "bx"
	);
/*	__asm__ volatile (
		"mov $0xE, %%ah\n"
		"mov $0, %%bx\n"
		"int $0x10\n"
		: // no outputs
		: "a"(c)
		: "bx"
	);*/
}

/* text printer */
static void print(const char *s) {
	do {
		switch (*s) {
		default:
			videoChar(0x0E00 | *s);
			++_vpos;
			break;
		case '\n':
			_vpos = (_vpos+80)/80*80;
		case 0:
			break;
		}
		_vpos %= (80*25);
	} while (*s++);
}

/* hex dump */
static void hex(unsigned short val) {
	char hex[6], o='0', s=12;
	int c=0;
	do {
		hex[c] = ((val>>s) & 0xF)+o;
		if (hex[c]>'9') hex[c]+=('A'-'0'-10);
		s-=4;
	} while (++c<4);
	hex[4] = '\n';
	hex[5] = 0;
	print(hex);
}

/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
extern char _text;
extern char _data;
extern char _rodata;
extern char _bss;
extern char _end;
void main() {
	videoInit();
	hex(0xBEEF);
	hex((unsigned short)&_text);
	hex((unsigned short)&_data);
	hex((unsigned short)&_rodata);
	hex((unsigned short)&_bss);
	hex((unsigned short)&_end);
	print("no BIOS!");
	while(1);
}
