#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"

#include "generated/csr.h"
#include "irq.h"
#include "time.h"
#include "rgb.h"
#include "usb.h"

#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

static char *stack_top;

__attribute__((section(".ramtext")))
void isr(void) {
    unsigned int irqs;

    irqs = irq_pending() & irq_getmask();

    if (irqs & (1 << USB_INTERRUPT))
        usb_isr();
}

static void fomu_init(void) {
    irq_setmask(0);
    irq_setie(1);
    usb_init();
    time_init();
    rgb_init();
    usb_connect();
    while (!usb_can_putc())
        usb_poll();
}

static void python_main(void) {
    int stack_dummy;
    extern uint32_t _heap_start;
    stack_top = (char*)&stack_dummy;

    #if MICROPY_ENABLE_GC
    {
        uint32_t *start_ptr = (uint32_t *)&_heap_start;
        uint32_t *end_ptr = (uint32_t *)(((uint32_t)start_ptr) + 65536);
        while (start_ptr < end_ptr) {
            *start_ptr++ = 0;
        }
    }
    gc_init((char*)&_heap_start , (char*)(&_heap_start) + 65536);
    #endif
    mp_init();
    #if MICROPY_ENABLE_COMPILER
    #if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    for (;;) {
        int c = mp_hal_stdin_rx_chr();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
    #else
    pyexec_friendly_repl();
    #endif
    //do_str("print('hello world!', list(x+1 for x in range(10)), end='eol\\n')", MP_PARSE_SINGLE_INPUT);
    //do_str("for i in range(10):\r\n  print(i)", MP_PARSE_FILE_INPUT);
    #else
    pyexec_frozen_module("frozentest.py");
    #endif
    mp_deinit();
}

void main(void) {
    fomu_init();
    while (1) {
        // while (!usb_can_getc())
        //     usb_poll();
        // int c = usb_getc();
        // usb_putc(c ^ 0x20);
        // usb_poll();
        python_main();
    }
}

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    (void)filename;
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    (void)path;
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    (void)n_args;
    (void)args;
    (void)kwargs;
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
    while (1);
}

void NORETURN __fatal_error(const char *msg) {
    while (1);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif