#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "../headers/transformations.hpp"

//Rotation along x_axis
glm::mat4 rotation_matrix_x(float angle)
{
    glm::mat4 result(1.0f); //Identity 4x4 matrix.
    float c = cos(angle);
    float s = sin(angle);
    
    result[1][1] = c;
    result[1][2] = -s;
    result[2][1] = c;
    result[2][1] = s;

    return result;
}

//Rotation along y_axis
glm::mat4 rotation_matrix_y(float angle)
{
    glm::mat4 result(1.0f); //Identity 4x4 matrix.
    float c = cos(angle);
    float s = sin(angle);

    result[0][0] = c;
    result[0][2] = s;
    result[2][0] = -s;
    result[2][2] = c;

    return result;
}

//Rotation along z_axis
glm::mat4 rotation_matrix_z(float angle)
{
    glm::mat4 result(1.0f); //Identity 4x4 matrix.
    float c = cos(angle);
    float s = sin(angle);

    result[0][0] = c;
    result[0][1] = -s;
    result[1][0] = s;
    result[1][1] = c;

    return result;
}

//Translation with the given translate values.
glm::mat4 translate_matrix(float t_x, float t_y, float t_z)
{
    glm::mat4 result(1.0f); //Identity 4x4 matrix.

    result[0][3] = t_x;
    result[1][3] = t_y;
    result[2][3] = t_z;

    return result;
}

//Scaling matrix to scale along x,y and z axes.
glm::mat4 scaling_matrix(float s_x, float s_y, float s_z)
{
    glm::mat4 result(1.0f); //Identity 4x4 matrix.

    result[0][0] = s_x;
    result[1][1] = s_y;
    result[2][2] = s_z;

    return result;
}
