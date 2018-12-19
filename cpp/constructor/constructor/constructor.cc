/*
* Copyright 2018 Castles Inc.
* License MIT
* Author: winter
* Version: 1.0
* 
*/

#include "constructor.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "base/constructor_base.h"
#include "base/constructor_util.h"

#define MAX_KERNEL_ID            10
#define MAX_BUSSNESS_NO          16


using std::vector;

namespace constructor {

CConstructor::CConstructor(void) 
    : err_code_(0),
    : util_(nullptr) {


}

int CConstructor::err_code() {
  return err_code_;
}

void CConstructor::set_err_code(const int err_code) {
  err_code_ = err_code;
}

int  CConstructor::exec_bussiness const (const int kernel_id, 
                                         const int bussiness_no, 
                                         const char* data_stream) {
  int len = 0;                                             
  if (0 < kernel_id && kernel_id <= MAX_KERNEL_ID) {
    for (int i = 0; i < MAX_BUSSNESS_NO; ++i) {
      if (bussiness_no * 2 < MAX_BUSSNESS_NO) {
        len = strlrn(data_stream);
        printf("data stream len: %d\n", len);
      }
    }
  }
}


}


int main() {

}