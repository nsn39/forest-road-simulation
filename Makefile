SRCDIR = src
BINDIR = bin
HEADERS = headers

#The entrypoint of our application
MAIN_FILE = main.cpp

#Source files
FILES = controls.cpp glfw.cpp objloader.cpp shader.cpp texture.cpp
OBJS = $(addprefix $(SRCDIR)/, $(FILES))
EXEC = $(BINDIR)/demo

#The compiler which we're using
CC = g++

##COMPILER_FLAGS specifies the additional compilation options we're using.
##-w suppresses all the warnings.
COMPILER_FLAGS = -w 

##LINKER_FLAGS specifies the libraries we're linking against.
LINKER_FLAGS = -lGLEW -lglfw -lGL -lGLU -lassimp

##OBJ_NAME specifies the name of our executable
OBJ_NAME = $(EXEC)

#-lX11 -lXrandr -lpthread -lXi -lm
all: $(MAIN_FILE) $(OBJS)
	$(CC) $(MAIN_FILE) $(OBJS) ${LINKER_FLAGS} -o ${OBJ_NAME}

