
#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objexcept.h"

#include "generated/csr.h"

#ifndef CSR_CAS_BASE
static inline unsigned char cas_leds_out_read(void) {
	return 0;
}

static inline void cas_leds_out_write(unsigned char value) {
}
#define CAS_NUM_LEDS 0
#endif

const mp_obj_type_t litex_led_type;

typedef struct _litex_led_obj_t {
    mp_obj_base_t base;
    mp_uint_t num;
} litex_led_obj_t;

STATIC mp_obj_t litex_led_make_new(const mp_obj_type_t *type_in,
		size_t n_args, size_t n_kw, const mp_obj_t *args) {
	mp_arg_check_num(n_args, n_kw, 1, 1, false);
	mp_uint_t led_num = mp_obj_get_int(args[0]);

	if ((led_num < 1) || (led_num > CAS_NUM_LEDS)) {
		nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
			"not a valid LED number: %d", led_num));
	} else {
	    litex_led_obj_t* led = m_new_obj(litex_led_obj_t);
        led->base.type = type_in;
	    led->num = led_num;
        return MP_OBJ_FROM_PTR(led);
    }
}

void litex_led_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
	litex_led_obj_t *self = MP_OBJ_TO_PTR(self_in);
	mp_printf(print, "LED(%u)", self->num);
}

STATIC mp_obj_t litex_led_read(mp_obj_t self_in) {
	litex_led_obj_t *led = MP_OBJ_TO_PTR(self_in);
	bool state = cas_leds_out_read() & (1 << (led->num - 1));

	return mp_obj_new_bool(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_led_read_obj, litex_led_read);

STATIC mp_obj_t litex_led_on(mp_obj_t self_in) {
	litex_led_obj_t *led = MP_OBJ_TO_PTR(self_in);
	char value = cas_leds_out_read();

	cas_leds_out_write(value | (1 << (led->num - 1)));

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_led_on_obj, litex_led_on);

STATIC mp_obj_t litex_led_off(mp_obj_t self_in) {
	litex_led_obj_t *led = MP_OBJ_TO_PTR(self_in);
	char value = cas_leds_out_read();

	cas_leds_out_write(value & ~(1 << (led->num - 1)));

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_led_off_obj, litex_led_off);


STATIC const mp_rom_map_elem_t litex_led_locals_dict_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR_read), MP_ROM_PTR(&litex_led_read_obj) },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_on), MP_ROM_PTR(&litex_led_on_obj) },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_off), MP_ROM_PTR(&litex_led_off_obj) },

    //class constatnts
    { MP_OBJ_NEW_QSTR(MP_QSTR_NUM_LEDS), MP_OBJ_NEW_SMALL_INT(CAS_NUM_LEDS) },
};
STATIC MP_DEFINE_CONST_DICT(litex_led_locals_dict, litex_led_locals_dict_table);

const mp_obj_type_t litex_led_type = {
	{ &mp_type_type },
	.name = MP_QSTR_LED,
	.print = litex_led_print,
	.make_new = litex_led_make_new,
	.locals_dict = (mp_obj_t)&litex_led_locals_dict,
};
