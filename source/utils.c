#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "front.h"

/*
 * Code: utils.c
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

// void freeParserGroup(parserGroup group)
// {
// 	for(unsigned int i = 0; i < group.length; i++) {
// 		free(group.parserGroup[i].arg1);
// 		free(group.parserGroup[i].arg2);
// 		free(group.parserGroup[i].arg2);
// 	}
// 	free(group.parserGroup);
// }
//
//
// void printParserGroup(parserGroup group)
// {
// 	for(unsigned int i = 0; i < group.length; i++)
// 		printf(" Parser{ Operatpr = %d, Arg1 = %s , Arg2 = %s, Arg4 = %s}",
// 		       group.parserGroup[i].op,
// 		       group.parserGroup[i].arg1,
// 		       group.parserGroup[i].arg2,
// 		       group.parserGroup[i].arg3);
// }


/*
 *  @function add_tokenStruct
 *
 *  Get a new tokenStruct.
 *  And add it in this.
 * */
tokenStruct* add_tokenStruct(tokenStruct* pos)
{
	pos->next = (tokenStruct*)calloc(1, sizeof(tokenStruct));
	if(pos->next == NULL) fprintf(stderr, "Memcpy Error: Memory Overflow");

	tokenStruct* tmp = pos;
	pos = pos->next;
	pos->back = tmp;
	pos->next = NULL;
  return pos;
}


/*
 *  @function del_tokenStruct
 *
 *  Delete the structure pointed to by pos.
 *  And free it
 * */
tokenStruct* del_tokenStruct(tokenStruct* pos)
{
	if(pos->next == NULL) {// last node
		pos = pos->back;
		free(pos->next);
		return pos;
	}
	pos->back = pos->next;
	struct tokenStruct* tmp = pos->back;
	free(pos);
	pos = tmp;
  return pos;
}


/*
 *  @function free_tokenStruct
 *  
 *  Release this linked list.
 * */
void free_tokenStruct(tokenStruct group)
{
	tokenStruct* P = &group;
	do {
		P = P->next;
	} while(P->next != NULL);
  P = P->back;
	do {
		free(P->next);
		P = P->back;
	} while(P->back != NULL);
}


/*
 *  @function print_tokenSroup
 *
 *  Iterate through this linked list.
 *  And print it.
 * */
void print_tokenStruct(tokenStruct group)
{
	tokenStruct* P = &group;
	do {
		printf("Token{ type = %d wordlen = %d line = %d word=%s } \n",
		       P->type,
		       P->length,
		       P->line,
		       P->word);
		P = P->next;
	} while(P->next != NULL);
	printf("Token{ type = %d wordlen = %d line = %d word=%s } \n",
	       P->type,
	       P->length,
	       P->line,
	       P->word);
}


inline int isfloat(char c)
{
	int i = c;
	return (i >= '0' && i <= '9') || i == '.';
}

