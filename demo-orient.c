//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "orient.h"

#define delay(x) usleep(x * 1000)

#define TERM_CLEAR "\e[2J"
#define TERM_HOME "\e[1;1H"

//******************************************************************************

int main()
{
  orient_create();

  printf(TERM_CLEAR);
  while (1)
  {
    orient_t orient = orient_get();
    
    printf(TERM_HOME);
    printf("+------------------------+------------------------+\n");

    int x = orient.x / 40 + 25;
    for (int i = 0; i < x; i++)
      putchar(' ');
    putchar('x');
    for (int i = x; i < 51; i++)
      putchar(' ');
    putchar('\n');
    
    int y = orient.y / 40 + 25;
    for (int i = 0; i < y; i++)
      putchar(' ');
    putchar('y');
    for (int i = y; i < 51; i++)
      putchar(' ');
    putchar('\n');
    
    int z = orient.z / 40 + 25;
    for (int i = 0; i < z; i++)
      putchar(' ');
    putchar('z');
    for (int i = z; i < 51; i++)
      putchar(' ');
    putchar('\n');

    printf("+------------------------+------------------------+\n");

    printf("\n");
    printf("%4d  %4d  %4d\n", orient.x, orient.y, orient.z);
    delay(200);
  }

  orient_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
