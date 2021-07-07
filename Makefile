#-lX11 -lXrandr -lpthread -lXi -lm
all: main.cpp
	g++ main.cpp src/controls.cpp src/objloader.cpp src/shader.cpp src/texture.cpp -lGLEW -lglfw -lGL -lGLU -lassimp -o bin/final

