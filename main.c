//
// BeagleboneBlack UART Demo Program - Interrupt Driven
//
// simple, mixed C & assembly 
// demonstrates UART0, RTC and GPIO usage under interrupt control
// requires no external h/w
// tested on Win32 dev platform :(
//
// mostly hacked from Al Selen at github.com/auselen
// & Nick Kondrashov github.com/spbnick
// & Mattius van Duin, TI e2e forum
//
// known bugs:
// spurious interrupts from RTC & UART
// UART1-5 unable to initialize successfully
// contact me if you want to help at www.baremetal.tech
//
#include<stdint.h>
#define SOC_UART_0_REGS            0x44E09000
#define CONF_UART_0_RXD            0x970
#define CONF_UART_0_TXD            0x974
#define SOC_GPIO_1_REGS            0x4804c000

extern void und_isr();
extern void irq_isr();
extern void irq_init();
extern void rtc_init();
extern void rtc_irq();
extern void pinmux(uint32_t pin, uint32_t val);
extern uint32_t uart0_init(uint32_t uart_base_addr);
extern uint32_t uart_rx(uint32_t uart_base_addr);
extern void uart_tx(uint32_t uart_base_addr, uint32_t byte);
extern void uart_txi(uint32_t uart_base_addr);
extern void hexprint(uint32_t word);
extern uint32_t gpio_init(uint32_t gpio_base_addr, uint32_t gpio_pins);
extern void gpio_on(uint32_t gpio_base_addr, uint32_t gpio_pins);
extern void gpio_off(uint32_t gpio_base_addr, uint32_t gpio_pins);

volatile uint32_t rtc_irq_count=0;
volatile uint32_t hour, min, sec;
volatile uint32_t gpio_irq_count=0;
volatile uint32_t uart0_irq_count=0;
volatile uint32_t uart0_rbuf;
volatile uint32_t uart0_tbuf;

void main() {
  uint32_t old_count=0, pins;
  uint32_t masterUART = SOC_UART_0_REGS;
  pins = 0xf << 21;  // enab USR LEDs, pins 21-24
  gpio_init(SOC_GPIO_1_REGS, pins);
  pinmux(CONF_UART_0_RXD, 0X30);     // PullUp, RxActive, MUXmode 0
  pinmux(CONF_UART_0_TXD, 0x10);     // PullUp, MUXmode 0, spruh73l 9.2.2, 9.3.1.49
  uart0_init(masterUART);
  uart_tx(masterUART, 0x0A);   // print n!
  uart_tx(masterUART, 0x6E);   // (transmit in poll mode)
  uart_tx(masterUART, 0x21);
  uart_tx(masterUART, 0x0A);
  hexprint(masterUART);  // test hexprint, remove later

  rtc_init();
  irq_init();

  asm volatile(".word 0xe7f000f0");  // an undefined instruction (to test undefined isr)

// Rx IQR is enabled, bytes received by UART0 are put into uart0_rbuf
// and uart0_irq_count is incremented by uart0_isr
  while (1) {
    if(old_count != uart0_irq_count) {  // trigger on changed IRQ count
      old_count = uart0_irq_count;
      if(uart0_rbuf >= 0x20) {          // readable ASCII character received
        uart0_tbuf = uart0_rbuf;
        uart_txi(masterUART);           // transmit echo in interrupt mode
      }
    }
  }
}
