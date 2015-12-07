# BBB_UART_IRQ
BeagleBone Black UART program with Interrupts, all bare metal.

README  Beaglebone Black UART Demo Program

Main prog written in C and drivers/runtime services in GNU-Assembly.  
It demonstrates UART (and RTC and GPIO) usage under interupt control, UND & IRQ.  
Makefile & loadscript provided for GCC in Win32 (gasp! it is all i have).  
I use the XDS100V2 jtag to load programs.  It is more work up front, but 
much easier in the long run.  
I did not test the binary format version 
for MMC booting.  (i broke the SD slot on my laptop)

It is all there in very concise format, so it should be easy for noobs 
to understand/test/hack/mod  
for your next BBB bare metal interrupt project!  

It requires no additional hardware.  Type letters from a terminal emulator 
program (I use termite)  
and it will echo them.  Also USR1 blinks every second 
& USR2 blinks when you hit the boot button.  
All under interrupt control.  

You can provide feedback at my website www.baremetal.tech  
