
#ifndef CPP_CONSTRUCTOR_CONSTRUCTOR_H_
#define CPP_CONSTRUCTOR_CONSTRUCTOR_H_

#include <vector>   // 头文件中尽量使用前置声明,stl类除外,使用头文件
#include "base.h"
#include "base/base.inc"
#include "base/util.inc"

using std::vector;  // forbidden to use 'using namespace xxx'

namespace constructor {

class CConstructor : public CConstructorBase
{
 public:
  typedef vector<int> IntVector;
  enum ErrorCode {
    ERROR_NULL_POINTER = 0,
    ERROR_OVERFLOW,
    ERROR_EXCEPTION
  };

  explicit CConstructor(int);
  explicit CConstructor(CConstructor &&con);
  ~CConstructor(void);

  CConstructor get_right_value();
  int  err_code();
  void set_err_code(int err_code) { err_code_ = err_code; }
  int  exec_bussiness(const int kernel_id, 
                      const int bussiness_no, 
                      const char* data_stream) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(CConstructor);

  const int kInstanceNum = 8;
  int err_code_;
  static int service_code;

  CUtil *util_;

};

}

#endif  // CPP_CONSTRUCTOR_CONSTRUCTOR_H_