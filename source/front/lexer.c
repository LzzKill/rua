#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../front.h"

/*
 * Code: lexer.c
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

int check(char* word, int len)
{
	if(len >= SHORTEST_KEYWORD && len <= LONGEST_KEYWORD) {
		for(int i = 0; i < MANY_KEYWORD; i++) {
			if(!strcmp(word, RuaTokens[i])) return i + first;
		}
	}
	return RT_NAME;
}

/*
 *  @function end_tokenStruct
 *
 *  Finalize all values pointed to by pos.
 * */
void end_tokenStruct(tokenStruct* pos, char* word, unsigned length, unsigned line)
{
	if(!length) return;
	pos->type = check(word, length);
  pos->line = line;
	if(pos->type == RT_NAME) {
		pos->word = (char*)calloc(length + 2, sizeof(char));
		memcpy(pos->word, word, length);
		pos->word[length + 1] = '\0';
	}
}

char nextchar(FILE* file, char c)
{
	if(fgetc(file) == c) return 1;
	fseek(file, -1, SEEK_CUR);
	return 0;
}

void readnumber(tokenStruct* pos, FILE* file, unsigned int line)
{
	fseek(file, -1L, SEEK_CUR);
	pos->type = RT_INT;
	char c;
	while((c = fgetc(file)) != EOF && isfloat(c)) {
		if(c == '.' && pos->type == RT_FLOAT)
			fprintf(stderr, "Too many dots in line %d", line);
		else
			pos->type = RT_FLOAT;
		pos->word = realloc(pos->word, sizeof(char) * (pos->length + 1));
		pos->word[pos->length++] = c;
	}
	pos->word = realloc(pos->word, sizeof(char) * (pos->length + 1));
	pos->word[pos->length++] = '\0';
	pos->line = line;
	fseek(file, -1L, SEEK_CUR);
}

tokenStruct makeTokenGroup(FILE* file)
{
	tokenStruct G = {NULL, 0, 0, 0, NULL, NULL};
	tokenStruct* pos = &G;
	unsigned int line = 0, length = 0;
	char* word = NULL;
	char c;

	while((c = fgetc(file)) != EOF) {
		if(isfloat(c)) {
			end_tokenStruct(pos, word, length, line);
			length = 0;
			pos = add_tokenStruct(pos);
			readnumber(pos, file, line);
			pos = add_tokenStruct(pos);
			continue;
		}

		switch(c) {
			case '\n':
			case '\r': line++;
			case '\f':
			case ' ': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
				pos = add_tokenStruct(pos);
				continue;
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
				end_tokenStruct(pos, word, length, line);
				length = 0;
				pos = add_tokenStruct(pos);
				pos->word = NULL;
				pos->type = c;
				pos->length = 0;
				pos->line = line;
				break;
			}

			case '\'':
			case '"': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				char tmp = c;
				while((tmp = fgetc(file)) != c) {
					if(tmp == EOF) {
						fprintf(stderr, "No string endchar in line %d", line);
            fflush(stderr);
            exit(1);
					}
					word = (char*)realloc(word, sizeof(char) * (length + 1));
					word[length++] = tmp;
				}
				pos->word = calloc(length + 3, sizeof(char));
				memcpy(pos->word, word, length);
				pos->word[length++] = '\0';
				pos->length = length;
				pos->type = RT_STRING;
				length = 0;
				word = (char*)calloc(2, sizeof(char));
				pos = add_tokenStruct(pos);
				break;
			}
			case '#': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				char Temp_c;
				while((Temp_c = fgetc(file)) != EOF && Temp_c != '\n' && Temp_c != '#')
					;
				break;
			}
			case '+': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '+') ? RT_SELFADD : '+');
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '-': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '-') ? RT_SELFSUB : '-');
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '/': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '/') ? RT_IDIV : '/');
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '=': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '=') ? RT_EQ : '=');
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '!': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '=') ? RT_NE : '!');
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '>': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '>') ? RT_SHR : (nextchar(file, '=') ? RT_GE : '>'));
				pos->length = 0;
        pos->line = line;
				break;
			}
			case '<': {
				end_tokenStruct(pos, word, length, line);
				length = 0;
        pos = add_tokenStruct(pos);
				pos->type = (nextchar(file, '<') ? RT_SHL : (nextchar(file, '=') ? RT_LE : '<'));
				pos->length = 0;
        pos->line = line;
				break;
			}
			default: word = (char*)realloc(word, sizeof(char) * (length + 1)); word[length++] = c;
		}
	}
	if(word != NULL) free(word);
	return G;
}
