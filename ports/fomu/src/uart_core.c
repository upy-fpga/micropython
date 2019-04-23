#include <unistd.h>
#include "py/mpconfig.h"
#include "usb.h"

/*
 * Core UART functions to implement for a port
 */

int usb_getc(void);
void usb_putc(uint8_t c);
int usb_can_getc(void);
int usb_can_putc(void);

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    do {
        usb_poll();
    } while (!usb_can_getc());
    return usb_getc();
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        do {
            usb_poll();
        } while (!usb_can_putc());
        // wait for TXE
        usb_putc(*str++);
    }
}
