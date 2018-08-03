

# version 1.0
#build: main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o
#	cc -o main main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o


#version 2.0
#objects = main.o strstr.o strtok.o test_file.o	test_syscall.o testhtonll.o
#build: $(objects)
#	cc -o main $(objects)


#version 3.0
vpath = src:../mk
objects = $(wildcard *.o)
build: $(objects)
	cc    -o main $(objects)

clean:
	-rm -rf main *.o