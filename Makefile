# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Define target output
TARGET = CppWebServer

# Define source files
SRC = server.cpp

# Define the default target
all: $(TARGET)

# Rule to compile the target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Rule to run the server after compiling
run: $(TARGET)
	./$(TARGET)

# Rule to clean up compiled files
clean:
	rm -f $(TARGET)

# Rule to recompile and run the server
rebuild: clean all run

.PHONY: all run clean rebuild

