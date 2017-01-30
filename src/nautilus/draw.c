#include <nautilus/draw.h>
#include <nautilus/intrinsics.h>


inline void draw_line(uint32_t x_st, uint32_t x_end, uint32_t y_st, uint32_t y_end, color color){
        sint32_t delta_x;
        sint32_t delta_y;

        uint32_t curr_x;
        uint32_t curr_y;

        sint32_t prev_error;
        sint32_t error;

        sint8_t incx;
        sint8_t incy;

        if(x_st < x_end){
                delta_x = x_end - x_st;
                incx = 1;
        }
        else{
                delta_x = x_st- x_end;
                incx = -1;
        }

        if(y_st < y_end){
                delta_y = y_end - y_st;
                incy = 1;
        }
        else{
                delta_y = y_st - y_end;
                incy = -1;
        }

        if (delta_x > delta_y){
                error = delta_x / 2;
        }
        else{
                error = -delta_y / 2;
        }

        curr_x = x_st;
        curr_y = y_st;

        uint16_t n,i,j;
        vesa_draw_pixel(curr_x, curr_y, color.r, color.g, color.b);

        while(curr_x != x_end || curr_y != y_end){
                vesa_draw_pixel(curr_x, curr_y, color.r, color.g, color.b);

                prev_error = error;

                if (prev_error > -delta_x){
                        error -= delta_y;
                        curr_x += incx;
                }
                if(prev_error < delta_y){
                        error +=  delta_x;
                        curr_y +=  incy;
                }


        }
}
