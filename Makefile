# Keep intermediates, don't apply old suffix rules
.PRECIOUS: %.s %.o
.SUFFIXES:

CC=faucc
CFLAGS=-b i286 --freestanding -D$(TARGET)
LDFLAGS=-b i286 -nostdlib -T $(LDSCRIPT)
# gcc switches..
#CC=gcc
#CFLAGS=-save-temps -Os -m16 -march=i386 -ffreestanding -Wall -Werror -D$(TARGET)
#LDFLAGS=-save-temps -m16 -march=i386 -nostdlib -Wl,--nmagic,--script=$(LDSCRIPT)

all: boot.bin rom.fix

clean:
	rm -f *.bin *.fix *.o *.i *.s fix_csum

test: all
	qemu-system-i386 -drive file=boot.bin,format=raw,if=floppy,readonly -boot a -net none

testrom: all
	qemu-system-i386 -option-rom rom.fix -net none

rom.fix: rom.bin fix_csum
	cp rom.bin $@
	./fix_csum $@ write

boot.bin: LDSCRIPT=pcboot.ld
boot.bin: TARGET=TARGET_BOOT

rom.bin: LDSCRIPT=pcrom.ld
rom.bin: TARGET=TARGET_ROM

boot.s: hack.c
	$(CC) $(CFLAGS) -S -o $@ $<

rom.s: hack.c
	$(CC) $(CFLAGS) -S -o $@ $<

%.bin: %.o $(LDSCRIPT)
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.s
	$(CC) $(CFLAGS) -c -o $@ $<

fix_csum: fix_csum.c
	gcc -o $@ $<

