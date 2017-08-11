
#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objexcept.h"

#include "generated/csr.h"

#ifndef CSR_CAS_BASE
static inline unsigned char cas_buttons_ev_status_read(void) {
	return 0;
}

static inline void cas_buttons_ev_status_write(unsigned char value) {}

static inline unsigned char cas_buttons_ev_pending_read(void) {
    return 0;
}
static inline void cas_buttons_ev_pending_write(unsigned char value) {}
static inline unsigned char cas_buttons_ev_enable_read(void) {
    return 0;
}
static inline void cas_buttons_ev_enable_write(unsigned char value) {}
#define CAS_NUM_BUTTONS 0
#endif

const mp_obj_type_t litex_button_type;

typedef struct _litex_button_obj_t {
    mp_obj_base_t base;
    int num;
} litex_button_obj_t;


STATIC mp_obj_t litex_button_make_new(const mp_obj_type_t *type_in,
		size_t n_args, size_t n_kw, const mp_obj_t *args) {
	mp_arg_check_num(n_args, n_kw, 1, 1, false);

	mp_uint_t button_num = mp_obj_get_int(args[0]);

    if ((button_num < 0) || (button_num > CAS_NUM_BUTTONS)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
			"not a valid BUTTON number: %d", button_num));
	} else { 
        litex_button_obj_t* button = m_new_obj(litex_button_obj_t);
        button->base.type = type_in;
        button->num = button_num;
        return MP_OBJ_FROM_PTR(button);
    }

}

void litex_button_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
	litex_button_obj_t *self = MP_OBJ_TO_PTR(self_in);
	mp_printf(print, "button(%u)", self->num);
}

STATIC mp_obj_t litex_button_status_read(mp_obj_t self_in) {
	litex_button_obj_t *button = MP_OBJ_TO_PTR(self_in);
	bool state = cas_buttons_ev_status_read() & (1 << (button->num - 1));

	return mp_obj_new_bool(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_button_read_status_obj, litex_button_status_read);

STATIC mp_obj_t litex_button_was_pressed(size_t n_args, const mp_obj_t *args) {
    litex_button_obj_t *button = MP_OBJ_TO_PTR(args[0]);
    //work out if we should reset the read, default is true
    bool reset = (n_args < 2) ? true : mp_obj_is_true(args[1]);

    bool state = cas_buttons_ev_pending_read() & (1 << (button->num - 1));

    if (reset) {
        cas_buttons_ev_pending_write(1 << (button->num - 1));
    }

    return mp_obj_new_bool(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(litex_button_was_pressed_obj, 1, 2, litex_button_was_pressed);

STATIC const mp_rom_map_elem_t litex_button_locals_dict_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR_status), MP_ROM_PTR(&litex_button_read_status_obj) },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_was_pressed), MP_ROM_PTR(&litex_button_was_pressed_obj) },

    //class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_NUM_BUTTONS), MP_OBJ_NEW_SMALL_INT(CAS_NUM_BUTTONS) },
};
STATIC MP_DEFINE_CONST_DICT(litex_button_locals_dict, litex_button_locals_dict_table);

const mp_obj_type_t litex_button_type = {
	{ &mp_type_type },
	.name = MP_QSTR_button,
	.print = litex_button_print,
	.make_new = litex_button_make_new,
	.locals_dict = (mp_obj_t)&litex_button_locals_dict,
};
