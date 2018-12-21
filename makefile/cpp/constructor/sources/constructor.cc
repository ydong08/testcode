/*
* Copyright 2018 Castles Inc.
* License MIT
* Author: winter
* Version: 1.0
* 
*/

#include "constructor.h"
#include "base/util.inc"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

#define MAX_KERNEL_ID            10
#define MAX_BUSSNESS_NO          16


using std::vector;

namespace constructor {

int CConstructor::service_code = 10;
CConstructor::CConstructor(int err_code) 
    : err_code_(err_code),
      util_(nullptr) {

      util_ = new CUtil;
      printf("util addr: %#X\n", util_);
}

CConstructor::~CConstructor(void) {
  if (util_)
    delete util_;
}

CConstructor::CConstructor(CConstructor &&con) {
  err_code_ = con.err_code_;
  con.err_code_ = 0;

  util_ = con.util_;
  con.util_ = nullptr;
  printf("util addr: %#X\n", util_);
}

CConstructor CConstructor::get_right_value() {
  CConstructor con(20);
  return con;
}

int CConstructor::err_code() {
  return err_code_;
}

int  CConstructor::exec_bussiness(const int kernel_id, 
                                  const int bussiness_no, 
                                  const char* data_stream) const {
  int len = 0;                                             
  if (0 < kernel_id && kernel_id <= MAX_KERNEL_ID) {
    for (int i = 0; i < MAX_BUSSNESS_NO; ++i) {
      if (bussiness_no * 2 < MAX_BUSSNESS_NO) {
        len = strlen(data_stream);
        printf("data stream len: %d\n", len);
      }
    }
  }
  return 0;
}


}
