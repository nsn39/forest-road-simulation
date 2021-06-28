#-lX11 -lXrandr -lpthread -lXi -lm
all: main.c
	g++ main.c src/controls.cpp src/objloader.cpp src/shader.cpp src/texture.cpp -lGLEW -lglfw -lGL -lGLU -o final

