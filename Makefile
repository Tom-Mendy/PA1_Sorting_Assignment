CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET := pa1_sorting

SRCS := $(wildcard src/*.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "Remove-Item -Force -ErrorAction SilentlyContinue 'src\\*.o', '$(TARGET)'; exit 0"
else
	rm -f $(OBJS) $(TARGET)
endif

.PHONY: all run clean
