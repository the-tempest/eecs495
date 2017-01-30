#ifndef DRAW_H
#define DRAW_H

#include <nautilus/intrinsics.h>
#include <nautilus/naut_types.h>
#include <dev/vesa.h>

typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
} color;


inline void draw_line(uint32_t x_st, uint32_t x_end, uint32_t y_st, uint32_t y_end, color color);

#endif /* DRAW_H */
