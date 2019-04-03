
#ifndef CPP_CONSTRUCTOR_CONSTRUCTOR_H_
#define CPP_CONSTRUCTOR_CONSTRUCTOR_H_

#include "base/base.inc"

using std;
class CUtil;

namespace constructor {

class CConstructor : public CConstructorBase
{
 public:
  typedef std::vector<int> IntVector;
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
  void set_err_code(int err_code) { err_code_ = err_code; };
  int  exec_bussiness(const int kernel_id, 
                      const int bussiness_no, 
                      const char* data_stream);

 private:
  DISALLOW_COPY_AND_ASSIGN(CConstructor);

  const int kInstanceNum = 8;
  int err_code_;

  CUtil *util_;

};

}

#endif  // CPP_CONSTRUCTOR_CONSTRUCTOR_H_