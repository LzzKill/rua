#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Error.h"
#include "Lexer.h"


int check(char* word, int len)
{
	if(len >= SHORTEST_KEYWORD && len <= LONGEST_KEYWORD) {
		for(int i = 0; i < MANY_KEYWORD; i++) {
			if(!strcmp(word, RuaTokens[i])) return i - 40;
		}
	}
	return RT_NAME;
}
#define nextchar(c) nextChar(file, c)

inline char nextChar(FILE* file, char c)
{
	if(fgetc(file) == c) return 1;
	fseek(file, -1, SEEK_CUR);
	return 0;
}

inline int isfloat(char c)
{
	int i = c;
	return ((i >= '0' && i <= '9') || i == '.') ? 1 : 0;
}

tokenStruct readnumber(FILE* file, unsigned int line)
{
	fseek(file, -1L, SEEK_CUR);
	tokenStruct T = {calloc(2, sizeof(char)), RT_INT, 0, line};
	char        c;
	while((c = fgetc(file)) != EOF && isfloat(c)) {
		if(c == '.' && T.type == RT_FLOAT)
			fprintf(stderr, "Too many dots in line %d", line);
		else
			T.type = RT_FLOAT;
		T.word             = realloc(T.word, sizeof(char) * (T.length + 1));
		T.word[T.length++] = c;
	}
	T.word             = realloc(T.word, sizeof(char) * (T.length + 1));
	T.word[T.length++] = '\0';
	fseek(file, -1L, SEEK_CUR);
	return T;
}

#define newTokenGroup                                                                          \
	G.tokenGroup                  = realloc(G.tokenGroup, sizeof(tokenStruct) * (G.length + 1)); \
	G.tokenGroup[G.length].line   = line;                                                        \
	G.tokenGroup[G.length].length = 0;                                                           \
	G.tokenGroup[G.length].type   = 0;                                                           \
	G.tokenGroup[G.length].word   = NULL;

#define endTokenGroup                                                 \
	if(length) {                                                        \
		newTokenGroup;                                                    \
		int i;                                                            \
		if((i = check(word, length)) == RT_NAME) {                        \
			G.tokenGroup[G.length].word = calloc(length + 3, sizeof(char)); \
			memcpy(G.tokenGroup[G.length].word, word, length);              \
			G.tokenGroup[G.length].word[length++] = '\0';                   \
			G.tokenGroup[G.length].length         = length;                 \
			G.tokenGroup[G.length++].type         = RT_NAME;                \
		}                                                                 \
		else                                                              \
			G.tokenGroup[G.length].type = i;                                \
		length = 0;                                                       \
		word   = (char*)calloc(2, sizeof(char));                          \
	}
#define EndTokenGroup \
	endTokenGroup;      \
	newTokenGroup;      \
	if(length) G.tokenGroup[G.length].type = RT_EOL;


tokenGroup makeTokenGroup(FILE* file)
{
	tokenGroup G    = {NULL, 0, 0};
	char       c;

	unsigned int line = 0, length = 0;
	char*        word = NULL;
	while((c = fgetc(file)) != EOF) {
		switch(c) {
			case ';':
			case '\n': EndTokenGroup; break;

			case '\r': G.tokenGroup[G.length].line++;
			case '\f':
			case ' ': endTokenGroup; break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++] = readnumber(file, line);
				break;
			}
			case '@':
			case '$':
			case '%':
			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
			case '^':
			case ':':
			case '?':
			case ',':
			case '*': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length].type = c;
				break;
			}
			case '\'':
			case '"': {
				endTokenGroup;
				char tmp = c;
				while((tmp = fgetc(file)) != c) {
					if(tmp == EOF) {
						fprintf(stderr, "No string endchar in line %d", line);
						G.hasError++;
					}
					word           = (char*)realloc(word, sizeof(char) * (length + 1));
					word[length++] = tmp;
				}
				newTokenGroup;
				G.tokenGroup[G.length].word = calloc(length + 3, sizeof(char));
				memcpy(G.tokenGroup[G.length].word, word, length);
				G.tokenGroup[G.length].word[length++] = '\0';
				G.tokenGroup[G.length].length         = length;
				G.tokenGroup[G.length++].type         = RT_STRING;
				length                                = 0;
				word                                  = (char*)calloc(2, sizeof(char));
				break;
			}
			case '#': {
				endTokenGroup;
				char Temp_c;
				while((Temp_c = fgetc(file)) != EOF && Temp_c != '\n' && Temp_c != '#')
					;
				break;
			}
			case '+': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('+') ? RT_SELFADD : '+');
				break;
			}
			case '-': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('-') ? RT_SELFSUB : '-');
				break;
			}
			case '/': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('/') ? RT_IDIV : '/');
				break;
			}
			case '=': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('=') ? RT_EQ : '=');
				break;
			}
			case '!': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('=') ? RT_NE : '!');
				break;
			}
			case '>': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('>') ? RT_SHR : (nextchar('=') ? RT_GE : '>'));
				break;
			}
			case '<': {
				endTokenGroup;
				newTokenGroup;
				G.tokenGroup[G.length++].type = (nextchar('<') ? RT_SHL : (nextchar('=') ? RT_LE : '<'));
				break;
			}
			default: word = (char*)realloc(word, sizeof(char) * (length + 1)); word[length++] = c;
		}
	}
	if(word != NULL) free(word);
	return G;
}

void free_tokenGroup(tokenGroup group)
{
	for(unsigned int i = 0; i < group.length; i++)
		free(group.tokenGroup[i].word);
	free(group.tokenGroup);
}

void printTokenGroup(tokenGroup group)
{
	for(unsigned int i = 0; i < group.length; i++)
		printf("{ type = %d wordlen = %d line = %d word=%s } \n",
		       group.tokenGroup[i].type,
		       group.tokenGroup[i].length,
		       group.tokenGroup[i].line,
		       group.tokenGroup[i].word);
}
