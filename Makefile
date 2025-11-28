# Makefile
CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -Iinclude
LDFLAGS = -lSDL2

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Find all .cpp files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Convert .cpp filenames to .o filenames inside obj/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = raycaster

# The Default Rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
