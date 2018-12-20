
# := 先定义再使用
# = 可以先使用,后面定义
INC := -I. -Ibase -Isources
LINK := -g -Wall -std=c++11
LINKLIB := -lpthread -lrt

vpath % .
vpath %.inc base
vpath %.cc sources

variable := "20181220"
test:
	@echo "variable := '20181221'"
	export variable
	@echo "variable := $(variable)"
	@echo "who := $(shell whoami)"
	@echo "curpath := $(shell pwd)"
	
#	@echo "$(who)"
#	@echo "$(curpath)"

#OBJ= main.o base.o constructor.o
#all: $(OBJ)
#        @echo "$(CXX)"
#        $(CXX) $(LINK) $^ -o $@ $(LINKLIB)
#$(OBJ): %.o : %.cc
#        $(CXX) -c $(LINK) $(INC) $< -o $@
#        @echo "Tar: $@, first dep: $<, all dep: $^"

all: main.o constructor.o base.o
	@echo "$(CXX)"
	$(CXX) $(LINK) $(INC) $^ -o $@ $(LINKLIB)

constructor.o: 	
	$(CXX) -c $(LINK) $(INC) constructor.cc
	@echo "Tar: $@, first dep: $<, all dep: $^"

base.o: 	
	$(CXX) -c $(LINK) $(INC) base.cc
	@echo "Tar: $@, first dep: $<, all dep: $^"

main.o: 
	$(CXX) -c $(LINK) $(INC) main.cc
	@echo "Tar: $@, first dep: $<, all dep: $^"

clean:
	-rm -rf main
	-rm -rf *.o

.PHONY: clean all
