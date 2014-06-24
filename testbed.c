#include "color.h"
#include "opalesce.h"

int main(){
    int i = 0;
    /* Test Load/Move instructions
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_LDL   | _S(0x1234)    | _D(_AREG(0));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_LDH   | _S(0xFFFF)    | _D(_AREG(0));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_LD    | _S(0xF00F)    | _D(_AREG(0));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_MV    | _S(_AREG(0))  | _D(_AREG(1));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(1));
    filters[0][i++] = OPL_OP_LD    | _S(0xFF)      | _D(_AREG(0));
    */

    filters[0][i++] = OPL_OP_LD    | _S(0xFF)      | _D(_AREG(0));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_R;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_G;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_B;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_S;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));

    filters[0][i++] = OPL_OP_HALT;
    opalesce_exec(filters);
}
