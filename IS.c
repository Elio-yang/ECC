/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */

#include "IS.h"

//registers:
// program counter :address of next instruction
// base pointer
// stack pointer   :top of stack,decrease when push stack
// ax is general register
extern int64_t *pc,
        *bp,
        *sp,
        ax,
        cycle;

//extern definitions
extern int64_t *text, *old_text, *stack;
extern int32_t pool_size;


void reg_init()
{
        bp = sp = (int64_t *) ((char *) stack + pool_size);
        ax = 0;
}


//interpreter part (using VM)
int64_t eval()
{

        int64_t op;
        int64_t *tmp;


        while (true) {
                op = *pc++;
                if (op == IMM) {
                        ax = *pc++;
                } else if (op == LOADC) {
                        ax = *(char *) ax;
                } else if (op == LOADI) {
                        ax = *(int64_t *) ax;
                } else if (op == STOREC) {
                        // sp store addr
                        // *sp -> an address
                        // assuming store char -> (char*)
                        // store the value in ax
                        ax = *(char *) *sp = ax;
                        sp++;
                } else if (op == STOREI) {
                        *(int64_t *) *sp = ax;
                        sp++;
                } else if (op == PUSH) {
                        *--sp = ax;
                } else if (op == JMP) {
                        // pc points to next instruction
                        // JMP <addr>
                        // pc store 'addr'
                        // ins1: JMP   <-run here
                        // ins2: addr  <-pc
                        pc = (int64_t *) *pc;
                } else if (op == JZ) {
                        pc = (ax ? pc + 1 : (int64_t *) *pc);
                } else if (op == JNZ) {
                        pc = (ax ? (int64_t *) *pc : pc + 1);
                } else if (op == CALL) {
                        // call <addr>
                        // jmp to addr
                        // save current pc & jmp addr = call

                        //push current pc to stack (save environment)
                        *--sp = (int64_t) (pc + 1);
                        // set pc the 'addr'
                        pc = (int64_t *) *pc;

                } else if (op == RET) {
                        //get ret addr from stack
                        pc = (int64_t *) *sp++;
                } else if (op == ENT) {
                        // new new frame
                        // ENT <size>
                        // alloc some space in stack
                        // to store local variables
                        // in intel:
                        // push bp
                        // mov  bp, sp
                        // sub imm, sp

                        // save old bp into stack
                        *--sp = (int64_t) bp;
                        // set bp sp
                        bp = sp;
                        // get size to save
                        int64_t size = *pc;
                        // pc -> next instr
                        pc++;
                        // alloc stack
                        sp -= size;
                } else if (op == ADJ) {
                        // clear stack data when leave
                        // ADJ <size>
                        // in intel :
                        // add sp, size
                        int64_t size = *pc;
                        pc++;
                        sp += size;
                } else if (op == LEV) {
                        // restore old frame

                        /*
                                 +--------+  <high addr>
                                 | ...    |
                                 +--------+
                                 | save pc|
                                 +--------+
                             bp->| save bp|<- old sp = bp
                                 +--------+   ^
                                 |  ...   |   | +size
                                 |        |   |
                                 | (size) |<- sp
                                 +--------+
                                 |        |
                                 +--------+  <low addr>
                         */
                        sp = bp;
                        bp = (int64_t *) *sp;
                        sp++;
                        pc = (int64_t *) *sp;
                        sp++;
                } else if (op == LEA) {
                        // get args from callee in caller
                        // __ecall: ECC calling convention
                        //  args push in order (left -> right)
                        // callee do the clean job
                        /*
                         T func1(...){
                                T local1;
                                T local2;
                                func2(arg1,arg2);
                                ...
                         }
                         +-----------+
                         |  ...      |  <high address>
                         +-----------+
                         |  arg1     | <- bp+3
                         +-----------+
                         |  arg2     | <- bp+2
                         +-----------+
                         | ret addr  | <- bp+1
                         +-----------+
                         | old bp    | <- bp
                         +-----------+
                         | local 1   | <- bp-1
                         +-----------+
                         | local 2   | <- bp-2
                         +-----------+
                         |  ...      |  <low address>
                         +-----------+
                         */

                        // LEA <offset>
                        // in intel :
                        // add bp,offset
                        // ax store args' addr
                        ax = (int64_t) (bp + *pc);
                        pc++;
                }
                        // for arithmetic all instructions are
                        // binary operand
                        // the first arg in stack
                        // the second arg in ax
                        // result will be save into ax
                else if (op == OR) {
                        ax = *sp | ax;
                        sp++;
                } else if (op == XOR) {
                        ax = *sp ^ ax;
                        sp++;
                } else if (op == AND) {
                        ax = *sp & ax;
                        sp++;
                } else if (op == EQ) {
                        ax = *sp == ax;
                        sp++;
                } else if (op == NE) {
                        ax = *sp != ax;
                        sp++;
                } else if (op == LT) {
                        ax = *sp < ax;
                        sp++;
                } else if (op == LE) {
                        ax = *sp <= ax;
                        sp++;
                } else if (op == GT) {
                        ax = *sp > ax;
                        sp++;
                } else if (op == GE) {
                        ax = *sp >= ax;
                        sp++;
                } else if (op == SHL) {
                        ax = *sp << ax;
                        sp++;
                } else if (op == SHR) {
                        ax = *sp >> ax;
                        sp++;
                } else if (op == ADD) {
                        ax = *sp + ax;
                        sp++;
                } else if (op == SUB) {
                        ax = *sp - ax;
                        sp++;
                } else if (op == MUL) {
                        ax = *sp * ax;
                        sp++;
                } else if (op == DIV) {
                        ax = *sp / ax;
                        sp++;
                } else if (op == MOD) {
                        ax = *sp % ax;
                        sp++;
                }
                        // C api part
                        // use some useful c function
                else if (op == EXIT) {
                        printf("exit(%ld)\n", *sp);
                        return *sp;
                } else if (op == OPEN) {
                        ax = open((char *) sp[1], sp[0]);
                } else if (op == CLOS) {
                        ax = close(*sp);
                } else if (op == READ) {
                        ax = read(sp[2], (char *) sp[1], *sp);
                } else if (op == PRTF) {
                        tmp = sp + pc[1];
                        ax = printf((char *) tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
                } else if (op == MALC) {
                        ax = (int64_t) malloc(*sp);
                } else if (op == MSET) {
                        ax = (int64_t) memset((char *) sp[2], sp[1], *sp);
                } else if (op == MCMP) {
                        ax = memcmp((char *) sp[2], (char *) sp[1], *sp);
                } else {
                        printf("unknown instruction :%ld\n", op);
                        return err_unk;
                }


        }
}