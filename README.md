Opalesce
========

Color Datatype
--------------

Store both HSV & RGB data in a single 32-bit word.

    typedef struct{
        unsigned hsv_valid:1;
        unsigned hsv_h:7;
        unsigned hsv_s:4;
        unsigned hsv_v:3;

        unsigned rgb_valid:1;
        unsigned rgb_r:5;
        unsigned rgb_g:5;
        unsigned rgb_b:5;
    } color;

Opcodes
-------

32 bits -> ~ 4-6 for op, then 13 for source/dest

- Arithmetic - 7
    - ADD, ADDS, {ADDC, ADDCS}, {SUB, SUBC}, {SUBS, SUBCS}, {MUL, MULS}, {DIV, DIVS}
- Bitwise - 8
    - AND, OR, XOR, MVNT, {BSET, BSETS}, {BCLR, BCLRS}, {SHR, SHRS}, {SHL, SHLS} 
- Color - 2 (24)
    - GET[RGBHSVXY], {PUT[RGBHSVXY], PUTS[RGBHSVXY]}
        - 5-opcode, 3-subop, 9-tgt, 15-data = 32
- Compare - 2
    - {CMP, CMPS}, TST
- Branching - 5
    - BEQ, BNEQ, JMP, CALL, RET
- Memory - 4
    - MV, {LD, LDC, LDCH}, {PUSH, POP}
- IO - 0
    - {ASEND, ARECV}
- Other - 0
    - {NOP, HALT, END, SPEC} +(PUSH, POP, ASEND, ARECV)

Total: 28
- Operations to add?
    - INCSZ/DECSZ - skip if zero
    - BTSS/BTSC - bit test skip if set/clear


Callstack size: 4?

ADD r1, r2, r3 ; r1 <- r2 + r3
ADDC r1, r2, C ; r1 <- r2 + C

Opcode: 5
Ref: 9

