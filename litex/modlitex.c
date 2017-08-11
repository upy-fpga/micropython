#include "py/obj.h"

extern const mp_obj_type_t litex_led_type;
extern const mp_obj_type_t litex_switch_type;
extern const mp_obj_type_t litex_button_type;

STATIC const mp_rom_map_elem_t litex_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_litex) },

    { MP_ROM_QSTR(MP_QSTR_Led), MP_ROM_PTR(&litex_led_type) },
    { MP_ROM_QSTR(MP_QSTR_Switch), MP_ROM_PTR(&litex_switch_type) },
    { MP_ROM_QSTR(MP_QSTR_Button), MP_ROM_PTR(&litex_button_type) },
};

STATIC MP_DEFINE_CONST_DICT(litex_module_globals, litex_module_globals_table);

const mp_obj_module_t mp_module_litex = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&litex_module_globals,
};
