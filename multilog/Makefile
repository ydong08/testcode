
all:build buildso

buildso:
	g++ -g -Wall -shared -fPIC -o liblog.so writelog.cpp

build:buildso
	g++ -g -Wall -o elog main.cpp -L. -llog

clean:
	-rm -rf *so log*
