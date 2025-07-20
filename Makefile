CXX = g++
CXXFLAGS = -Wall -std=c++17 \
    -I/home/gabriel/vcpkg/installed/x64-linux/include

LDFLAGS = \
    -L/home/gabriel/vcpkg/installed/x64-linux/lib \
    -lopengl32 -lglad -ldl -lGL

SRCS = main.cpp $(shell find src -name '*.cpp')
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET = my-opengl-project

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: compile
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
