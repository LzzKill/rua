#include <stdio.h>

// clang-format off
enum RTokens {
  RT_TRUE,    RT_FALSE,   RT_IF,      RT_ELSE,      RT_ELSEIF,
  RT_FUNCTION,RT_WHILE,   RT_BREAK,   RT_CONTIUNE,  RT_FOR,
  RT_DO,      RT_END,     RT_IN,      RT_GLOBAL,    RT_RETURN,
  RT_NIL,     RT_GOTO,    RT_REGISTERED,

  RT_EQ,    RT_NE,    RT_GE,    RT_LE,    RT_SHR, RT_SHL,
  RT_OR,    RT_AND,   RT_NOT,   RT_LINK,  RT_ANY, RT_IDIV,
  RT_SELFADD, RT_SELFSUB,

  RT_FLOAT, RT_INT, RT_STRING, RT_NAME,
};
// clang-format on

static const char* const RuaTokens [] = {
   "true", "false", "if",  "else", "elseif", "function", "while", "break", "contiune",
   "for",  "do",    "end", "in",   "global", "return",   "nil",   "goto",  "registered",

   "==",   "!=",    ">=",  "<=",   "<<",     ">>",       "||",    "&&",    "!",
   "..",   "...",   "//",  "++",   "--"};

typedef struct {
	char*        word;
	int          type;
	unsigned int length;
	unsigned int line;
} tokenStruct;

typedef struct {
	tokenStruct* tokenGroup;
	unsigned int length;
  int hasError;
} tokenGroup;

#define LONGEST_KEYWORD 10
#define SHORTEST_KEYWORD 2
#define MANY_KEYWORD 18
// function

void       free_tokenGroup(tokenGroup group);
tokenGroup makeTokenGroup(const char* filename);
int        check(char* word, int len);

void printTokenGroup(tokenGroup group);
