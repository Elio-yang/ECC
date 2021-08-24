/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/22
 */

/*
 simulated memory space
 +-------------------+  0xffffffff
 |      stack |      |
 |       ...  v      |
 |                   |
 |                   |
 |                   |
 |       ...  ^      |
 |      heap  |      |
 +-------------------+
 |      bss          |
 +-------------------+
 |      data         |
 +-------------------+
 |      text         |
 +-------------------+  0x00000000
 text : store codes
 data : initialized data
 bss  : uninitialized data
 stack: calling frame
 heap : dynamic allocation
 */
#include "ecc-def.h"
#include "vm.h"



extern int64_t *text,*old_text,*stack;
extern char *data;
extern int64_t *symbols;
extern int64_t pool_size;



int32_t vm_alloc_mem()
{
        if((text=old_text= malloc(pool_size))==nullptr){
                MEM_ERR(errno);
                goto err1;
        }
        if((data= malloc(pool_size))==nullptr){
                MEM_ERR(errno);
                goto err2;
        }
        if((stack= malloc(pool_size))==nullptr){
                MEM_ERR(errno);
                goto err3;
        }
        if((symbols= malloc(pool_size))==nullptr){
                MEM_ERR(errno);
                goto err4;
        }
        goto correct;

        //memory error handle
err1:
        return err_memory;
err2:
        free(text);
        goto err1;
err3:
        free(text);
        free(data);
        goto err1;
err4:
        free(text);
        free(data);
        free(stack);
        goto err1;

        correct:
        //initial memory space
        memset(text,0,pool_size);
        memset(data,0,pool_size);
        memset(stack,0,pool_size);
        memset(symbols,0,pool_size);
        return done;
}









