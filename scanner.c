#include "stdio.h"

int Scan(FILE *file) {
  int c;

  while (1) {

    c = fgetc(file);
    if (feof(file)) {
      break;
    }
    printf("%c", c);
  }

  return 1;
}