all: src/main.c
	g++ src/main.c -o final -lGLEW -lglfw -lGL -lGLU  

