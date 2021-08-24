/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */


#ifndef ECC_IS_H
#define ECC_IS_H

//instruction set definitions

#include "ecc-def.h"


//whole supported instructions
/*
 IMM <num>      : move <num> into ax
 LOADC          : load a 'char'    to ax(ax store addr so *ax is ok)
 LOADI          : load a 'int64_t' to ax
 STOREC/I       : store the data in ax to addr ,addr is stored in stack(by sp)
 PUSH           : push ax into stack
 JMP <addr>     : unconditional jump
 JZ/JNZ         : ax=0 is condition
 ...
 //TODO complete description
 */
enum {
        LEA,
        IMM,
        JMP,
        CALL,
        JZ,
        JNZ,
        ENT,
        ADJ,
        LEV,
        RET,
        LOADI,
        LOADC,
        STOREI,
        STOREC,
        PUSH,
        OR,
        XOR,
        AND,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        SHL,
        SHR,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        OPEN,
        READ,
        CLOS,
        PRTF,
        MALC,
        MSET,
        MCMP,
        EXIT
};


int64_t eval();
void reg_init();





#endif //ECC_IS_H
