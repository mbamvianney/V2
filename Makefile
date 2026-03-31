CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude
TARGET = student_split
SOURCES = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) studentai.txt

clean:
	rm -f $(TARGET)
	rm -rf output

.PHONY: all run clean
