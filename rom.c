/* start of option ROM */
__asm__(
	"mov $0xE, %ah\n"
	"mov $0x30, %al\n"
	"sub %bx, %bx\n"
	"int $0x10\n"
	"mov $128, %cx\n"	// relocate to 0x7C00 in RAM (so GNU ld can link for 16-bit!)
	"mov $0x7C0, %ax\n"
	"mov %ax, %es\n"
	"sub %si, %si\n"
	"sub %di, %di\n"
	"rep movsd\n"
	"mov $0xE, %ah\n"
	"mov $0x31, %al\n"
	"sub %bx, %bx\n"
	"int $0x10\n"
	"pushl $0\n"
	"pop %ds\n"
	"mov $0xE, %ah\n"
	"mov $0x32, %al\n"
	"sub %bx, %bx\n"
	"int $0x10\n"
	"ljmp $0, $main\n"
);

/* BIOS-free character display - raw video RAM access assuming standard CGA/VGA mode 3 */
static int _vpos = 0;
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

static void videoChar(const unsigned short c) {
	__asm__ volatile (
		"mov %%cx, %%bx\n"
		"imul $2, %%bx\n"
		"mov %%ax, %%es:(%%bx)\n"
		: // no outputs
		: "a"(c), "c"(_vpos)
		: "bx"
	);
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
static void hex(unsigned int val) {
	char hex[10], o='0', s=28;
	int c=0;
	do {
		hex[c] = ((val>>s) & 0xF)+o;
		if (hex[c]>'9') hex[c]+=('A'-'0'-10);
		s-=4;
	} while (++c<8);
	hex[8] = '\n';
	hex[9] = 0;
	print(hex);
}

/* and for everything else you can use C! Be it traversing the filesystem, or verifying the kernel image etc.*/
extern char _text;
extern char _rodata;
extern char _data;
extern char _bss;
extern char _end;
void main() {
	videoInit();
	hex(0xDEADBEEF);
	hex((unsigned int)&_text);
	hex((unsigned int)&_rodata);
	hex((unsigned int)&_data);
	hex((unsigned int)&_bss);
	hex((unsigned int)&_end);
	print("no BIOS!");
	while(1);
}