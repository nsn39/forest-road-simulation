#ifndef CONTROLS_HPP
#define CONTROLS_HPP

glm::mat4 rotation_matrix_x(float angle);
glm::mat4 rotation_matrix_y(float angle);
glm::mat4 rotation_matrix_z(float angle);
glm::mat4 translate_matrix(float, float, float);
glm::mat4 scaling_matrix(float, float, float);


#endif