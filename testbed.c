#include "color.h"
#include "opalesce.h"
#include <stdio.h>

void loop();
void print_color(color_t);
void print_strip_html();

int main(){
    int i = 0, j=0;
    /* 
    // Test Load/Move instructions
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

    /*
    // Test Color instructions 
    filters[0][i++] = OPL_OP_LD    | _S(0xFF)      | _D(_AREG(0));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_R;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_G;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_B;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    filters[0][i++] = OPL_OP_CPUT  | _S(_AREG(0))  | _D(_AREG(2)) | OPL_C_S;
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(2));
    */

    /*
    // Test BEQ/BNEQ
    filters[0][i++] = OPL_OP_LD    | _S(0x0)      | _D(_AREG(0));
    filters[0][i++] = OPL_OP_LD    | _S(0x4)      | _D(_AREG(1));
    filters[0][i++] = OPL_OP_LD    | _S(0x10)     | _D(_AREG(2));
    filters[0][i++] = OPL_OP_LD    | _S(0xff)     | _D(_AREG(3));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(3));
    filters[0][i++] = OPL_OP_ADD   | _S(_AREG(1)) | _D(_AREG(0)) | _L(_AREG(0));
    filters[0][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[0][i++] = OPL_OP_BNEQ  | _S(_AREG(2)) | _D(_AREG(0)) | _L(5);
    filters[0][i++] = OPL_OP_BEQ   | _S(_AREG(2)) | _D(_AREG(0)) | _L(6);
    */

    // Rainbow swipe
    // Pull counter from special register or store in program memory.
    //filters[0][i++] = OPL_OP_MV    | _S(_APROG(0x10)) | _D(_AREG(0)); // Progmem
    filters[j][i++] = OPL_PGM_RUN;
    filters[j][i++] = OPL_OP_MV    | _S(_ASPEC(OPL_S_BEAT)) | _D(_AREG(0)); // Beat  register
    filters[j][i++] = OPL_OP_LD    | _S(0x01)         | _D(_AREG(1));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_V |  _S(0x0)     | _D(_AFREG(0,0));
    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(1))     | _D(_AREG(0))   | _L(_AREG(0));
    //filters[j][i++] = OPL_OP_MV    | _D(_APROG(0x10)) | _S(_AREG(0)); // Progmem
    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(0))     | _D(_AREG(0))   | _L(_AREG(0));
    filters[j][i++] = OPL_OP_CPUT  | OPL_C_H |  _S(_AREG(0))    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_S |  _S(0xff)    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_V |  _S(0xff)    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(1))     | _D(_AREG(0))   | _L(_AREG(0));
    filters[j][i++] = OPL_OP_CPUT  | OPL_C_H |  _S(_AREG(0))    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_S |  _S(0xff)    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_V |  _S(0xff)    | _D(_AFREG(1,0));
    filters[j][i++] = OPL_OP_HALT;

    // Chaser
    i = 0;
    j++;
    filters[j][i++] = OPL_PGM_RUN;
    filters[j][i++] = OPL_OP_MV    | _S(_ASPEC(OPL_S_BEAT)) | _D(_AREG(0)); // Beat  register
    filters[j][i++] = OPL_OP_MV    | _S(_ASPEC(OPL_S_TICK)) | _D(_AREG(1)); // Beat  register
    filters[j][i++] = OPL_OP_MULS  | _S(_AREG(0)) | _D(_AREG(0)) | _L(4);
    filters[j][i++] = OPL_OP_DIVS  | _S(_AREG(1)) | _D(_AREG(1)) | _L(4);
    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(1)) | _D(_AREG(3)) | _L(_AREG(0));
    filters[j][i++] = OPL_OP_LD    | _S(5)                  | _D(_AREG(1));
    filters[j][i++] = OPL_OP_LD    | _S(1)                  | _D(_AREG(4));

    /*
    filters[j][i++] = OPL_OP_CMP   | _L(_ASPEC(OPL_S_FBLEN))| _D(_AREG(4)) | _S(_AREG(3));
    filters[j][i++] = OPL_OP_BNEQ  | _S(_ZERO)              | _D(_AREG(4)) | _L(9);
    filters[j][i++] = OPL_OP_SUBS  | _L(1)                  | _D(_AREG(4)) | _S(_ZERO);
    */

    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(2))           | _D(_AREG(0))   | _L(_AREG(3));
    filters[j][i++] = OPL_OP_DEBUG | _S(_AREG(0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_R |  _S(0x00)    | _D(_AFREG(3,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_G |  _S(0x1f)    | _D(_AFREG(3,0));
    filters[j][i++] = OPL_OP_CPUTS | OPL_C_B |  _S(0x00)    | _D(_AFREG(3,0));
    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(2))           | _D(_AREG(2))   | _L(_AREG(4));
    filters[j][i++] = OPL_OP_BNEQ  | _S(_AREG(1))           | _D(_AREG(2))   | _L(6);

    filters[j][i++] = OPL_OP_HALT;
    
    // Dim green & blue
    i = 0;
    j++;
    filters[j][i++] = OPL_PGM_RUN;
    filters[j][i++] = OPL_OP_LD    | _S(0x01)         | _D(_AREG(1));
    filters[j][i++] = OPL_OP_LD    | _S(NUM_PIXELS)         | _D(_AREG(3));


    filters[j][i++] = OPL_OP_MV    | _S(_AFREG(1,0))         | _D(_AREG(4));
    filters[j][i++] = OPL_OP_MV    | _S(_AFREG(3,0))         | _D(_AREG(5));

    filters[j][i++] = OPL_OP_CGET  | OPL_C_G |  _S(_AREG(4))    | _D(_AREG(2));
    filters[j][i++] = OPL_OP_CGET  | OPL_C_G |  _S(_AREG(5))    | _D(_AREG(6));
    filters[j][i++] = OPL_OP_DIVS  | _L(3) |  _S(_AREG(2))  | _D(_AREG(2));
    filters[j][i++] = OPL_OP_DIVS  | _L(2) |  _S(_AREG(6))  | _D(_AREG(6));
    filters[j][i++] = OPL_OP_ADD   |  _S(_AREG(6))  | _D(_AREG(2)) | _L(_AREG(2));
    //filters[j][i++] = OPL_OP_BEQ   | _S(_AREG(2))     | _D(_AREG(10))   | _L(5);
    //filters[j][i++] = OPL_OP_SUBS  | _L(1) |  _S(_AREG(2))  | _D(_AREG(2));
    filters[j][i++] = OPL_OP_CPUT  | OPL_C_G |  _D(_AREG(4))    | _S(_AREG(2));

    filters[j][i++] = OPL_OP_CGET  | OPL_C_B |  _S(_AREG(4))    | _D(_AREG(2));
    filters[j][i++] = OPL_OP_DIVS  | _L(2) |  _S(_AREG(2))  | _D(_AREG(2));
    filters[j][i++] = OPL_OP_CPUT  | OPL_C_B |  _D(_AREG(4))    | _S(_AREG(2));

    filters[j][i++] = OPL_OP_MV  |  _D(_AFREG(0,0))    | _S(_AREG(4));

    filters[j][i++] = OPL_OP_ADD   | _S(_AREG(1))     | _D(_AREG(0))   | _L(_AREG(0));
    filters[j][i++] = OPL_OP_BNEQ   | _S(_ASPEC(OPL_S_FBLEN))     | _D(_AREG(0))   | _L(1);
    filters[j][i++] = OPL_OP_HALT;


    printf("<style>span{ width: 5; height: 5; margin: 0px; padding: 0px; display: inline-block; } div{font-size: 0; height: 5px; margin-bottom: 0px;}</style>\n");
    //opalesce_exec(filters);
    for(i = 0; i < 100; i++){
        loop(i);
    }
}

void loop(int i){
    int j;
    opalesce_exec(filters[0], i, 0);
    opalesce_exec(filters[1], i, 0);
    opalesce_exec(filters[2], i, 0);
    //opalesce_extract_pixels(pixels);
    print_strip_html();
    for(j = 0; j < NUM_PIXELS; j++){
        framebuffers[3][j].raw = 0;
    }
}

void print_color(color_t color){
    printf("#%02x%02x%02x ", (color.st.rgb_r << 3), (color.st.rgb_g << 3), (color.st.rgb_b << 3));
}

void print_strip_html(){
    int i;
    printf("<div>\n");
    for(i = 0; i < NUM_PIXELS; i++){
        printf("\t<span style='background-color:");
        ASSERT_RGB(framebuffers[0][i]);
        print_color(framebuffers[0][i]);
        printf("'>%d</span>\n", i);
    }
    printf("</div>\n");
}
