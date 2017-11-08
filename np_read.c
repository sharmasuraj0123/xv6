/*
*Simple test function to READ from Memory Address 0.
*/

#include <stdio.h>

int
main(int argc, char **argv)
{
  char *np =0;
  printf("About to read a characterfro  memory Adress 0\n");
  printf("The character at memory Address 0 is: %c\n", *np);
  return 0;
}
