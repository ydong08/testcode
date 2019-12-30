

#include "sources/constructor.h"
#include <stdio.h>
#include <stdlib.h>

#define Constructor  constructor::CConstructor

int main() {
  Constructor con(Constructor::ERROR_EXCEPTION);
  Constructor con2(con.get_right_value());
  return 0;
}