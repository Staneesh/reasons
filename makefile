reasons: src/main.cpp src/glad.c 
	bear clang++ -o main -ldl -lstdc++ -lglfw src/main.cpp src/glad.c
