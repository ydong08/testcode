

LINKLIB= -lpthread -lrt
vpath %.inc base
vpath %.cc constructor

all: main.o constructor.o
	@echo "$(CXX)"
	$(CXX) $(LINK) $^ -o $@ $(LINKLIB)

constructor.o: constructor.cc constructor.h base.h util.h	
	$(CXX) -c $(LINK) $<
	@echo "Tar: $@, first dep: $<, all dep: $^"

main.o: main.cc constructor.h	
	$(CXX) -c $(LINK) $<
	@echo "Tar: $@, first dep: $<, all dep: $^"

clean:
	-rm -rf main
	-rm -rf *.o

.PHONY: clean
