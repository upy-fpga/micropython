
#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/objexcept.h"

#include "generated/csr.h"

#ifndef CSR_CAS_BASE
static inline unsigned char cas_switches_in_read(void) {
    return 0;
}
#endif

const mp_obj_type_t litex_switch_type;

typedef struct _litex_switch_obj_t {
    mp_obj_base_t base;
    int num;
} litex_switch_obj_t;

STATIC litex_switch_obj_t litex_switches[8] = {
	{{&litex_switch_type}, 1},
	{{&litex_switch_type}, 2},
	{{&litex_switch_type}, 3},
	{{&litex_switch_type}, 4},
	{{&litex_switch_type}, 5},
	{{&litex_switch_type}, 6},
	{{&litex_switch_type}, 7},
	{{&litex_switch_type}, 8}
};

STATIC mp_obj_t litex_switch_make_new(const mp_obj_type_t *type_in,
		size_t n_args, size_t n_kw, const mp_obj_t *args) {
	mp_arg_check_num(n_args, n_kw, 1, 1, false);

	mp_uint_t switch_num = mp_obj_get_int(args[0]);

	switch (switch_num) {
	case 1 ... 8:
		return &litex_switches[switch_num - 1];
	default:
		nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
			"not a valid SWITCH number: %d", switch_num));
	}
}

void litex_switch_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
	litex_switch_obj_t *self = self_in;
	mp_printf(print, "SWITCH(%u)", self->num);
}

STATIC mp_obj_t litex_switch_read(mp_obj_t self_in) {
	litex_switch_obj_t *sw = self_in;
	bool state = cas_switches_in_read() & (1 << (sw->num - 1));

	return mp_obj_new_bool(state);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(litex_switch_read_obj, litex_switch_read);



STATIC const mp_map_elem_t litex_switches_locals_dict_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR_read), (mp_obj_t)&litex_switch_read_obj },
};
STATIC MP_DEFINE_CONST_DICT(litex_switches_locals_dict, litex_switches_locals_dict_table);

const mp_obj_type_t litex_switch_type = {
	{ &mp_type_type },
	.name = MP_QSTR_SWITCH,
	.print = litex_switch_print,
	.make_new = litex_switch_make_new,
	.locals_dict = (mp_obj_t)&litex_switches_locals_dict,
};
