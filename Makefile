# CROSSCOMPILE = arm-linux-gnueabihf-
# arm-none-eabi  toolchain for Windows based compiling
CROSSCOMPILE = arm-none-eabi-

CFLAGS = -mcpu=cortex-a8 -marm -Wall -O2 -nostdlib -nostartfiles -ffreestanding -fstack-usage -Wstack-usage=256

all : rts.elf

olib\startup.o : slib\startup.s
	$(CROSSCOMPILE)as slib\startup.s -o olib\startup.o

main.o : main.c
	$(CROSSCOMPILE)gcc $(CFLAGS) -c main.c -o main.o

olib\irq.o : slib\irq.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\irq.s -o olib\irq.o

olib\uart.o : slib\uart.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\uart.s -o olib\uart.o

olib\gpio.o : slib\gpio.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\gpio.s -o olib\gpio.o

olib\timer.o : slib\timer.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\timer.s -o olib\timer.o

rts.elf : memmap.lds olib\startup.o main.o olib\irq.o olib\uart.o olib\gpio.o olib\timer.o
	$(CROSSCOMPILE)ld -o rts.elf -T memmap.lds olib\startup.o main.o olib\irq.o  \
    olib\uart.o olib\gpio.o olib\timer.o
	$(CROSSCOMPILE)objcopy rts.elf rts.bin -O srec
# srec format for jtag loading (ie binary format with a short header)
# binary format for MMC booting
#	$(CROSSCOMPILE)objcopy rts.elf rts.bin -O binary
	$(CROSSCOMPILE)objdump -M reg-names-raw -D rts.elf > rts.lst
	$(CROSSCOMPILE)objdump -d -S -h -t rts.elf > rts.dmp

clean :
	-@del olib\*.o *.o *.lst *.elf *.bin *.su
