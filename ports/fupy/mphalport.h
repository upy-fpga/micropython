#include "uart.h"
#include "lib/utils/interrupt_char.h"

static inline mp_uint_t mp_hal_ticks_ms(void) { return 0; }


// Receive single character
static inline int mp_hal_stdin_rx_chr(void) {
    return uart_read();
}

// Send string of given length
static inline void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
               uart_write(*str++);
    }
}
