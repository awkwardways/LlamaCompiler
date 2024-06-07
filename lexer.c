#include "include/lexer.h"
#include <stdio.h>
#include <stdbool.h>

int lineNumber = 1;
int charNumber = 1;
char *currentCharacter = NULL;
FILE *source = NULL;
char buffer[BUFFER_SIZE] = {};
bool endOfFile = 0;

int isWhitespace(char c)
{
    return (  (c == ' ') || (c == '\n') || (c == '\t') || (c == '\0') );
}
//Sustituto temporal para el hashmap
/*
    palabras reservadas:
    -int, if, while, for, return, TRUE, FALSE, void, bool, else
*/

tokenTag lookupAutomaton(const char* lexeme)
{
    int state = *lexeme;
    tokenTag tag = ID;
    switch( state )
    {
        case 'i':   //int & if
            state = 300; lexeme++;
        
        //INT & IF
        case 300:
            if( *lexeme == 'n') { state = 350; lexeme++; }  
            else if( *lexeme == 'f' ) { lexeme++; state = 5000; tag = IF; }
            else state = 5000;
                        printf("state: %d\n", state);

        case 350:
            if( *lexeme == 't') { lexeme++; state = 5000; tag = INT; }
            else return ID;
        
        case 'w':   //while
            state = 400; lexeme++;
        
        //WHILE
        case 400:
            if( *lexeme == 'h' ) { state = 401; lexeme++; }
            else state = 5000;

        case 401:
            if( *lexeme == 'i' ) { state = 402; lexeme++; }
            else state = 5000;

        case 402:
            if( *lexeme == 'l' ) { state = 403; lexeme++; }
            else state = 5000;

        case 403:
            if( *lexeme == 'e' ) { lexeme++; tag = WHILE; }
            state = 5000; 

        case 'f':   //for
            state = 500; lexeme++;
        
        //FOR
        case 500:
            if( *lexeme == 'o' ) { state = 501; lexeme++; }
            else state = 5000;

        case 501:
            if( *lexeme == 'r' ) { lexeme++; tag = FOR; } state = 5000;

        case 'r':   //return
            state = 600; lexeme++;
        
        case 600:
            if( *lexeme == 'e' ) { state = 601; lexeme++; }
            else state = 5000;

        case 601:
            if( *lexeme == 't' ) { state = 602; lexeme++; }
            else state = 5000;

        case 602:
            if( *lexeme == 'u' ) { state = 603; lexeme++; }
            else state = 5000;

        case 603:
            if( *lexeme == 'r' ) { state = 604; lexeme++; }
            else state = 5000;

        case 604:
            if( *lexeme == 'n' ) { lexeme++; tag = RETURN; } state = 5000;

        
        case 'T':   //TRUE
            state = 700; lexeme++;
        
        case 700:
            if( *lexeme == 'R' ) { state = 701; lexeme++; }
            else state = 5000;
        
        case 701: 
            if( *lexeme == 'U' ) { state = 702; lexeme++; }
            else state = 5000;

        case 702:
            if( *lexeme == 'E' ) { lexeme++; tag = TRUE; } state = 5000;

        case 'F':   //FALSE
            state = 800; lexeme++;
        
        case 800:
            if( *lexeme == 'A' ) { state = 801; lexeme++; }
            else state = 5000;

        case 801:
            if( *lexeme == 'L' ) { state = 802; lexeme++; }
            else state = 5000;

        case 802:
            if( *lexeme == 'S' ) { state = 803; lexeme++; }
            else state = 5000;

        case 803:
            if( *lexeme == 'E'  ) { lexeme++; tag = FALSE; } state = 5000;

        case 'v':   //void
            state = 900; lexeme++;

        case 900:
            if( *lexeme == 'o' ) { state = 901; lexeme++; }
            else state = 5000;

        case 901:
            if( *lexeme == 'i' ) { state = 902; lexeme++; }
            else state = 5000;

        case 902:
            if( *lexeme == 'd' ) { lexeme++; tag = VOID; } state = 5000;

        case 'b':   //bool
            state = 1000; lexeme++;

        case 1000:
            if( *lexeme == 'o' ) { state = 1001; lexeme++; }
            else state = 5000;

        case 1001:
            if( *lexeme == 'o' ) { state = 1002; lexeme++; }
            else state = 5000;

        case 1002:
            if( *lexeme == 'l' ) { lexeme++; tag = BOOL; } state = 5000;

        case 'e':   //else
            state = 1100; lexeme++;
        
        case 1100:
            if( *lexeme == 'l' ) { state = 1101; lexeme++; }
            else state = 5000;

        case 1101:
            if( *lexeme == 's' ) { state = 1102; lexeme++; }
            else state = 5000;

        case 1102:
            if( *lexeme == 'e' ) { lexeme++; tag = ELSE; } state = 5000;
                
        case 5000:
            if( isWhitespace(*lexeme) ) 
            {
                lexeme++;
                return tag;
            }
            else 
            {
                lexeme++;
                return ID;
            }

        default:
            return ID;
    } 
}

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
        currentToken.tag = lookupAutomaton(currentToken.attribute.lexeme);
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
