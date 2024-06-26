CFLAGS = -g -Wall -std=c++11 -I /home/ubuntu/OtherGithub/WebServer/qinguoyi/TinyWebServer/log/
LIBS =  -lpthread

SRCS = log_test.cpp ../log/log.cpp
EXECUTABLE = logtest
CC = g++

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRCS) $(LIBS)

.PHONY:clean
clean:
	rm $(EXECUTABLE)
