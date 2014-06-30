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

 # - only has 2 operands

 Need to still write:
    - BSETS, BCLRS
    - TST
    - ASEND, ARECV
    - END, SPEC

- Arithmetic - 6 [Done]
    - ADD, SUB, MUL, DIV, {ADDS, SUBS}, {MULS, DIVS}
- Bitwise - 12 [AND, OR, XOR, SHR, SHL, MVNT, BSET, BCLR Done]
    - AND, OR, XOR, {#MVNT}, BSET, BCLR, {BSETS, BCLRS}, SHR, SHL, {SHRS, SHLS}
- Color - 2 (24) [Done]
    - {GET[RGBHSVXY], PUT[RGBHSVXY]}, PUTS[RGBHSVXY]
        - 5-opcode, 3-subop, 15-data, 9-tgt = 32
- Compare - 2 [CMP, CMPS -  Done]
    - {CMP, CMPS}, ~TST
- Branching - 5 [BEQ, BNEQ Done]
    - BEQ, BNEQ, CALL, RET
- Memory - 4 [MV, LD, LDC, LDCH Done]
    - {MV, LD, LDC, LDCH}, {#PUSH, #POP}
- IO - 0
    - {#ASEND, #ARECV}
- Other - 0 [NOP, HALT Done]
    - {#NOP, #HALT, #END, #SPEC, #DEBUG}

Total: 32?

- Operations to add?
    - INCSZ/DECSZ - skip if zero
    - BTSS/BTSC - bit test skip if set/clear
    - MODULO


Callstack size: 4?

ADD r1, r2, r3 ; r1 <- r2 + r3
ADDC r1, r2, C ; r1 <- r2 + C

Opcode: 5
Ref: 9

