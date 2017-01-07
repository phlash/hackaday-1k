CFLAGS=-save-temps -Os -m16 -march=i386 -ffreestanding -Wall -Werror
LDFLAGS=-save-temps -m16 -march=i386 -nostdlib -Wl,--nmagic,--script=$(LDSCRIPT)

all: boot.bin rom.fix

clean:
	rm -f *.bin *.fix *.o *.i *.s fix_csum

test: all
	qemu-system-i386 -drive file=boot.bin,format=raw,if=floppy,readonly -boot a -net none

testrom: all
	qemu-system-i386 -option-rom rom.fix -net none

rom.fix: rom.bin fix_csum
	cp rom.bin $@
	./fix_csum $@

boot.bin: LDSCRIPT=pcboot.ld

rom.bin: LDSCRIPT=pcrom.ld

%.bin: %.o $(LDSCRIPT)
	gcc $(LDFLAGS) -o $@ $<

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

fix_csum: fix_csum.c
	gcc -o $@ $<
