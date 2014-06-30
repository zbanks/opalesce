#include "stdio.h"
#include "opalesce.h"

color_t framebuffers[NUM_FRAMEBUFFERS][NUM_PIXELS];
opcode_t filters[NUM_FILTERS][FILTER_SIZE];

// 9 bits of ref
// # registers = 16
// 8 7654 3210
// 0 0000 xxxx - READONLY Special[`xxxx`]
// 0 0001 **** - READONLY Unused[`xxxx`] (Regs ___?)
// 0 0010 rrrr - Regs[`xxxx`]
// 0 0011 rrrr - Program[Regs[`rrrr`]]
// 0 01ff rrrr - Framebuffer[`ff`][Regs[`rrrr`]]
// 0 1*** **** - Unused (Lit?)
// 1 xxxx xxxx - Program[`xxxxxx`] (exact)
//
inline uint32_t opalesce_fetch_address(uint32_t addr, opcode_t* pgm, op_regs_t* regs){
    uint32_t data;
    #define _REGp(a) (regs->r[a & 0xf])
    /* Decode source address */
    if(addr & 0x100){ // Program const
        data = *(pgm + (addr & FILTER_SIZE_MASK));
    }else if(addr & 0x80){ // Unused
        data = 0;
    }else if(addr & 0x40){ // Framebuffer[y][Reg[x]]
        data = framebuffers[(addr >> 4) & NUM_FRAMEBUFFERS_MASK][_REGp(addr) & NUM_PIXELS_MASK].raw;
    }else if((addr & 0x30) == 0x30){ // Program[Reg[x]]
        data = *(pgm + (_REGp(addr) & FILTER_SIZE_MASK));
    }else if(addr & 0x20){ // Reg[x]
        data = _REGp(addr);
    }else if(addr & 0x10){ // Unused
        data = 0;
    }else{ // Special
        data = *(&(regs->pc) + (addr & 0x7)); //TODO
    }
    return data;
}

