#ifndef CONSTRUCTOR_BASE_BASE_H_
#define CONSTRUCTOR_BASE_BASE_H_

namespace constructor {

#define DISALLOW_COPY_AND_ASSIGN(classname) \
private:                                    \
 classname(const classname&);               \
 classname& operator= (const classname&);

#define DISLAAOW_IMPLICIT_CONSTRUCTORS(classname) \
private:                                          \
 classname(void);                                 \
 DISALLOW_COPY_AND_ASSIGN(classname);

#define DECLARE_SINGLETON(classname)        \
public:                                     \
 static classname* instance() {             \
     static classname cn;                   \
     return &cn;                            \
 }                                          \
                                            \
DISALLOW_IMPLICIT_CONSTRUCTORS(classname)   \
 private:
 

class CConstructorBase {
 public:
  CConstructorBase(void);
  ~CConstructorBase(void);
  explicit CConstructorBase(int);

 private: 
  int flag_;
};

}
#endif   // CONSTRUCTOR_BASE_BASE_H_