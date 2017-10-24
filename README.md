# hackaday-1k
Fun with tiny programs (&lt;1024bytes) inspired by the hackaday 1k challenge

## Target? Build host? Tools?
OK:
* Target: x86 PC (QEMU i386), specifically a boot sector or BIOS ROM.
* Build host: Linux (Debian 9 in my case)
* Tooling: faucc (standard package), a reasonable, slightly buggy x86 real mode compiler. Also gcc but it makes huge code. And breaks.

## Files
* Makefile: those lovely build rules, targets all &amp; clean are helpful
* fix_csum.c: patches up the checksum in a PC BIOS ROM image
* hack.c: the target source code, mostly C (with some embedded assembly)
* pcboot.ld: loader map for bootsector build
* pcrom.ld: loader map for BIOS ROM build
