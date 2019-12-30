
#include <iostream>

namespace testcpp {
class Cpp {
 public:
  explicit Cpp(std::initializer_list<int> init_list) {
    for(int i: init_list)
	  baseno_ += i;
	printf("baseno_: %ld\n", baseno_);
  } 

 private:
  long baseno_;
};

}

int main() {
	for(int i : {-1, 3, 8, 12}) {
		std::cout<< i << std::endl;
	}
	
	testcpp::Cpp p({1,3,5,7,9});

	auto d = double{5.90};
	printf("d: %lf\n", d);
	return 0;
}



