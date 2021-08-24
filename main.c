/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */

// implement a simple c interpreter
// based on the VM and IS designed
// target code will be based on the IS
// basic workflow :
// 1.build VM & IS
// 2.build lexical analysis
// 3.build grammar analysis



#include "ecc-def.h"
#include "vm.h"
#include "IS.h"
#include "lexical.h"
#include "grammar.h"



/* global declarations */
// current token
int64_t token;
// as the name pointer to source file
char * p_src,*old_p_src;
// default size
uint64_t pool_size = kB256;
// IS related
int64_t *text, *old_text, *stack;
char* data;
int64_t *pc,
        *bp,
        *sp,
        ax;
// source code lines
int64_t lines;
// used for numbers value
int64_t token_val;
// used for symbol table
int64_t * symbols;
// currently parsed
int64_t * parse_id;
//track of main
int64_t *p_main;

int32_t main(int argc , char *argv[])
{
        read_src(*++argv);
        int ret;
        if( (ret=vm_alloc_mem())!=done){
                return ret;
        }
        reg_init();
        kw_parse();
        program();
        return (int32_t)eval();
}