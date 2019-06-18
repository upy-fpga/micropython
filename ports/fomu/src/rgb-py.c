#include "py/runtime.h"
#include "py/mphal.h"
#include "rgb.h"

#include <strings.h>

typedef struct _rgb_obj_t
{
    mp_obj_base_t base;
} rgb_obj_t;

extern const mp_obj_type_t fomu_rgb_obj;

STATIC const rgb_obj_t rgb_obj = {
    {{&fomu_rgb_obj}},
};

STATIC mp_obj_t rgb_make_new(const mp_obj_type_t *type,
                             size_t n_args, size_t n_kw,
                             const mp_obj_t *args)
{
    (void)type;
    (void)n_args;
    (void)n_kw;
    (void)args;
    // mp_arg_check_num(n_args, n_kw, 1, 1, true);
    const rgb_obj_t *self = &rgb_obj;
    // if (!self)
    //     mp_raise_ValueError("invalid pin for touchpad");

    rgb_init();

    // esp_err_t err = touch_pad_config(self->touchpad_id, 0);
    return MP_OBJ_FROM_PTR(self);
    // mp_raise_ValueError("Touch pad error");
}

STATIC mp_obj_t rgb_mode(mp_obj_t self_in, mp_obj_t mode_name)
{
    (void)self_in;
    const char *mode_str = mp_obj_str_get_str(mode_name);
    if (!strcasecmp(mode_str, "idle")) {
        rgb_mode_idle();
        return mp_const_none;
    }
    else if (!strcasecmp(mode_str, "done")) {
        rgb_mode_done();
        return mp_const_none;
    }
    else if (!strcasecmp(mode_str, "writing")) {
        rgb_mode_writing();
        return mp_const_none;
    }
    else if (!strcasecmp(mode_str, "error")) {
        rgb_mode_error();
        return mp_const_none;
    }
    nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_TypeError,
        "unrecognized mode \"%s\". must be one of idle, done, writing, or error",
        mode_str));
    // esp_err_t err = touch_pad_config(self->touchpad_id, value);
    // if (err == ESP_OK)
    //     return mp_const_none;
    mp_raise_ValueError("Touch pad config error");
}
MP_DEFINE_CONST_FUN_OBJ_2(rgb_mode_obj, rgb_mode);

STATIC mp_obj_t rgb_write_raw(mp_obj_t self_in, mp_obj_t addr_obj, mp_obj_t val_obj)
{
    (void)self_in;
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t value = mp_obj_get_int(val_obj);
    rgb_write(value, addr);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_3(rgb_write_raw_obj, rgb_write_raw);

STATIC mp_obj_t rgb_read_raw(mp_obj_t self_in, mp_obj_t addr_obj)
{
    (void)self_in;
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t value = rgb_read(addr);
    return mp_obj_new_int(value);
}
MP_DEFINE_CONST_FUN_OBJ_2(rgb_read_raw_obj, rgb_read_raw);

STATIC const mp_rom_map_elem_t rgb_locals_dict_table[] = {
    // instance methods
    {MP_ROM_QSTR(MP_QSTR_mode), MP_ROM_PTR(&rgb_mode_obj)},
    {MP_ROM_QSTR(MP_QSTR_write_raw), MP_ROM_PTR(&rgb_write_raw_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_raw), MP_ROM_PTR(&rgb_read_raw_obj)},
};

STATIC MP_DEFINE_CONST_DICT(rgb_locals_dict, rgb_locals_dict_table);

const mp_obj_type_t fomu_rgb_obj = {
    {&mp_type_type},
    .name = MP_QSTR_rgb,
    .make_new = rgb_make_new,
    .locals_dict = (mp_obj_t)&rgb_locals_dict,
};