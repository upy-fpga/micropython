#include "py/runtime.h"
#include "py/mphal.h"
#include "spi.h"

typedef struct _rgb_obj_t
{
    mp_obj_base_t base;
} rgb_obj_t;

extern const mp_obj_type_t fomu_spi_obj;

STATIC const rgb_obj_t spi_obj = {
    {{&fomu_spi_obj}},
};

STATIC mp_obj_t spi_make_new(const mp_obj_type_t *type,
                             size_t n_args, size_t n_kw,
                             const mp_obj_t *args)
{
    (void)type;
    (void)n_args;
    (void)n_kw;
    (void)args;
    const rgb_obj_t *self = &spi_obj;
    return MP_OBJ_FROM_PTR(self);
}

__attribute__((section(".ramtext"),noinline))
static uint32_t get_spi_id(void) {
    uint32_t id;
    spiStartBB();
    id = spiId();
    spiEndBB();
    return id;
}

STATIC mp_obj_t spi_id(mp_obj_t self_in)
{
    (void)self_in;
    return mp_obj_new_int_from_uint(get_spi_id());
}
MP_DEFINE_CONST_FUN_OBJ_1(spi_id_obj, spi_id);

STATIC const mp_rom_map_elem_t spi_locals_dict_table[] = {
    // instance methods
    {MP_ROM_QSTR(MP_QSTR_id), MP_ROM_PTR(&spi_id_obj)},
};

STATIC MP_DEFINE_CONST_DICT(spi_locals_dict, spi_locals_dict_table);

const mp_obj_type_t fomu_spi_obj = {
    {&mp_type_type},
    .name = MP_QSTR_spi,
    .make_new = spi_make_new,
    .locals_dict = (mp_obj_t)&spi_locals_dict,
};