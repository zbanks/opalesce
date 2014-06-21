#ifndef __COLOR_H__
#define __COLOR_H__
#include <stdint.h>

typedef union{
    struct{
        unsigned hsv_valid:1;
        unsigned hsv_h:7;
        unsigned hsv_s:4;
        unsigned hsv_v:3;

        unsigned rgb_valid:1;
        unsigned rgb_r:5;
        unsigned rgb_g:5;
        unsigned rgb_b:5;
    } st;
    uint32_t raw;
} color_t;

#define ASSERT_RGB(c) (c.rgb_valid || rgb_from_hsv(&c));
#define ASSERT_HSV(c) (c.hsv_valid || hsv_from_rgb(&c));

void rgb_from_hsv(color_t*);
void hsv_from_rgb(color_t*);

#endif
