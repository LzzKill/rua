#include <stdio.h>

#include "front.h"

int main()
{
  const char* fp = "Tests/tokentest.rua";
  FILE* file = fopen(fp, "r");
  tokenStruct E = makeTokenGroup(file);
  print_tokenStruct(E);
  return 0;
}
