#include "uart.h"
#include <stdio.h>      // perror
#include <sys/mman.h>   // mmap
#include <sys/types.h>  // fd open
#include <sys/stat.h>   // fd open
#include <fcntl.h>      // fd open
#include <unistd.h>     // fd close
#include <time.h>

struct bcm2835_peripheral gpio = { GPIO_BASE };
struct bcm2835_peripheral uart = { UART_BASE };

struct uart_settings dmx_settings = {
    16, 0, UART_LCRH_PARITY_OFF | UART_LCRH_STOP_2 | UART_LCRH_WORDLEN_8
};
struct timespec brk_sleep = { 0, 136000 };
struct timespec mark_sleep = { 0, 12000 };

static inline void delay(int count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}

int map_peripheral(struct bcm2835_peripheral *p)
{
    if ((p->mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        fprintf(stderr, "Failed to open /dev/mem, try checking permissions.\n");
        return -1;
    }

    p->map = mmap(
            NULL,
            0x1000,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
            p->addr_p       // Address in physical map that we want this memory block to expose
            );

    if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    p->addr = (volatile unsigned int *)p->map;

    return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{
    munmap(p->map, 0x1000);
    close(p->mem_fd);
}

void uart_setup(struct uart_settings s) {
	UART_CR = 0x00000000;
	UART_FR = 0x00000000;
	UART_ICR = 0x7FF;
	UART_IBRD = s.baud_div;
	UART_FBRD = s.baud_frac;
	UART_LCRH = UART_LCRH_FIFO_OFF;
	UART_LCRH = s.lcrh;
	UART_IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);
	UART_CR = UART_CR_ENABLE | UART_CR_TXE;
}

int uart_init(void)
{
    int result;

    result = map_peripheral(&gpio);
    if (result) {
        return result;
    }
    result = map_peripheral(&uart);
    if (result) {
        return result;
    }

    	/**-------Procedure to make gpio pins ready to user  -------*/
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	GPPUD = 0x00000000;
	delay(150);

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
	GPPUDCLK0 = (1 << 14) | (1 << 15);
	delay(150);
	// Write 0 to GPPUDCLK0 to make it take effect.
	GPPUDCLK0 =  0x00000000;
	/*--------GPIO Initialization over---------------*/

    uart_setup(dmx_settings);
    return 0;
}


void uart_putc(unsigned char c) {
    while (UART_FR & UART_FR_TXFF); // Wait until there is room for new data in Transmission fifo
    UART_DR = (unsigned char) c; // Write data in transmission fifo
}
void uart_puts(const char* str) {
    size_t i;
    for (i = 0; str[i] != '\0'; i++) {
        uart_putc((unsigned char)str[i]);
    }
}
void uart_putb(unsigned char* data, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) {
        uart_putc((unsigned char)data[i]);
    }
}

void uart_deinit(void) {
    unmap_peripheral(&uart);
    unmap_peripheral(&gpio);
}

void uart_send_break(void) {
    UART_LCRH = UART_LCRH | UART_LCRH_BREAK_ON;
    nanosleep(&brk_sleep, NULL);
    UART_LCRH = UART_LCRH & ~UART_LCRH_BREAK_ON;
    nanosleep(&mark_sleep, NULL);
}

void uart_send_packet(unsigned char* data, size_t length, unsigned char start) {
    uart_send_break();
    uart_putc(start);
    uart_putb(data, length);
    nanosleep(&mark_sleep, NULL);
}

void uart_send_dmx_packet(unsigned char *data, size_t length) {
    uart_send_packet(data, length, 0x00);
}

int main(int argc, char* argv[]) {
    if (uart_init()) {
        return -1;
    }

    FILE* dmx0 = fopen("/dev/dmx0", "rb");
    if (dmx0 <= 0) {
        fprintf(stderr, "Failed to open /dev/dmx0, check if the driver is running.\n");
        return -1;
    }
    unsigned char bufferDmx0[512];
 
    while(1) {
        int read = fread(bufferDmx0, 1, 512, dmx0);
        uart_send_dmx_packet(bufferDmx0, 512);
    }

    uart_deinit();
}