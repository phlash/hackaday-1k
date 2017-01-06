CFLAGS=-Os -m16 -march=i386 -ffreestanding -Wall -Werror
LDFLAGS=-m16 -march=i386 -nostdlib -Wl,--nmagic,--script=pcboot.ld

all: boot.bin

clean:
	rm -f boot.bin boot.elf boot.o

test: all
	qemu-system-i386 -drive file=boot.bin,format=raw,if=floppy,readonly -boot a

boot.bin: boot.o pcboot.ld
	gcc $(LDFLAGS) -o $@ $<

boot.o: boot.c
	gcc $(CFLAGS) -c -o $@ $<
