#define STB_IMAGE_IMPLEMENTATION

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "headers/shader.hpp"
#include "headers/texture.hpp"
#include "headers/controls.hpp"
#include "headers/objloader.hpp"
#include "headers/glfw.hpp"
#include "src/transformations.cpp"
#include "library/stb_image.h"

GLFWwindow* window;

unsigned int loadCubemap(std::vector<std::string> faces);

int main( void )
{	
	float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	//Initialize a GLFW Window
	window = glfw_init();

	// Dark blue background
	glClearColor(0.529f, 0.807f, 0.921f, 0.0f);

	// Enable depth test using Z-buffer algorithm.
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE); 



	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");
	GLuint programID2 = LoadShaders("shaders/skybox.vertexshader", "shaders/skybox.fragmentshader");
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
	
	bool res2 = import_obj_mesh("static/mesh/basic_forest_2_exp.obj", t_mesh, m_vertices, m_uvs, m_normals, name_list);
	printf("No of meshes %d\n", t_mesh);
	
	// skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces
    {
        "static/skybox/right.jpg",
        "static/skybox/left.jpg",
        "static/skybox/top.jpg",
        "static/skybox/bottom.jpg",
        "static/skybox/front.jpg",
        "static/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
	std::string skybox_shader_name = "skybox";
	glUseProgram(programID2);
	glUniform1i(glGetUniformLocation(programID2, skybox_shader_name.c_str()), 0);

	// Load the texture
	GLuint Texture[t_mesh];
	for(int i=0; i<t_mesh; i++)
	{	
		//std::cout << name_list[i] << std::endl;
		if (name_list[i].compare(0,4,"Hill") == 0)
		{
			Texture[i] = loadDDS("static/textures/hill.dds");
		}
		else if (name_list[i].compare(0,4,"stem") == 0)
		{
			Texture[i] = loadDDS("static/textures/stem.dds");
		}
		else if (name_list[i].compare(0,4,"Road") == 0)
		{
			Texture[i] = loadDDS("static/textures/road.dds");
		}
		else if (name_list[i].compare(0,4,"Land") == 0)
		{
			Texture[i] = loadDDS("static/textures/Land.dds");
		}
		else if (name_list[i].compare(0,4,"Fenc") == 0)
		{
			Texture[i] = loadDDS("static/textures/fence.dds");
		}
		else {
			Texture[i] = loadDDS("static/textures/stem.dds");
		}
	}


	
	// Past point
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
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
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
			//glDisableVertexAttribArray(0);
			//glDisableVertexAttribArray(1);
			//glDisableVertexAttribArray(2);
		
		}
		// draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glUseProgram(programID2);
        glm::mat4 view = glm::mat4(glm::mat3(getViewMatrix())); // remove translation from the view matrix
        glm::mat4 projection = getProjectionMatrix();
		glUniformMatrix4fv(glGetUniformLocation(programID2, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(programID2, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 3, 36);
        glBindVertexArray(3);
        glDepthFunc(GL_LESS); // set depth function back to default


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
	//glfwTerminate();
	glfw_close();

    return 0;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
