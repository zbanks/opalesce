#include "color.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x < y ? x : y)

void hsv_from_rgb(color_t* c){
    //TODO: Double check this
    int16_t h;
    uint8_t s, v;
    uint8_t r, g, b;

    r = (c->st.rgb_r << 3) | (c->st.rgb_r >> 2);
    g = (c->st.rgb_g << 3) | (c->st.rgb_g >> 2);
    b = (c->st.rgb_b << 3) | (c->st.rgb_b >> 2);

    
    uint8_t min = MIN(r, MIN(g, b));
    uint8_t max = MAX(r, MAX(g, b));
    v = max;
    uint8_t delta = max - min;
    if(max > 0){
        s = delta / max;
        if(r >= max){
            h = (g - b);
        }else if(g >= max){
            h = (b - r) + (256 / 12);
        }else{
            h = (r - g) + (256 / 6);
        }
        h = (h * 256 / 6) / delta;
        if(h < 0){
            h = (h + 0x100);
        }
    }else{
        // Black
        s = 0;
        h = 0;
    }
    c->st.hsv_h = (uint8_t) (h & 0xff);
    c->st.hsv_s = s;
    c->st.hsv_v = v;
    c->st.hsv_valid = 1;
}

void rgb_from_hsv(color_t* c){
    // Convert HSV to RGB.  Hue from 0-127, sat 4 bit, val 3 bit
    // TODO: Optimize for the values we actually have
    uint8_t r, g, b;

    // Convert sub-8 bit values into 8 bit values
    uint8_t hue = (c->st.hsv_h << 1) | (c->st.hsv_h >> 6);
    uint8_t sat = (c->st.hsv_s << 4) | (c->st.hsv_s);
    uint8_t val = (c->st.hsv_v << 5) | (c->st.hsv_v << 1) | (c->st.hsv_v >> 2);


    if (hue == 255) hue = 254;
    uint16_t chroma = (val) * (sat);
    uint16_t m = 255*(val) - chroma;
    signed long X =(42-abs((hue)%85-42));
    X *= chroma;
    
    uint8_t x8b = X/(255 * 42);
    uint8_t c8b = chroma/255;
    uint8_t m8b = m/255;
    
    if (hue < 42) {
        r = c8b + m8b;
        g = x8b + m8b;
        b = m8b;
    } else if (hue < 84) {
        r = x8b + m8b;
        g = c8b + m8b;
        b = m8b;
    } else if (hue < 127) {
        r = m8b;
        g = c8b + m8b;
        b = x8b + m8b;
    } else if (hue < 169) {
        r = m8b;
        g = x8b + m8b;
        b = c8b + m8b;
    } else if (hue < 212) {
        r = x8b + m8b;
        g = m8b;
        b = c8b + m8b;
    } else {
        r = c8b + m8b;
        g = m8b;
        b = x8b + m8b;
    }

    c->st.rgb_r = r >> 3;
    c->st.rgb_g = g >> 3;
    c->st.rgb_b = b >> 3;
    c->st.rgb_valid = 1;
}
