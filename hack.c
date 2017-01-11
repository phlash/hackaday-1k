/* Where are we running? */
#if defined(TARGET_BOOT)
	#define ID "Boot!\n"
#elif defined(TARGET_ROM)
	#define ID "Option ROM!\n"
#else
	#define ID "Errr...\n"
#endif

/* start of boot sector OR option ROM: jump to main,
   NB: must be in a function for faucc to emit in .text segment.. :( */
static void __attribute__((naked)) _start(void) {
	__asm__ volatile (
		"jmp main\n"
	);
}

/* BIOS-free character display - raw video RAM access assuming standard CGA/VGA mode 3 */
static unsigned short videoInit(unsigned short v) {
	unsigned short cs=0;
	// set ES to video buffer, clear screen to specified value
	// set DS to CS, return value
	__asm__ volatile (
		"mov $0xB800, %%ax\n"
		"mov %%ax, %%es\n"
		"mov $2000, %%cx\n"
		"xor %%di, %%di\n"
		"mov %1, %%ax\n"
		"rep stosw\n"
		"mov %%cs, %%ax\n"
		"mov %%ax, %%ds\n"
		"mov %%ax, %0\n"
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
		"mov %1, %%ax\n"
		"mov %%ax, %%es:(%%bx)\n"
		: // no outputs
		: "m"(p), "m"(c)
		: "ax","bx"
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
static char *_hlkup = "0123456789ABCDEF";
static unsigned short hex(unsigned short p, unsigned short val) {
	char hex[6], c=4;
	while (c-->0) {
		hex[c] = _hlkup[val & 0xF];
		val >>= 4;
	}
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
	pos=hex(print(pos, "CS:"), cs);
	pos=hex(print(pos, "TX:"), (unsigned short)&_text);
//	pos=hex(print(pos, "RO:"), (unsigned short)&_rodata);
//	pos=hex(print(pos, "DA:"), (unsigned short)&_data);
	pos=hex(print(pos, "EN:"), (unsigned short)&_end);
	while(1);
}
