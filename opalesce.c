#include "stdio.h"
#include "opalesce.h"

color_t framebuffers[NUM_FRAMEBUFFERS][NUM_PIXELS];
opcode_t filters[NUM_FILTERS][FILTER_SIZE];

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

void opalesce_exec(opcode_t* program){
    op_regs_t regs;
    #define _REG(a) (regs.r[a & 0xf])
    uint32_t src, cons, dest;
    uint32_t src_a, cons_a, dest_a;
    uint8_t write_dest;

    regs.pc = 0;
    regs.time = 0;
    opcode_t* program_start = program;

    while(regs.time < OPL_MAX_RUNTIME){
        write_dest = 0;
        /* Decode source address */
        src_a = _gS(*program);
        if(src_a & 0x100){ // Program const
            src = *(program_start + (src_a & FILTER_SIZE_MASK));
        }else if(src_a & 0x80){ // Unused
            src = 0;
        }else if(src_a & 0x40){ // Framebuffer[y][Reg[x]]
            src = framebuffers[(src_a >> 4) & NUM_FRAMEBUFFERS_MASK][_REG(src_a) & NUM_PIXELS_MASK].raw;
        }else if((src_a & 0x30) == 0x30){ // Program[Reg[x]]
            src = *(program_start + (_REG(src_a) & FILTER_SIZE_MASK));
        }else if(src_a & 0x20){ // Reg[x]
            src = _REG(src_a);
        }else if(src_a & 0x10){ // Unused
            src = 0;
        }else{ // Special
            src = regs.pc; //TODO
        }
        printf("[0x%08x] source: 0x%08x 0x%08x\n", *program, src, src_a);

        switch(*program & OPL_OPMASK){
            case OPL_OP_DEBUG:
                printf("Debug: %i 0x%x\n", src, src);
            break;
            case OPL_OP_HALT:
                regs.time = OPL_MAX_RUNTIME;
            //goto _halt;
            break;
            case OPL_OP_NOP:
            default:
            break;
        }

        if(write_dest){
            dest_a = _gD(*program);
            if(dest_a & 0x100){ // Program const
                *(program_start + (src_a & FILTER_SIZE_MASK)) = dest;
            }else if(dest_a & 0x80){ // Unused
            }else if(dest_a & 0x40){ // Framebuffer[y][Reg[x]]
                framebuffers[(src_a >> 4) & NUM_FRAMEBUFFERS_MASK][_REG(src_a) & NUM_PIXELS_MASK].raw = dest;
            }else if((dest_a & 0x30) == 0x30){ // Program[Reg[x]]
                *(program_start + (_REG(src_a) & FILTER_SIZE_MASK)) = dest;
            }else if(dest_a & 0x20){ // Reg[x]
                _REG(dest_a) = dest;
            }else if(dest_a & 0x10){ // Unused
            }else{ // Special
            }
        }

        program++;
        regs.pc++;
        regs.time++;
    }
    //_halt:
}
