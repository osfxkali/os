CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-x86_64

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -ffreestanding -m32
LDFLAGS = -T linker.ld -nostdlib -nostartfiles -nodefaultlibs -m32

SRC = net.c antivirus.c vfs.c
OBJ = $(SRC:.c=.o)

all: simpleos.elf

simpleos.elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) -f elf $< -o $@

clean:
	rm -f *.o simpleos.elf

run: simpleos.elf
	$(QEMU) -kernel simpleos.elf -m 512M -display sdl
