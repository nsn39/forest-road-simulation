// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../headers/controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

//Computes view matrix from given camera, camera_target and up directions.
glm::mat4 computeViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 temp) 
{
	glm::mat4 view_matrix;
	glm::vec3 forward = glm::normalize(from - to);
	glm::vec3 right = glm::cross(temp, forward);
	glm::vec3 up = glm::cross(forward, right);

	view_matrix[0][0] = right.x;
	view_matrix[1][0] = right.y;
	view_matrix[2][0] = right.z;

	view_matrix[0][1] = up.x;
	view_matrix[1][1] = up.y;
	view_matrix[2][1] = up.z;

	view_matrix[0][2] = forward.x;
	view_matrix[1][2] = forward.y;
	view_matrix[2][2] = forward.z;

	view_matrix[3][0] = -glm::dot(right, from);
	view_matrix[3][1] = -glm::dot(up, from);
	view_matrix[3][2] = -glm::dot(forward, from);

	view_matrix[0][3] = 0;
	view_matrix[1][3] = 0;
	view_matrix[2][3] = 0;
	view_matrix[3][3] = 1;

	return view_matrix;
}

//Computes Perspective Projection Matrix from given Field of View angle, aspect_ratio and clipping planes.
glm::mat4 computeProjectionMatrix(float fov_radians, float aspect_ratio, float z_near, float z_far)
{	
	//Right-handed coordinate system.
	//Creates Perspective projection matrix.
	const float tanHalfFoV = tan(fov_radians / 2.0f);
	
	glm::mat4 result(1.0f);
	result[0][0] = 1.0f / (aspect_ratio * tanHalfFoV);
	result[1][1] = 1.0f / (tanHalfFoV);
	result[2][2] = z_far / (z_near - z_far);
	result[2][3] = -1.0f;
	result[3][2] = - (z_far * z_near) / (z_far - z_near);
	return result;
}


//Initial position for camera
glm::vec3 pos = glm::vec3(
	20,
	7,
	0
);

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

//Initial angle;
float init_angle = 0.0f;


void computeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	//horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	//verticalAngle   += mouseSpeed * float( 768/2 - ypos );
	//horizontalAngle += 0.1f;


	// Direction : Spherical coordinates to Cartesian coordinates conversion
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
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		//position += direction * deltaTime * speed;
		//verticalAngle -= 0.01f;
		pos += glm::vec3(0.2,0,0);
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		//position -= direction * deltaTime * speed;
		//verticalAngle += 0.01f;
		pos -= glm::vec3(0.2,0,0);
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
		pos += glm::vec3(0,0,0.2);
		
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
		//horizontalAngle += 0.01f;
		pos -= glm::vec3(0,0,0.2);
	}
	

	glm::vec3 pos_target = pos + glm::vec3(0, 0, 0.5);

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = computeProjectionMatrix(FoV, 4.0f / 3.0f, 0.1f, 50.0f);
	// Camera matrix
	ViewMatrix       = computeViewMatrix(
								pos,          // Camera is here
								pos_target, // and looks here : at the same position, plus "direction"
								up          // Head is up (set to 0,1,0)
						   );



	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
