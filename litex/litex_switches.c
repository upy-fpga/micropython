
#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objexcept.h"

#include "generated/csr.h"

#ifndef CSR_CAS_BASE
static inline void cas_switches_in_read(void) {
}
#define CAS_NUM_SWITCHES 0 
#endif

const mp_obj_type_t litex_switch_type;

typedef struct _litex_switch_obj_t {
    mp_obj_base_t base;
    int num;
} litex_switch_obj_t;

STATIC mp_obj_t litex_switch_make_new(const mp_obj_type_t *type_in,
		size_t n_args, size_t n_kw, const mp_obj_t *args) {
	mp_arg_check_num(n_args, n_kw, 1, 1, false);

	mp_uint_t switch_num = mp_obj_get_int(args[0]);

    if ((switch_num < 1) || (switch_num > CAS_NUM_SWITCHES)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
			"not a valid SWITCH number: %d", switch_num));
	} else {
        litex_switch_obj_t* sw = m_new_obj(litex_switch_obj_t);
        sw->base.type = type_in;
        sw->num = switch_num;
        return MP_OBJ_FROM_PTR(sw);
    }
}

void litex_switch_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
	litex_switch_obj_t *self = MP_OBJ_TO_PTR(self_in);
	mp_printf(print, "Switch(%u)", self->num);
}

STATIC mp_obj_t litex_switch_read(mp_obj_t self_in) {
	litex_switch_obj_t *sw = MP_OBJ_TO_PTR(self_in);
	bool state = cas_switches_in_read() & (1 << (sw->num - 1));

	return mp_obj_new_bool(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_switch_read_obj, litex_switch_read);



STATIC const mp_rom_map_elem_t litex_switches_locals_dict_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR_read), MP_ROM_PTR(&litex_switch_read_obj) },

    //class constants
    { MP_OBJ_NEW_QSTR(MP_QSTR_NUM_SWITCHES), MP_OBJ_NEW_SMALL_INT(CAS_NUM_SWITCHES) },
};
STATIC MP_DEFINE_CONST_DICT(litex_switches_locals_dict, litex_switches_locals_dict_table);

const mp_obj_type_t litex_switch_type = {
	{ &mp_type_type },
	.name = MP_QSTR_SWITCH,
	.print = litex_switch_print,
	.make_new = litex_switch_make_new,
	.locals_dict = (mp_obj_t)&litex_switches_locals_dict,
};
