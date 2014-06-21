#include "opalesce.h"

void opalesce_exec(opcode_t* program){
    static op_regs_t regs;
    regs.pc = 0;
    opcode_t* program_start = program;

    while(regs.time < OPL_MAX_RUNTIME){
        switch(program->op){
            case OPL_OP_HALT:
            goto _halt;
            case OPL_OP_NOP:
            default:
        }
        program++;
        regs.pc++;
        regs.time++;
    }
    _halt:
}
