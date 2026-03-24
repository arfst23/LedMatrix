//******************************************************************************

#include "text.h"
#include "matrix.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

int main(int ac, char *av[])
{
  matrix_create();

  for (int i = 0; i < ROWS && av[i + 1]; i++) 
    print_str(i, av[i + 1], 238, 173, 14);

  matrix_flush();
  delay(20000);

  matrix_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
