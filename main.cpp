// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "headers/shader.hpp"
#include "headers/texture.hpp"
#include "headers/controls.hpp"
#include "headers/objloader.hpp"
//#include "../common/vboindexer.hpp"


int main( void )
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1366, 768, "Project Demo", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1366/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader" );
	//printf("Hey reached here!\n");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	
	
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	all_vertices m_vertices;
	all_UVs m_uvs;
	all_normals m_normals;
	std::vector<std::string> name_list;

	int t_mesh;
	
	bool res2 = DoTheImportThing("static/mesh/basic-forest2.obj", t_mesh, m_vertices, m_uvs, m_normals, name_list);
	//printf("Hey reached here!\n");
	printf("No of meshes %d\n", t_mesh);
	
	// Load the texture
	GLuint Texture[t_mesh];
	for(int i=0; i<t_mesh; i++)
	{	
		std::cout << name_list[i] << std::endl;
		if (name_list[i].compare(0,4,"Tree") == 0)
		{
			Texture[i] = loadDDS("static/textures/Tree.dds");
		}
		else if (name_list[i].compare(0,4,"stem") == 0)
		{
			Texture[i] = loadDDS("static/textures/stem.dds");
		}
		else if (name_list[i].compare(0,4,"Road") == 0)
		{
			Texture[i] = loadDDS("static/textures/Road.dds");
		}
		else if (name_list[i].compare(0,4,"Land") == 0)
		{
			Texture[i] = loadDDS("static/textures/Land.dds");
		}
		else {
			Texture[i] = loadDDS("static/textures/texture1.dds");
		}
	}

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	vertices = m_vertices[0];
	uvs = m_uvs[0];
	normals = m_normals[0];

	// Load it into a VBO
	GLuint vertexbuffer[t_mesh];
	for(int i=0; i<t_mesh; i++)
	{
		glGenBuffers(1, &vertexbuffer[i]);
		//printf("%u\n", vertexbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, m_vertices[i].size() * sizeof(glm::vec3), &m_vertices[i][0], GL_STATIC_DRAW);
	}
	
	
	GLuint uvbuffer[t_mesh];
	for(int i=0; i<t_mesh; i++)
	{
		glGenBuffers(1, &uvbuffer[i]);
		//printf("%u\n", uvbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, m_uvs[i].size() * sizeof(glm::vec2), &m_uvs[i][0], GL_STATIC_DRAW);
	}
	

	GLuint normalbuffer[t_mesh];
	for(int i=0; i<t_mesh; i++)
	{
		glGenBuffers(1, &normalbuffer[i]);
		//printf("%u\n", normalbuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, m_normals[i].size() * sizeof(glm::vec3), &m_normals[i][0], GL_STATIC_DRAW);
	}
	
	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(0,50, -5);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


		//Loop for each of our mesh.
		for(int i=0; i<t_mesh; i++)
		{
			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[i]);
			glVertexAttribPointer(
				1,                                // attribute
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[i]);
			glVertexAttribPointer(
				2,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, Texture[i]);
			// Set our "myTextureSampler" sampler to use Texture Unit 0
			glUniform1i(TextureID, 0);

			// Draw the triangles !
			//printf("Hey reached here!");
			glDrawArrays(GL_TRIANGLES, 0, m_vertices[i].size());
			
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	for(int i=0; i<t_mesh; i++)
	{
		glDeleteBuffers(1, &vertexbuffer[i]);
		glDeleteBuffers(1, &uvbuffer[i]);
		glDeleteBuffers(1, &normalbuffer[i]);
		glDeleteTextures(1, &Texture[i]);
	}
	
	glDeleteProgram(programID);
	
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

    return 0;
}
