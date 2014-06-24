#ifndef __COLOR_H__
#define __COLOR_H__
#include <stdint.h>

typedef union{
    struct{
        unsigned rgb_r:5;
        unsigned rgb_g:5;
        unsigned rgb_b:5;
        unsigned rgb_valid:1;

        unsigned hsv_h:7;
        unsigned hsv_s:4;
        unsigned hsv_v:4;
        unsigned hsv_valid:1;
    } st;
    uint32_t raw;
} color_t;

#define ASSERT_RGB(c) (c.st.rgb_valid ? 0 : rgb_from_hsv(&c));
#define ASSERT_HSV(c) (c.st.hsv_valid ? 0 : hsv_from_rgb(&c));

void rgb_from_hsv(color_t*);
void hsv_from_rgb(color_t*);

#endif
