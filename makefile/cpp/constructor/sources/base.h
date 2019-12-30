
#ifndef CONSTRUCTOR_SOURCES_BASE_H_
#define CONSTRUCTOR_SOURCES_BASE_H_

namespace constructor {

class CConstructorBase {
 public:
  CConstructorBase(void);
  ~CConstructorBase(void);
  explicit CConstructorBase(int);

 private: 
  int flag_;
};

}


#endif    // CONSTRUCTOR_SOURCES_BASE_H_