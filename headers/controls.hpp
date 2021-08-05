#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::mat4 computeViewMatrix(glm::vec3, glm::vec3, glm::vec3);
glm::mat4 computeProjectionMatrix(float, float, float, float);

#endif
