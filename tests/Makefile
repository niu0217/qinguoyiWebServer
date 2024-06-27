CFLAGS = -g -Wall -std=c++11
LIBS =  -lpthread -lmysqlclient

SRCS = mysql_test.cpp
EXECUTABLE = mysql_test
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
