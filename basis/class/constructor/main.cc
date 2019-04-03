

#include "sources/constructor.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
  constructor::CConstructor con(constructor::CConstructor::ERROR_EXCEPTION);
  constructor::CConstructor con2(con.get_right_value());
  return 0;
}