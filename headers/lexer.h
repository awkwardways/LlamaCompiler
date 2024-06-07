#ifndef LEXER_H
#define LEXER_H

#define BUFFER_SIZE 4096
#define MAX_LEXEME_LENGTH 256

#include <stdio.h>
typedef enum {
    NUM = 256, ID, SEMICOLON, GREATER, LESSER, GREATEREQ, LESSEREQ, INCREMENT,
    DECREMENT, PLUS, MINUS, ASSIGN, EQUAL, DIFFERENT, ADDASSIGN, SUBASSIGN, LGCL_NOT,
    OP_PAR, CL_PAR, OP_CRLYBRACK, CL_CRLYBRACK, OP_SQREBRACK, CL_SQREBRACK, ENDOFFILE,
    COMMA, IF, WHILE, FOR, RETURN, TRUE, FALSE, INT, VOID, BOOL, ELSE
    } tokenTag;

typedef struct
{
    tokenTag tag;
    union 
    {
        char lexeme[MAX_LEXEME_LENGTH];
        int numval;
    }attribute;
} token_t;

void initLexer(FILE *fptr);
token_t scan();
#endif
