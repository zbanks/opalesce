#ifndef __OPALESCE_H__
#define __OPALESCE_H__

#include <stdint.h>
#include "color.h"

#define NUM_PIXELS 128      // 8 bits
#define NUM_PIXELS_MASK 0x7f
#define FILTER_SIZE 64      // 8 bits
#define FILTER_SIZE_MASK 0x3f
#define NUM_FILTERS 16      // 4 bits
#define NUM_FILTERS_MASK 0xf
#define NUM_FRAMEBUFFERS 4  // 2 bits
#define NUM_FRAMEBUFFERS_MASK 0x3


#define TICK_LENGTH  240

// print NUM_PIXELS * NUM_FRAMEBUFFERS * sizeof(color_t) + NUM_FILTERS * FILTER_SIZE * sizeof(uint32_t) < 8kb
// 80 * 4 * 4 + 16 * 64 * 4 = 5376 bytes

//typedef color_t framebuffer_t[NUM_PIXELS];
//typedef uint32_t[FILTER_SIZE] filter_t;

// Array of all framebuffers
//extern framebuffer_t framebuffers[];
// Array of all filters
//extern filter_t filters[];
//

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
#define OPL_MAX_RUNTIME (1 << 12)

#define OPL_OPOFFSET 27
#define OPL_OPMASK     (0x1f << 27)
#define OPL_OP_TWOI    (0x00 << OPL_OPOFFSET)
#define OPL_OP_NOP     (OPL_OP_TWOI | (0x00 << 18))
#define OPL_OP_MVNT    (OPL_OP_TWOI | (0x01 << 18))
#define OPL_OP_CALL    (OPL_OP_TWOI | (0x02 << 18))
#define OPL_OP_RET     (OPL_OP_TWOI | (0x03 << 18))
#define OPL_OP_HALT    (OPL_OP_TWOI | (0x04 << 18))
#define OPL_OP_DEBUG   (OPL_OP_TWOI | (0x05 << 18))
#define OPL_OP_POP     (OPL_OP_TWOI | (0x06 << 18))
#define OPL_OP_PUSH    (OPL_OP_TWOI | (0x07 << 18))
#define OPL_OP_INCSQ   (OPL_OP_TWOI | (0x08 << 18))
#define OPL_OP_DECSQ   (OPL_OP_TWOI | (0x09 << 18))
#define OPL_OP_BTSS    (OPL_OP_TWOI | (0x0A << 18))
#define OPL_OP_BTSC    (OPL_OP_TWOI | (0x0B << 18))
#define OPL_OP_JMP     (OPL_OP_TWOI | (0x0C << 18))
#define OPL_OP_JMPO    (OPL_OP_TWOI | (0x0D << 18))
#define OPL_OP_END     (OPL_OP_TWOI | (0xFF << 18))
// 0x01
// 0x02
// 0x03
#define OPL_OP_MV      (0x04 << OPL_OPOFFSET)
#define OPL_OP_LD      (OPL_OP_MV | (0x01 << 25))
#define OPL_OP_LDL     (OPL_OP_MV | (0x02 << 25))
#define OPL_OP_LDH     (OPL_OP_MV | (0x03 << 25))
#define OPL_OP_CGET    (0x05 << OPL_OPOFFSET)
#define OPL_OP_CPUT    (0x06 << OPL_OPOFFSET)
#define OPL_OP_CPUTS   (OPL_OP_CPUT | (1 << 23))
#define OPL_OP_BEQ     (0x07 << OPL_OPOFFSET)
#define OPL_OP_BNEQ    (OPL_OP_BEQ | (0x01 << 26))
// 0x08
// 0x09
// 0x0A
// 0x0B
// 0x0C
// 0x0D
// 0x0E
// 0x0F
#define OPL_OP_ADD     (0x10 << OPL_OPOFFSET)
#define OPL_OP_SUB     (0x11 << OPL_OPOFFSET)
#define OPL_OP_MUL     (0x12 << OPL_OPOFFSET)
#define OPL_OP_DIV     (0x13 << OPL_OPOFFSET)
#define OPL_OP_ADDS    (0x14 << OPL_OPOFFSET)
#define OPL_OP_SUBS    (OPL_OP_ADDS | (0x01 << 26))
#define OPL_OP_MULS    (0x15 << OPL_OPOFFSET)
#define OPL_OP_DIVS    (OPL_OP_MULS | (0x01 << 26))
#define OPL_OP_AND     (0x16 << OPL_OPOFFSET)
#define OPL_OP_OR      (0x17 << OPL_OPOFFSET)
#define OPL_OP_XOR     (0x18 << OPL_OPOFFSET)
#define OPL_OP_SHL     (0x19 << OPL_OPOFFSET)
#define OPL_OP_SHR     (0x1A << OPL_OPOFFSET)
// 0x1B
#define OPL_OP_BSET    (0x1C << OPL_OPOFFSET)
#define OPL_OP_BCLR    (0x1D << OPL_OPOFFSET)
#define OPL_OP_CMP     (0x1E << OPL_OPOFFSET)
#define OPL_OP_CMPS    (OPL_OP_CMP | (0x01 << 26))

