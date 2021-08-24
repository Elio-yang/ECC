/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/22
 */

#ifndef ECC_ECC_DEF_H
#define ECC_ECC_DEF_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>



#define print_errno(e) printf("line %d with errno :%d->%s\n" ,__LINE__,e,strerror(e))
#define MEM_ERR(e)  do{printf("couldn't alloc enough space.\n");print_errno(e);}while(0)
#define IO_ERR(e)   do{printf("IO error.\n");print_errno(e);}while(0)


#define kB256           (256*1024)
#define done            (0)
#define err_memory      (-1)
#define err_io          (-2)
#define err_unk         (-3)
#define eof             '\0'
#define nullptr         NULL

static void panic(char *fmt, ...)
{
        va_list va;

        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        fputs("\n", stderr);
        va_end(va);
        exit(1);
}


#endif //ECC_ECC_DEF_H
