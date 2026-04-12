//******************************************************************************

#include "text.h"
#include "matrix.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define AMBER 238, 173, 14

static inline void delay(int ms)
{
  usleep(ms * 1000);
}

//******************************************************************************

int main(int ac, char *av[])
{
  matrix_create();

  print_frame(AMBER);
  for (int i = 0; i < ROWS && av[i + 1]; i++) 
    print_str(i, av[i + 1], AMBER);

  matrix_flush();
  delay(10000);

  matrix_free();
  return EXIT_SUCCESS;
}

//******************************************************************************