#define OPL_C_H        (0x00 << 24)
#define OPL_C_S        (0x01 << 24)
#define OPL_C_V        (0x02 << 24)
#define OPL_C_HSV      (0x03 << 24)
#define OPL_C_R        (0x04 << 24)
#define OPL_C_G        (0x05 << 24)
#define OPL_C_B        (0x06 << 24)
#define OPL_C_RGB      (0x07 << 24)
#define OPL_C_MASK     (OPL_C_RGB)

// 9 bits of ref
// # registers = 16
// 8 7654 3210
// 0 0000 xxxx - READONLY Special[`xxxx`]
// 0 0001 **** - READONLY Unused[`xxxx`]
// 0 0010 rrrr - Regs[`xxxx`]
// 0 0011 rrrr - Program[Regs[`rrrr`]]
// 0 01ff rrrr - Framebuffer[`ff`][Regs[`rrrr`]]
// 0 1*** **** - Unused
// 1 xxxx xxxx - Program[`xxxxxx`] (exact)

#define OPL_A_SP      (0x00)
#define OPL_A_REG     (0x20)
#define OPL_A_PREG    (0x30)
#define OPL_A_FREG    (0x40)
#define OPL_A_F0REG   (0x40)
#define OPL_A_F1REG   (0x50)
#define OPL_A_F2REG   (0x60)
#define OPL_A_F3REG   (0x70)
#define OPL_A_PROG    (0x100)

#define _AREG(r) (OPL_A_REG | r)
#define _APREG(r) (OPL_A_PREG | r)
#define _AFREG(f, r) (OPL_A_FREG | r | (f << 4))
#define _APROG(l) (OPL_A_PROG | l)
#define _ASPEC(s) (OPL_A_SP | s)
#define _ZERO     (0x10) // Currently unused


#define _D(x) (x << 0)
#define _S(x) (x << 9)
#define _L(x) (x << 18)
#define _O(x) (x << 27)

#define _gD(x) (x & 0x1FF)
#define _gS(x) ((x >> 9) & 0x1FF)
#define _gL(x) ((x >> 18) & 0x1FF)
#define _gO(x) ((x >> 27) & 0x1F)

typedef uint32_t opcode_t;

typedef struct{
    uint32_t r[OPL_NUM_REGS];
    uint32_t pc;      // Program Counter
    uint32_t time;    // "Cycles" the program has been running
    uint32_t beat;    // Beat (timing)
    uint32_t tick;    // Tick (timing)
    uint32_t sp;      // Stack Pointer
    uint32_t fblen;   // Length of each framebuffer (constant)
    uint32_t cycles;  // CPU cycles used (estimate, debugging)
    uint32_t _unused;
} op_regs_t;

#define OPL_S_PC       (0)
#define OPL_S_TIME     (1)
#define OPL_S_BEAT     (2)
#define OPL_S_TICK     (3)
#define OPL_S_SP       (4)
#define OPL_S_FBLEN    (5)
#define OPL_S_CYCLES   (6)

#define OPL_PGM_RUN    (1 << 0)

extern color_t framebuffers[NUM_FRAMEBUFFERS][NUM_PIXELS];
extern opcode_t filters[NUM_FILTERS][FILTER_SIZE];

void oplaesce_extract_rgb(uint16_t *);

#endif
