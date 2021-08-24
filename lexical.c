/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/8/23
 */

#include "lexical.h"
/*
                    +-------+                      +--------+
--> source code --> | lexer | --> token stream --> | parser | --> assembly
                    +-------+                      +--------+
*/
//current token
extern int64_t token;
//as the name pointer to source file
extern char *p_src, *old_p_src;
extern int64_t pool_size;
extern int64_t lines;
// used for numbers value
extern int64_t token_val;
// used for symbol table
extern int64_t *symbols;
// currently parsed
extern int64_t *parse_id;
// data section
extern char *data;

//get next token
void get_next()
{
        char *last_pos;
        int64_t hash;
        while ((token = (int64_t) *p_src)) {
                ++p_src;
                if (token == '\n') {
                        ++lines;
                } else if (token == '#') {
                        // macros not supported now
                        // just skip
                        while (*p_src != 0 && *p_src != '\n') {
                                p_src++;
                        }
                } else if ((token >= 'a' && token <= 'z') ||
                           (token >= 'A' && token <= 'Z') ||
                           (token == '_')) {

                        /*
                                 p_src
                                  |
                                  v
                          [...][a][b][1][2]
                                ^
                                |
                             last_pos & token
                         */
                        last_pos = (char *) (p_src - 1);
                        hash = (int64_t) token;
                        while ((*p_src >= 'a' && *p_src <= 'z') ||
                               (*p_src >= 'A' && *p_src <= 'Z') ||
                               (*p_src >= '0' && *p_src <= '9') ||
                               *p_src == '_') {
                                hash = hash * 131 + *p_src;
                                p_src++;

                                parse_id = symbols;
                                while (parse_id[Token]) {
                                        // got the same name
                                        // hash used for the quick first term of comparison
                                        if (parse_id[Hash] == hash &&
                                            !memcmp((char *) parse_id[Name], last_pos, p_src - last_pos)
                                                ) {
                                                token = parse_id[Token];
                                                return;
                                        }
                                        //to next identifier
                                        parse_id = parse_id + Size;
                                }
                                //a new identifier
                                parse_id[Name] = (int64_t) last_pos;
                                parse_id[Hash] = hash;
                                parse_id[Token] = Identifier;
                                token = Identifier;
                                return;
                        }
                } else if (token >= '0' && token <= '9') {
                        //numbers dec(15) hex(0xf 0xF) oct(017)
                        token_val = token - '0';
                        if (token_val > 0) {
                                //[1-9][0-9]*
                                while (*p_src >= '0' && *p_src <= '9') {
                                        token_val = token_val * 10 + (*p_src++ - '0');
                                }
                        } else {
                                //0[1-9]* ->hex
                                if (*p_src == 'x' || *p_src == 'X') {
                                        //0x[0-9,a-z,A-Z]*
                                        token = (int64_t) *(++p_src);
                                        while ((token >= '0' && token <= '9') ||
                                               (token >= 'a' && token <= 'z') ||
                                               (token >= 'Z' && token <= 'Z')) {
                                                /*
                                                 * ascii code of 'a' is 61H ,'A' is 41H
                                                 * it's a trick here
                                                 * that the value stood by this char
                                                 * is 9(dec) + the lower 4 bits of its
                                                 * ascii code.
                                                 * -> ch&0xf + 9
                                                 * */
                                                token_val = (token_val << 4) +
                                                            (token & 0x000000000000000f) +
                                                            (token >= 'A' ? 9 : 0);
                                                token = (int64_t) (*++p_src);
                                        }
                                } else {
                                        //0[0-7]*
                                        while (*p_src >= '0' && *p_src <= '7') {
                                                token_val = (token_val << 3) + (*p_src++ - '0');
                                        }
                                }
                                token = Number;
                                return;
                        }

                } else if (token == '"' || token == '\'') {
                        // string part : "string" , 'c'
                        // for escape char : only \n is supported
                        // as the VM assume, strings should be stored
                        // at data section
                        /*
                         the end \0 is for the whole source file
                                  p_src
                                   |
                                   v
                          [...]["][b][1][2][\0]
                                ^
                                |
                              token
                        +----------------------+
                                  p_src
                                   |
                                   v
                          [...]["][b][1][2]["]
                                ^
                                |
                              token
                         */
                        //points to data section
                        last_pos = data;
                        while (*p_src!='\0' && *p_src!= token){
                                token_val=(int64_t)(*p_src++);
                                if(token_val=='\\'){
                                        //escape char
                                        token_val=(int64_t)(*p_src++);
                                        if(token_val=='n'){
                                                //only '\n' are supported
                                                token_val='\n';
                                        }
                                }
                                if (token=='"'){
                                        //store the string
                                     *data++=(char)token_val;
                                }
                        }
                        p_src++;
                        if(token=='"'){
                                // just store the addr
                                token_val=(int64_t)last_pos;
                        }else{
                                // single chat
                                // set as NUM
                                token=Number;
                        }
                        return;
                } else if (token=='/'){
                        // for comment only "//xxxxx" is allowed
                        // LL(1) idea
                        if(*p_src=='/'){
                                while(*p_src!=0 && *p_src!='\n'){
                                        ++p_src;
                                }
                        }else{
                                // yy/xx devide
                                token=Div;
                                return;
                        }
                } else if()



        }


}

int64_t read_src(const char *filename)
{
        int fd;
        if ((fd = open(filename, O_RDWR | O_CREAT)) == -1) {
                IO_ERR(errno);
                return err_io;
        }
        //alloc space to store file
        if ((p_src = old_p_src = (char *) malloc(sizeof(char) * pool_size)) == NULL) {
                MEM_ERR(errno);
                return err_memory;
        }
        ssize_t i;
        if ((i = read(fd, p_src, pool_size - 1)) <= 0) {
                IO_ERR(errno);
                return err_io;
        }
        p_src[i] = eof;
        close(fd);
}