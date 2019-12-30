

# version 1.0
#build: main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o
#	cc -o main main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o


#version 2.0
#objects = main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o
#build: $(objects)
#	cc -o main $(objects)


#version 3.0
#-include src/SMakefile
#-include ../mk/MMakefile
VPATH = src:../mk
#vpath %.c ../mk src
override CAST := a b c d e 
override null :=
override space := $(null)
all:
	@echo "CAST: " $(CAST)
	@echo "space:" $(space) ":space"
objects = main.o strstr.o strtok.o test_file.o \
		  test_syscall.o testhtonll.o
build: CFLAGS = -g		   
build: $(objects)
	cc  $(CFLAGS)  -o main $(objects)
	$(run_exec)

clean:
	-rm -rf main *.o

exec:
	pwd;cd ..;pwd

define run_exec
pwd;cd ..;pwd;cd logfile
endef