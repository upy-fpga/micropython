#ifndef _RGB_H_
#define _RGB_H_

#include <stdint.h>

void rgb_init(void);
void rgb_mode_idle(void);
void rgb_mode_done(void);
void rgb_mode_writing(void);
void rgb_mode_error(void);
void rgb_write(uint8_t value, uint8_t addr);

#endif /* _RGB_H_ */