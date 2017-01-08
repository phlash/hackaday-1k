/* Where are we running? */
#if defined(TARGET_BOOT)
	#define ID "Boot!\n"
#elif defined(TARGET_ROM)
	#define ID "Option ROM!\n"
#else
	#define ID "Errr...\n"
#endif

/* start of option ROM jump to main, must be in a function for faucc to emit in .text segment.. :( */
static void __attribute__((naked)) _start(void) {
	__asm__ volatile (
		"jmp main\n"
	);
}

/* BIOS-free character display - raw video RAM access assuming standard CGA/VGA mode 3 */
static unsigned short videoInit(unsigned short v) {
	unsigned short cs=0;
	// set ES to video buffer, clear screen to specified value
	__asm__ volatile (
		"mov $0xB800, %%ax\n"
		"mov %%ax, %%es\n"
		"mov $2000, %%cx\n"
		"xor %%di, %%di\n"
		"mov %1, %%ax\n"
		"rep stosw\n"
		"push %%cs\n"
		"pop %0\n"
		: "=r"(cs)
		: "m"(v)
		: "ax","cx","di"
	);
	return cs;
}

static void videoChar(const unsigned short p, const unsigned short c) {
	__asm__ volatile (
		"mov %0, %%bx\n"
		"imul $2, %%bx\n"
		"mov %1, %%es:(%%bx)\n"
		: // no outputs
		: "r"(p), "r"(c)
		: "bx"
	);
}

/* text printer */
static unsigned short round(unsigned short v, unsigned short r) {
	return (v/r)*r;
}

static unsigned short print(unsigned short p, const char *s) {
	do {
		switch (*s) {
		default:
			videoChar(p++, 0x0E00 | *s);
			break;
		case '\n':
			p = round(p+80, 80); // function required to avoid faucc register collision :(
		case 0:
			break;
		}
		p %= (80*25);
	} while (*s++);
	return p;
}

/* hex dump */
static unsigned short hex(unsigned short p, unsigned short val) {
	char hex[6], o='0', s=12;
	char c=0;
	do {
		hex[c] = ((val>>s) & 0xF)+o;
		if (hex[c]>'9') hex[c]+=('A'-'0'-10);
		s-=4;
	} while (++c<4);
	hex[4] = '\n';
	hex[5] = 0;
	return print(p, hex);
}

/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
extern char _text;
extern char _rodata;
extern char _data;
extern char _bss;
extern char _end;
void main() {
	unsigned short pos = 0;
	unsigned short cs = videoInit(0x3521);
	pos=print(pos, ID);
	pos=hex(pos, cs);
	pos=hex(pos, (unsigned short)&_text);
	pos=hex(pos, (unsigned short)&_rodata);
	pos=hex(pos, (unsigned short)&_end);
	while(1);
}
