/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */

#ifndef ECC_LEXICAL_H
#define ECC_LEXICAL_H
#include "ecc-def.h"

//L stands for logical or less
enum{
        Number=64,
        Function,
        System,
        Global,
        Local,
        Identifier,
        Char,
        Else,
        Enum,
        If,
        Int,
        Return,
        Sizeof,
        While,
        Assign,
        Cond,
        Lor,
        Land,
        Or,
        Xor,
        And,
        Eq,
        Ne,
        Lthan,
        Gthan,
        Leq,
        Geq,
        Shl,
        Shr,
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Inc,
        Dec,
        Bracket

};

/*struct identifier{
        int64_t token;
        // used for judge the same
        int64_t hash;
        char *name;
        // category like : global ,local etc.
        int64_t category;
        int64_t type;
        // value or address(for function etc.)
        int64_t value;
        // used for global name equals local name
        // save global
        int64_t shadow_category;
        int64_t shadow_type;
        int64_t shadow_value;

};*/

enum {
        Token,
        Hash,// used for judge the same
        Name,
        Type,
        Category,// category like : global ,local etc.
        Value,// value or address(for function etc.)
        ShadowType,
        ShadowCategory,// used for global name equals local name ,save global
        ShadowValue,
        Size
};

void get_next();
int64_t read_src(const char *filename);

#endif //ECC_LEXICAL_H
