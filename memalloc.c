#include <stdio.h>
#include <stdlib.h>

#define NGIGA 1000

int main(void)
{
  int i, j;

  int kilo=1024;
  int mega=kilo*kilo;
  int giga=mega*kilo;

  char *x[NGIGA];

  printf("Doing dummy malloc of %d GiB in 1 GiB chunks\n", NGIGA, giga);

  for (i=0; i < NGIGA; i++)
    {
      x[i] = (char *) malloc(giga);

      for (j=0; j < giga; j++)
	{
	  x[i][j] = 'x';
	}

      printf("Completed i = %d: x[%d][%d] = %d\n", i, i, giga-1, x[i][giga-1]);
    }
  return 0;
}
