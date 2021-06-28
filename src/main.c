//Standard libraries
#include <stdio.h>
#include <stdlib.h>
//#include <vector.h>

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int main( void )
{
    //Initialise GLFW
    if (!glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Basic", NULL, NULL);
    
    if (window == NULL) {
        fprintf(stderr, "Failed to create a GLFW Window.");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    do{
        
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
    //Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}