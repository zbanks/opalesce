#ifndef __OPALESCE_H__
#define __OPALESCE_H__

#include <stdint.h>

#define NUM_PIXELS 80       // 8 bits
#define FILTER_SIZE 64      // 8 bits
#define NUM_FILTERS 16      // 4 bits
#define NUM_FRAMEBUFFERS 4  // 2 bits

// 9 bits of ref
// # registers = 16
// 8 7654 3210
// 0 0000 xxxx - READONLY Special[`xxxx`]
// 0 0001 **** - READONLY ???[`xxxx`]
// 0 0010 rrrr - Regs[`xxxx`]
// 0 0011 rrrr - Program[Regs[`rrrr`]]
// 0 01ff rrrr - Framebuffer[`ff`][Regs[`rrrr`]]
// 0 1*** **** - 
// 1 xxxx xxxx - Program[`xxxxxx`] (exact)
//
/*

   4 - R[] 
   6=2+4  - FB[R[]]
   4 - P[R[]]
   4 - STAT[] RO
   6-8 - P[]

*/


#define TICK_LENGTH  240

// print NUM_PIXELS * NUM_FRAMEBUFFERS * sizeof(color_t) + NUM_FILTERS * FILTER_SIZE * sizeof(uint32_t) < 8kb
// 80 * 4 * 4 + 16 * 64 * 4 = 5376 bytes

typedef struct{
    unsigned hsv_valid:1;
    unsigned hsv_h:7;
    unsigned hsv_s:4;
    unsigned hsv_v:3;

    unsigned rgb_valid:1;
    unsigned rgb_r:5;
    unsigned rgb_g:5;
    unsigned rgb_b:5;
} color_t;

typedef color_t[NUM_PIXELS] framebuffer_t;
typedef uint32_t[FILTER_SIZE] filter_t;

// Array of all framebuffers
extern framebuffer_t framebuffers[];
// Array of all filters
extern filter_t filters[];

// Fractional Tick, out of TICK_LENGTH. If TICK_LENGTH >= 255, use uint16_t
typedef uint8_t fractick_t; // sizeof(fractick_t) > TICK_LENGTH
// Position on light strip (in pixels)
typedef uint8_t position_t; // sizeof(position_t) > STRIP_LENGTH
// Boolean
typedef uint8_t bool_t;

// Timekeeping
typedef struct tick_t {
    uint32_t tick:24;
    fractick_t frac:8;
} tick_t;

extern tick_t clock;

/*
- Arithmetic - 10
    - ADD, ADDC, SUB, MUL, MULC, DIV, SHR, SHL, INC, DEC
- Color - 12
    - GET[RGBHSV], PUT[RGBHSV]
- Branching - 3+
    - LOOPN, BEQ, BNEQ, JMP
- Memory - 2+
    - MV, LD, MVFB
- Other
    - NOP, HALT, POP
*/

#define OPL_NUM_REGS 16
#define OPL_MAX_RUNTIME 1024

#define OPL_OPOFFSET 27
#define OPL_OPMASK   (0x1f << 27)
#define OPL_OP_NOP   (0x00 << OPL_OPOFFSET)
#define OPL_OP_HALT   (0x01 << OPL_OPOFFSET)

typedef struct{
    // TODO
    unsigned op:5;
    unsigned cons:9;
    unsigned source:9;
    unsigned dest:9;
} opcode_t;

typedef struct{
    uint32_t r[OPL_NUM_REGS];
    uint32_t pc;
    uint32_t time;
    uint32_t beat;
    uint32_t tick;
    uint32_t loopc;
    uint32_t fblen;
} op_regs_t;

#endif
