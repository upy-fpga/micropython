#include <string.h>
#include <unistd.h>
#include "py/mpconfig.h"
#include "py/mphal.h"

/*
 * Extra stdout functions
 * These can be either optimized for a particular port, or reference
 * implementation below can be used.
 */

// Send "cooked" string of given length, where every occurrence of
// LF character is replaced with CR LF.
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    static uint8_t cooked_buffer[64];
    uint32_t cooked_buffer_len = 0;
    while (len--) {
        if (*str == '\n') {
            cooked_buffer[cooked_buffer_len++] = '\r';
            if (cooked_buffer_len >= sizeof(cooked_buffer)) {
                mp_hal_stdout_tx_strn(cooked_buffer, cooked_buffer_len);
                cooked_buffer_len = 0;
            }
        }
        cooked_buffer[cooked_buffer_len++] = *str++;
        if (cooked_buffer_len >= sizeof(cooked_buffer)) {
            mp_hal_stdout_tx_strn(cooked_buffer, cooked_buffer_len);
            cooked_buffer_len = 0;
        }
    }

    if (cooked_buffer_len) {
        mp_hal_stdout_tx_strn(cooked_buffer, cooked_buffer_len);
    }
}

// Send zero-terminated string
void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}
