OBJ = main.o glad.o shader.o utils.o camera.o triangle.o terrain.o
CXX = clang
CC = gcc
LDFLAGS = -ldl -lstdc++ -lglfw
LDLIBS = -lm
DEBUGFLAGS = -Wall -Wshadow

reasons: $(OBJ)
	$(CXX) $(LDFLAGS) -o main $(OBJ) $(LDLIBS)

clean: 
	rm *.o main

main.o: src/main.cpp
	$(CXX) -c src/main.cpp $(DEBUGFLAGS)

glad.o: src/glad.c
	$(CC) -c src/glad.c $(DEBUGFLAGS)

shader.o: src/shader.cpp
	$(CXX) -c src/shader.cpp $(DEBUGFLAGS)

utils.o: src/utils.cpp
	$(CXX) -c src/utils.cpp $(DEBUGFLAGS)

camera.o: src/camera.cpp
	$(CXX) -c src/camera.cpp $(DEBUGFLAGS)

triangle.o: src/triangle.cpp
	$(CXX) -c src/triangle.cpp $(DEBUGFLAGS)

terrain.o: src/terrain.cpp
	$(CXX) -c src/terrain.cpp $(DEBUGFLAGS)
