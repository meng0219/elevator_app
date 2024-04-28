CC = g++
CFLAGS = -std=c++11
TARGET = elevator_app monitor_app
SRCS = main.cpp elevator.cpp monitor.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

elevator_app: main.o elevator.o
	$(CC) $(CFLAGS) $^ -o $@

monitor_app: monitor.o
	$(CC) $(CFLAGS) $^ -o $@
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)

