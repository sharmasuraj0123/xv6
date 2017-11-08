/*
*Simple test function to write at Memory Address 0.
*/

#include <stdio.h>

int
main(int argc, char **argv)
{
  char *np =0;
  printf("About t write at memory Adress 0\n");
  *np = 'a';
  printf("The character at memory Address 0 is: %c\n", *np);

  return 0;
}
