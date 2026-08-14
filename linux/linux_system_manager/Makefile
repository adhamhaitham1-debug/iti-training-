CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TARGET = linux_system_manager

SRCS = main.cpp SystemInfo.cpp ProcessManager.cpp FileManager.cpp NetworkManager.cpp ThreadDemo.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)