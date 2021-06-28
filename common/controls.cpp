//Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
    return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
    return ProjectionMatrix;
}

//Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
//Initial horizontal angle: toward -Z
float horizontalAngle = 3.14f;
//Initial vertical angle: none
float verticalAngle = 0.0f;
//Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f;
float mouseSpeed = 0.005f;

//Initial angle for movement of camera:
float init_angle = 0.0f;

enum dir_rotate {
    PERP_XY,
    PERP_XZ,
    PERP_YZ
};

dir_rotate current_rotation_direction = PERP_YZ;

void computeMatricesFromInputs() {
    
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    //Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, 1024/2, 768/2);
    
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle), 
        cos(verticalAngle) * cos(horizontalAngle)
    );
    
    // Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

    
    glm::vec3 up = glm::cross(direction, right);

    //Move forward
    if (glfwGetKey( window, GLFW_KEY_UP) == GLFW_PRESS) {
        initialFoV += 1.0f;
    }
    //Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		//position -= direction * deltaTime * speed;
		//verticalAngle += 0.01f;
		initialFoV -= 1.0f;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        init_angle += 0.01f;
		//position += right * deltaTime * speed;
		//horizontalAngle -= 0.01f;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        init_angle -= 0.01f;
		//position -= right * deltaTime * speed;
		//horizontalAngle += 0.01f;
	}
	
    if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS){
		current_rotation_direction = PERP_XY;
		//horizontalAngle += 0.01f;
	}
    
    if (glfwGetKey( window, GLFW_KEY_Y ) == GLFW_PRESS){
		current_rotation_direction = PERP_XZ;
		//horizontalAngle += 0.01f;
	}

    if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS){
		current_rotation_direction = PERP_YZ;
		//horizontalAngle += 0.01f;
	}

	//Initial position for camera
	glm::vec3 pos;
    
    //Choose the correct pos vector for camera.
    if (current_rotation_direction == PERP_XY) {
        pos = glm::vec3(5 * cos(init_angle), 5 * sin(init_angle), 0);
    }
    else if (current_rotation_direction == PERP_XZ) {
        pos = glm::vec3(5 * cos(init_angle), 0, 5 * sin(init_angle));
    }
    else if (current_rotation_direction == PERP_YZ) {
        pos = glm::vec3(0, 5 * cos(init_angle), 5 * sin(init_angle));
    }

	glm::vec3 pos_target = pos * (4.0f/5.0f);

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								pos,          // Camera is here
								pos_target, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
