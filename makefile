OBJ = build/main.o build/glad.o build/shader.o build/utils.o build/camera.o build/triangle.o build/terrain.o
CXX = clang
CC = gcc
LDFLAGS = -ldl -lstdc++ -lglfw
LDLIBS = -lm
DEBUGFLAGS = -Wall -Wshadow

all: $(OBJ)
	$(CXX) $(LDFLAGS) -o main $(OBJ) $(LDLIBS)

clean: 
	rm -rf build *.o main

build/main.o: src/main.cpp
	mkdir -p build
	$(CXX) -c src/main.cpp -o build/main.o $(DEBUGFLAGS)

build/glad.o: src/glad.c
	mkdir -p build
	$(CC) -c src/glad.c -o build/glad.o $(DEBUGFLAGS)

build/shader.o: src/shader.cpp
	mkdir -p build
	$(CXX) -c src/shader.cpp -o build/shader.o $(DEBUGFLAGS)

build/utils.o: src/utils.cpp
	mkdir -p build
	$(CXX) -c src/utils.cpp -o build/utils.o $(DEBUGFLAGS)

build/camera.o: src/camera.cpp
	mkdir -p build
	$(CXX) -c src/camera.cpp -o build/camera.o $(DEBUGFLAGS)

build/triangle.o: src/triangle.cpp
	mkdir -p build
	$(CXX) -c src/triangle.cpp -o build/triangle.o $(DEBUGFLAGS)

build/terrain.o: src/terrain.cpp
	mkdir -p build
	$(CXX) -c src/terrain.cpp -o build/terrain.o $(DEBUGFLAGS)
