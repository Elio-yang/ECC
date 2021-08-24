/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */

#include "grammar.h"
#include "lexical.h"
extern int8_t token;
//grammar part
void program()
{
        get_next();
        while(token>0){
                printf("current token is %c \n",token);
                get_next();
        }
}