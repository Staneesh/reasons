
OBJ = main.o glad.o shader.o utils.o camera.o triangle.o terrain.o
CXX = clang
CC = gcc
LDFLAGS = -ldl -lstdc++ -lglfw
LDLIBS = -lm

reasons: $(OBJ)
	$(CXX) $(LDFLAGS) -o main $(OBJ) $(LDLIBS)

clean: 
	rm *.o main

main.o: src/main.cpp
	$(CXX) -c src/main.cpp

glad.o: src/glad.c
	$(CC) -c src/glad.c

shader.o: src/shader.cpp
	$(CXX) -c src/shader.cpp

utils.o: src/utils.cpp
	$(CXX) -c src/utils.cpp

camera.o: src/camera.cpp
	$(CXX) -c src/camera.cpp

triangle.o: src/triangle.cpp
	$(CXX) -c src/triangle.cpp

terrain.o: src/terrain.cpp
	$(CXX) -c src/terrain.cpp


