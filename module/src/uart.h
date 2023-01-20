#pragma once
#include "constants.h"
#include <stddef.h>

//For RPI 1
// #define BCM2708_PERI_BASE        0x20000000

// For RPI 2, RPI 3
#define BCM2708_PERI_BASE	    0x3F000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)   // GPIO controller
#define UART_BASE			    (BCM2708_PERI_BASE + 0x201000)

// Note that gpio.addr and uart.addr are pointers to unsigned int 
// Hence increment by one result into address 4 Bytes further than current address
// Thats why we need to divide all the offsets by 4.

#define UART_CLOCK              (64000000)
#define GPPUD				    *(gpio.addr + 0x94/4)
#define GPPUDCLK0			    *(gpio.addr + 0x98/4)
#define UART_DR				    *(uart.addr + 0x00/4)
#define UART_RSRECR			    *(uart.addr + 0x04/4)
#define UART_FR				    *(uart.addr + 0x18/4)
#define UART_ILPR			    *(uart.addr + 0x20/4)
#define UART_IBRD			    *(uart.addr + 0x24/4)
#define UART_FBRD			    *(uart.addr + 0x28/4)
#define UART_LCRH			    *(uart.addr + 0x2C/4)
#define UART_CR				    *(uart.addr + 0x30/4)
#define UART_IFLS			    *(uart.addr + 0x34/4)
#define UART_IMSC			    *(uart.addr + 0x38/4)
#define UART_RIS			    *(uart.addr + 0x3C/4)
#define UART_MIS			    *(uart.addr + 0x40/4)
#define UART_ICR			    *(uart.addr + 0x44/4)
#define UART_DMACR			    *(uart.addr + 0x48/4)
#define UART_ITCR			    *(uart.addr + 0x80/4)
#define UART_ITIP			    *(uart.addr + 0x84/4)
#define UART_ITOP			    *(uart.addr + 0x88/4)
#define UART_TDR			    *(uart.addr + 0x8c/4)

#define UART_DR_OE              (1 << 11) // Overrun error
#define UART_DR_BE              (1 << 10) // Break error
#define UART_DR_PE              (1 << 9)  // Parity error
#define UART_DR_FE              (1 << 8)  // Framing error

#define UART_RSRECR_OE          (1 << 3)  // Overrun error
#define UART_RSRECR_BE          (1 << 2)  // Break error
#define UART_RSRECR_PE          (1 << 1)  // Parity error
#define UART_RSRECR_FE          (1 << 0)  // Framing error

#define UART_FR_TXFE            (1 << 7)  // Transmit FIFO empty
#define UART_FR_RXFF            (1 << 6)  // Receive FIFO full
#define UART_FR_TXFF            (1 << 5)  // Transmit FIFO full
#define UART_FR_RXFE            (1 << 4)  // Receive FIFO empty
#define UART_FR_BUSY            (1 << 3)  // Busy
#define UART_FR_CTS             (1 << 1)  // Clear to send

#define UART_LCRH_WORDLEN_5     (0b00 << 5)
#define UART_LCRH_WORDLEN_6     (0b01 << 5)
#define UART_LCRH_WORDLEN_7     (0b10 << 5)
#define UART_LCRH_WORDLEN_8     (0b11 << 5)
#define UART_LCRH_FIFO_ON       (1 << 4)
#define UART_LCRH_FIFO_OFF      (0 << 4)
#define UART_LCRH_STOP_1        (0 << 3)
#define UART_LCRH_STOP_2        (1 << 3)
#define UART_LCRH_PARITY_ODD    (0 << 2)
#define UART_LCRH_PARITY_EVEN   (1 << 2)
#define UART_LCRH_PARITY_ON     (1 << 1)
#define UART_LCRH_PARITY_OFF    (0 << 1)
#define UART_LCRH_BREAK_ON      (1 << 0)

#define UART_CR_CTSEN           (1 << 15)
#define UART_CR_RTSEN           (1 << 14)
#define UART_CR_RTS             (1 << 11)
#define UART_CR_RXE             (1 << 9)
#define UART_CR_TXE             (1 << 8)
#define UART_CR_LBE             (1 << 7)
#define UART_CR_ENABLE          (1 << 0)

struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

struct uart_settings {
    short baud_div;
    short baud_frac;
    int lcrh;
};
