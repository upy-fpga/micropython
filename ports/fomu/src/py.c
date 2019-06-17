#include "py/runtime.h"

extern const mp_obj_type_t machine_rgb_obj;

STATIC const mp_rom_map_elem_t fomu_module_globals_table[] = {
     { MP_ROM_QSTR(MP_QSTR_rgb), MP_ROM_PTR(&machine_rgb_obj) },
};

STATIC MP_DEFINE_CONST_DICT(fomu_module_globals, fomu_module_globals_table);

const mp_obj_module_t mp_module_fomu = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&fomu_module_globals,
};