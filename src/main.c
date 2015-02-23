#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"

int main(int argc, const char *argv[]) {
  if (argc <= 1) {
    rsiv_open();
  } else if (argc > 1 && strcmp(argv[1], "-h") == 0) {
    rsiv_help();
  } else {
    rsiv_open_image(argv[1]);
  }
  return 0;
}
