/*
 * Code: front.h
 *
 * @Author FZZkill
 * @License BSD4-Clause License
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * */

#ifndef front
#define front

#include <limits.h>
#include <stdio.h>

#define first (UCHAR_MAX + 1)

#define LONGEST_KEYWORD 10
#define SHORTEST_KEYWORD 2
#define MANY_KEYWORD 18

// clang-format off
enum RTokens {
  RT_TRUE = first,
  RT_FALSE,   RT_IF,      RT_ELSE,    RT_ELSEIF,
  RT_FUNCTION,RT_WHILE,   RT_BREAK,   RT_CONTIUNE,  RT_FOR,
  RT_DO,      RT_END,     RT_IN,      RT_GLOBAL,    RT_RETURN,
  RT_NIL,     RT_GOTO,    RT_EXTERN,  RT_EOL,

  RT_EQ,    RT_NE,    RT_GE,    RT_LE,    RT_SHR, RT_SHL,
  RT_OR,    RT_AND,   RT_NOT,   RT_LINK,  RT_ANY, RT_IDIV,
  RT_SELFADD, RT_SELFSUB,

  RT_FLOAT, RT_INT, RT_STRING, RT_NAME,
};

enum Operator{
  OP_ADD,   OP_SUB,   OP_MUL,   OP_DIV,   OP_IDIV,
  OP_PUSH,  OP_POP,
  OP_MOV,   OP_GOTO,  OP_RET,
  OP_OR,    OP_AND
};

// clang-format on

static const char* const RuaTokens[] = {
  "true", "false", "if",  "else", "elseif", "function", "while", "break", "contiune",
  "for",  "do",    "end", "in",   "global", "return",   "nil",   "goto",  "RT_EXTERN",

  "==",   "!=",    ">=",  "<=",   "<<",     ">>",       "||",    "&&",    "!",
  "..",   "...",   "//",  "++",   "--"};

typedef struct tokenStruct {
	char* word;
	int type;
	unsigned int length;
	unsigned int line;
	struct tokenStruct* next;
	struct tokenStruct* back;
} tokenStruct;

typedef struct {
	const unsigned int op;
	char* arg1;
	char* arg2;
	char* arg3;
	struct parserGroup* next;
} parserStruct;

typedef struct {
	parserStruct parserGroup;
	unsigned int length;
	int hasError;
} parserGroup;

// lexer.c
tokenStruct makeTokenGroup(FILE* file);

// parser.c
parserGroup makeParserGroup(tokenStruct group);
tokenStruct rebuildTokenGroup(tokenStruct grouo);
// realize in utils.c
void freeParserGroup(parserGroup group);
void printParserGroup(parserGroup group);

tokenStruct* add_tokenStruct(tokenStruct* pos);
tokenStruct* del_tokenStruct(tokenStruct* pos);
void free_tokenStruct(tokenStruct group);
void print_tokenStruct(tokenStruct group);
int isfloat(char c);
#endif
