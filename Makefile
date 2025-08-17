CXX = g++
CXXFLAGS = -Wall -std=c++17 \
    -I/home/gabriel/Desktop/c++/opengl/vcpkg_installed/x64-linux/include \
    -Isrc/Render

LDFLAGS = \
    -L/home/gabriel/Desktop/c++/opengl/vcpkg_installed/x64-linux/lib \
    -lglfw -lglad -lGL -ldl

TARGET = my-opengl-project

SRCS = $(shell find src -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

.PHONY: compile run clean

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: compile
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
