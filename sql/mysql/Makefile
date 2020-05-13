
CC = gcc  -g

CFLAGS += -I/usr/include/mysql
LIB_PATH += -L/usr/lib/x86_64-linux-gnu/
LDFLAGS += -lmysqlclient

OBJ=ACDBAccess.o main.o
MYSRC=$(OBJ:.o=.c)

target = mysql
all: $(target)

$(target):$(OBJ)
	$(CC) $(CFLAGS) $(LIB_PATH) $(LDFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -rf *.o mysql