void opalesce_exec(opcode_t* program, uint32_t beat, uint32_t tick){
    op_regs_t regs;
    uint32_t src, cons, dest;
    color_t color;
    uint32_t value;
    uint32_t *ptr;
    uint32_t src_a, cons_a, dest_a;
    uint32_t *program_status;
    uint8_t write_dest;
    int i;
    #define _REG(a) (regs.r[a & 0xf])

    regs.pc = 0;
    regs.time = 0;
    regs.beat = beat;
    regs.tick = tick;
    regs.fblen = NUM_PIXELS;
    regs.sp = FILTER_SIZE - 1;
    regs.cycles = 20;
    for(i = 0; i < 16; i++){
        regs.r[i] = 0;
    }
    opcode_t* program_start = program + 1;
#define _FETCH_DEST (opalesce_fetch_address(_gD(*program), program_start, &regs))
#define _FETCH_SRC (opalesce_fetch_address(_gS(*program), program_start, &regs))
#define _FETCH_CONS (opalesce_fetch_address(_gL(*program), program_start, &regs))

    program_status = program;
    if(!(*program_status & OPL_PGM_RUN)){
        return;
    }

    while((regs.time < OPL_MAX_RUNTIME) && (regs.pc <= regs.sp)){
        write_dest = 0;
        program = program_start + regs.pc;
        src_a = _gS(*program);
        src = opalesce_fetch_address(src_a, program_start, &regs);
        //printf("[0x%08x] source: 0x%08x @0x%08x\n", *program, src, src_a);

        switch(*program & OPL_OPMASK){
            case OPL_OP_BEQ: // Branch if equal
                dest = _FETCH_DEST;
                value = _gL(*program); // Jump to location
                // Value & 0x100 is BNEQ
                if(value & 0x100 ? (src != dest) : (src == dest)){
                    value &= 0xFF;
                    if(value >= FILTER_SIZE){
                        value = FILTER_SIZE - 1;
                    }
                    regs.pc = value - 1;
                }
            break;
            case OPL_OP_CPUT: // Put color
                // TODO Put constant
                color.raw = _FETCH_DEST;
                if(*program & (1 << 23)){ // Put constant
                    value = src_a;
                }else{
                    value = src;
                }
                if(*program & OPL_C_R){
                    ASSERT_RGB(color);
                }else{
                    ASSERT_HSV(color);
                }
                switch(*program & OPL_C_MASK){
                    case OPL_C_H:
                        color.st.hsv_h = value;
                    break;
                    case OPL_C_S:
                        color.st.hsv_s = value;
                    break;
                    case OPL_C_V:
                        color.st.hsv_v = value;
                    break;
                    case OPL_C_HSV:
                        color.raw &= 0xFFFF0000;
                        color.raw |= value & 0xFFFF;
                    break;
                    case OPL_C_R:
                        color.st.rgb_r = value;
                    break;
                    case OPL_C_G:
                        color.st.rgb_g = value;
                    break;
                    case OPL_C_B:
                        color.st.rgb_b = value;
                    break;
                    case OPL_C_RGB:
                    default:
                        color.raw &= 0xFFFF;
                        color.raw |= value << 16;
                    break;
                }
                if(*program & OPL_C_R){ // RGB have 0x04 bit set
                    color.st.hsv_valid = 0;
                    color.st.rgb_valid = 1;
                }else{
                    color.st.hsv_valid = 1; //XXX &= 1?
                    color.st.rgb_valid = 0;
                }
                dest = color.raw;
                write_dest = 1;
            break;
            case OPL_OP_CGET: // Get color
                color.raw = src;
                if(*program & OPL_C_R){
                    ASSERT_RGB(color);
                }else{
                    ASSERT_HSV(color);
                }
                switch(*program & OPL_C_MASK){
                    case OPL_C_H:
                        dest = color.st.hsv_h;
                    break;
                    case OPL_C_S:
                        dest = color.st.hsv_s;
                    break;
                    case OPL_C_V:
                        dest = color.st.hsv_v;
                    break;
                    case OPL_C_HSV:
                        dest = color.raw >> 16; //TODO
                    break;
                    case OPL_C_R:
                        dest = color.st.rgb_r;
                    break;
                    case OPL_C_G:
                        dest = color.st.rgb_g;
                    break;
                    case OPL_C_B:
                        dest = color.st.rgb_b;
                    break;
                    case OPL_C_RGB:
                    default:
                        dest = color.raw & 0xFFFF; // TODO
                    break;
                }
                write_dest = 1;
            break;
            case OPL_OP_MV: // 00100 XYvvvvvvv sssssssss dddddddd
                // XY = 00: Move s -> d
                // XY = 01: Move lit vs -> d
                // XY = 10: Move lit vs -> lower half of d
                // XY = 11: Move lit vs -> upper half of d
                if(*program & (1 << 26)){ // Load half constant
                    dest = _FETCH_DEST;
                    if(*program & (1 << 25)){ // Load high
                        dest &= 0xFFFF;
                        dest |= (*program << 7) & 0xFFFF0000;
                    }else{ // Load Low
                        dest &= 0xFFFF0000;
                        dest |= (*program >> 9) & 0xFFFF;
                    }
                }else{ // Move
                    if(*program & (1 << 25)){ // Load Constant
                        dest = (*program >> 9) & 0xFFFF;
                    }else{
                        dest = src;
                    }
                }
                write_dest = 1;
            break;
            case OPL_OP_BSET:
                cons = _FETCH_CONS;
                dest |= (1 << cons);
                write_dest = 1;
            break;
            case OPL_OP_BCLR:
                cons = _FETCH_CONS;
                dest = src & ~cons;
                write_dest = 1;
            break;
            case OPL_OP_TWOI:
                switch(_gL(*program)){
                    case 0x00: // NOP
                    break;
                    case _gL(OPL_OP_MVNT): // MVNT
                        dest = ~src;
                    break;
                    case _gL(OPL_OP_CALL): // CALL
                        dest = *(program_start+regs.sp) = regs.pc;
                        regs.sp--;
                        if(regs.sp == 0){
                            goto _halt;
                        }
                        regs.pc = src;
                        //write_dest = 1
                    break;
                    case _gL(OPL_OP_RET): // RET
                        regs.sp++;
                        dest = regs.pc;
                        regs.pc = *(program_start+regs.sp);
                        if(regs.sp >= FILTER_SIZE){
                            goto _halt;
                        }
                        //write_dest = 1
                    break;
                    case _gL(OPL_OP_DEBUG):
                        fprintf(stderr, "Debug: %i 0x%x\n", src, src);
                    break;
                    case _gL(OPL_OP_HALT):
                        //regs.time = OPL_MAX_RUNTIME;
                        goto _halt;
                    break;
                    case _gL(OPL_OP_PUSH):
                        value = *program & 0xFFFF; // 16 regs
                        ptr = regs.r;
                        while(value && regs.sp){
                            regs.sp--;
                            if(value & 1){
                                *(program_start+regs.sp) = *ptr;
                            }
                            ptr++;
                            value >>= 1;
                        }
                        if(regs.sp == 0){
                            goto _halt;
                        }
                    break;
                    case _gL(OPL_OP_POP):
                        value = *program & 0xFFFF; // 16 regs
                        ptr = regs.r + 15;
                        while(value && regs.sp){
                            if(regs.sp >= FILTER_SIZE){
                                goto _halt;
                            }
                            regs.sp++;
                            ptr--;
                            value = (value << 1) & 0xFFFF; // opt with uint16_t?
                        }
                    break;
                    case _gL(OPL_OP_INCSQ):
                        dest = _FETCH_DEST;
                        dest++;
                        if(dest == src){
                            regs.pc++;
                        }
                        write_dest = 1;
                    break;
                    case _gL(OPL_OP_DECSQ):
                        dest = _FETCH_DEST;
                        dest--;
                        if(dest == src){
                            regs.pc++;
                        }
                        write_dest = 1;
                    break;
                    case _gL(OPL_OP_BTSS):
                        dest = _FETCH_DEST;
                        if(src & (1 << dest)){
                            regs.pc++;
                        }
                    break;
                    case _gL(OPL_OP_BTSC):
                        dest = _FETCH_DEST;
                        if(!(src & (1 << dest))){
                            regs.pc++;
                        }
                    break;
                    case _gL(OPL_OP_END):
                        *program_status &= ~OPL_PGM_RUN;
                    break;
                    default:
                    break;
                }
            break;
            case OPL_OP_CMP:
                cons = _gL(*program);
                if(cons & 0x100){ // CMPS
                    cons = cons & 0xFF;
                }else{
                    cons = _FETCH_CONS;
                }
                dest = cons > src;
                write_dest = 1;
            break;
            case OPL_OP_AND:
                cons = _FETCH_CONS;
                dest = cons & src;
                write_dest = 1;
            break;
            case OPL_OP_OR:
                cons = _FETCH_CONS;
                dest = cons | src;
                write_dest = 1;
            break;
            case OPL_OP_XOR:
                cons = _FETCH_CONS;
                dest = cons ^ src;
                write_dest = 1;
            break;
            case OPL_OP_SHL:
                cons = _FETCH_CONS;
                dest = src << cons;
                write_dest = 1;
            break;
            case OPL_OP_SHR:
                cons = _FETCH_CONS;
                dest = src >> cons;
                write_dest = 1;
            break;
            case OPL_OP_ADD:
                cons = _FETCH_CONS;
                dest = cons + src;
                write_dest = 1;
            break;
            case OPL_OP_SUB:
                cons = _FETCH_CONS;
                dest = src - cons;
                write_dest = 1;
            break;
            case OPL_OP_MUL:
                cons = _FETCH_CONS;
                dest = cons * src;
                write_dest = 1;
            break;
            case OPL_OP_DIV:
                cons = _FETCH_CONS;
                dest = src / cons;
                write_dest = 1;
            break;
            case OPL_OP_ADDS:
                cons = _gL(*program);
                if(cons & 0x100){ // SUBS
                    dest = src - (cons & 0xFF);
                }else{ // ADDS
                    dest = src + (cons & 0xFF);
                }
                write_dest = 1;
            break;
            case OPL_OP_MULS:
                cons = _gL(*program);
                if(cons & 0x100){ // DIVS
                    dest = src / (cons & 0xFF);
                }else{ // MULS
                    dest = src * (cons & 0xFF);
                }
                write_dest = 1;
            break;
            default:
            break;
        }

        if(write_dest){
            dest_a = _gD(*program);
            //printf("[0x%08x] dest: 0x%08x @0x%08x\n", *program, dest, dest_a);
            if(dest_a & 0x100){ // Program const
                *(program_start + (dest_a  & FILTER_SIZE_MASK)) = dest;
            }else if(dest_a & 0x80){ // Unused
            }else if(dest_a & 0x40){ // Framebuffer[y][Reg[x]]
                framebuffers[(dest_a >> 4) & NUM_FRAMEBUFFERS_MASK][_REG(dest_a) & NUM_PIXELS_MASK].raw = dest;
            }else if((dest_a & 0x30) == 0x30){ // Program[Reg[x]]
                *(program_start + (_REG(dest_a) & FILTER_SIZE_MASK)) = dest;
            }else if(dest_a & 0x20){ // Reg[x]
                _REG(dest_a) = dest;
            }else if(dest_a & 0x10){ // Unused
            }else{ // Special
            }
        }

        regs.pc++;
        if(regs.pc >= FILTER_SIZE){
            goto _halt;
        }
        regs.time++;
    }
    _halt:
    fprintf(stderr, "Run time: %d\n", regs.time);
    return;
}

void oplaesce_extract_rgb(uint16_t *pixel_data){
    int i;
    for(i = 0; i < NUM_PIXELS; i++){
        ASSERT_RGB(framebuffers[0][i]);
        *(pixel_data + i) = framebuffers[0][i].raw & 0xFFFF;
    }
}
