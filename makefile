reasons: src/main.cpp src/glad.c src/shader.cpp src/utils.cpp src/camera.cpp
	bear clang -o main -ldl -lstdc++ -lglfw src/main.cpp src/glad.c src/shader.cpp src/utils.cpp src/camera.cpp
