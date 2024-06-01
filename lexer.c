#include "include/lexer.h"
#include <stdio.h>
#include <stdbool.h>

int lineNumber = 1;
int charNumber = 1;
char *currentCharacter = NULL;
FILE *source = NULL;
char buffer[BUFFER_SIZE] = {};
bool endOfFile = 0;

void reloadBuffer()
{
    fgets(buffer, BUFFER_SIZE, source); 
    currentCharacter = buffer;  
}

bool isDigit(char c)
{
    return (  (c >= 48 && c <= 57) );
}

bool isCharacter(char c)
{
    return (  (c >= 65 && c <= 90) || (c >= 97 && c <= 122) );
}

void getLexeme(token_t* token)
{
    int i = 0;
    while(isCharacter(*currentCharacter))
    {
        token->attribute.lexeme[i++] = *currentCharacter;
        currentCharacter++;
        charNumber++;
        if(*currentCharacter == '\n')
        {
            reloadBuffer();
            lineNumber++;
            charNumber = 1;
        }
    }
    token->tag = ID;
}

void getValue(token_t* token)
{
    while(isDigit(*currentCharacter))
    {
        token->attribute.numval = (token->attribute.numval * 10) + (*currentCharacter - 48);
        currentCharacter++;
        charNumber++;
        if(*currentCharacter == '\n')
        {
            reloadBuffer();
            lineNumber++;
            charNumber = 1;
        }
    }
    token->tag = NUM;
}

void getOp(token_t* token)
{
    switch(*currentCharacter)
    {
        case ';':
            token->attribute.lexeme[0] = ';';
            token->tag = SEMICOLON;
            currentCharacter++;
            charNumber++;
        break;
        
        case '>':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '>';token->attribute.lexeme[1] = '=';
                token->tag = GREATEREQ;
                currentCharacter += 2;
                charNumber += 2;

            }
            else
            {
                token->attribute.lexeme[0] = '>';
                token->tag = GREATER;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '<':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '<';token->attribute.lexeme[1] = '=';
                token->tag = LESSEREQ;
                currentCharacter += 2;
                charNumber += 2;
            }
            else
            {
                token->attribute.lexeme[0] = '<';
                token->tag = LESSER;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '+':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '+';token->attribute.lexeme[1] = '=';
                token->tag = ADDASSIGN;
                currentCharacter += 2;
                charNumber += 2;
            }
            else if(  *(currentCharacter + 1) == '+' )
            {
                token->attribute.lexeme[0] = '+';token->attribute.lexeme[1] = '+';
                token->tag = INCREMENT;
                currentCharacter += 2;
                charNumber += 2;
            }
            else
            {
                token->attribute.lexeme[0] = '+';
                token->tag = PLUS;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '-':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '-';token->attribute.lexeme[1] = '=';
                token->tag = SUBASSIGN;
                currentCharacter += 2;
                charNumber += 2;
            }
            else if(  *(currentCharacter + 1) == '-' )
            {
                token->attribute.lexeme[0] = '-';token->attribute.lexeme[1] = '-';
                token->tag = DECREMENT;
                currentCharacter += 2;
                charNumber += 2;
            }
            else
            {
                token->attribute.lexeme[0] = '-';
                token->tag = MINUS;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '=':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '=';token->attribute.lexeme[1] = '=';
                token->tag = EQUAL;
                currentCharacter += 2;
                charNumber += 2;
            }
            else
            {
                token->attribute.lexeme[0] = '=';
                token->tag = ASSIGN;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '!':
            if( *(currentCharacter + 1) == '=' )
            {
                token->attribute.lexeme[0] = '!';token->attribute.lexeme[1] = '=';
                token->tag = DIFFERENT;
                currentCharacter += 2;
                charNumber += 2;
            }
            else
            {
                token->attribute.lexeme[0] = '!';
                token->tag = LGCL_NOT;
                currentCharacter++;
                charNumber++;
            }
        break;

        case '(':
            token->tag = OP_PAR;
            token->attribute.lexeme[0] = '(';
            currentCharacter++;
            charNumber++;
        break;

        case ')':
            token->tag = CL_PAR;
            token->attribute.lexeme[0] = ')';
            currentCharacter++;
            charNumber++;
        break;


        case '[':
            token->tag = OP_SQREBRACK;
            token->attribute.lexeme[0] = '[';
            currentCharacter++;
            charNumber++;
        break;

        case ']':
            token->tag = CL_SQREBRACK;
            token->attribute.lexeme[0] = ']';
            currentCharacter++;
            charNumber++;
        break;

        case '{':
            token->tag = OP_CRLYBRACK;
            token->attribute.lexeme[0] = '{';
            currentCharacter++;
            charNumber++;
        break;

        case '}':
            token->tag = CL_CRLYBRACK;
            token->attribute.lexeme[0] = '}';
            currentCharacter++;
            charNumber++;
        break;

        case ',':
            token->tag = COMMA;
            token->attribute.lexeme[0] = ',';
            currentCharacter++;
            charNumber++;
        break;

        default:
            printf("Operador '%c' no identificado\n", *currentCharacter);
            currentCharacter++;
            charNumber++;
            return;
        break;
    }
    printf("Operador '%s' conseguido en la linea: %d columna: %d\n", token->attribute.lexeme, lineNumber, charNumber);
}

void skipLineComment()
{
    while( *currentCharacter != '\n' )
    {
        currentCharacter++;
    }
    reloadBuffer();
}

void skipBlockComment()
{
    while(1)
    {
        if( *currentCharacter == '*' && *(currentCharacter + 1) == '/' )
        {
            currentCharacter += 2;
            charNumber += 2;
            return;
        }
        else if( *currentCharacter == '\n' )
        {
            reloadBuffer();
            lineNumber++;
            charNumber = 1;
        }
        else
        {
            currentCharacter++;
            charNumber++;
        }
    }
}

void skipWhitespace()
{
    while(1)
    {
        if( *currentCharacter == '\n' || *currentCharacter == '\0') 
        {
            lineNumber++;
            charNumber = 1;
            reloadBuffer();
        }
        else if( *currentCharacter == '/' )
        {
            switch( *(currentCharacter + 1) )
            {
                case '/':
                    skipLineComment();
                    lineNumber++;
                    charNumber = 1;
                break;

                case '*':
                    skipBlockComment();
                break;

                default:
                    return;
                break;
            }
        }
        else if( *currentCharacter != '\t' && *currentCharacter != ' ' )
        {
            return;
        }
        else
        {
            charNumber++;
            currentCharacter++;
        }
    }
}

void initLexer(FILE* fptr)
{
    source = fptr;
    reloadBuffer();
}

token_t scan()
{
    token_t currentToken = {0, 0, 0};
    if(endOfFile)
    {
        currentToken.tag = ENDOFFILE;
        return currentToken;
    }
    skipWhitespace();
    if(isCharacter(*currentCharacter))
    {
        getLexeme(&currentToken);
    }
    else if(isDigit(*currentCharacter))
    {
        getValue(&currentToken);
    }
    else
    {
        getOp(&currentToken);
    }
    if(*currentCharacter == '\0') endOfFile = 1;
    return currentToken;
}