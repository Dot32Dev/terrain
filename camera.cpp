#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

Camera::Camera() {}

Camera::Camera(vec3 pos) :
	pos(pos),
	dir(vec2(0.0f, 0.0f))
{}

mat4 Camera::get_view_matrix() {
	mat4 matrix = mat4(1.0);
	// Rotate by the camera's dir
	vec3 pitch_axis = vec3(1.0f, 0.0f, 0.0f);
	matrix = rotate(matrix, dir.y, pitch_axis);
	vec3 yaw_axis = vec3(0.0f, 1.0f, 0.0f);
	matrix = rotate(matrix, dir.x, yaw_axis);
	// Translate by the camera's pos
	// I found that negating it made the camera position more natural, e.g a 
	// position of Y=10 should put you up in the air, not below the ground
	matrix = translate(matrix, -pos);
	return matrix;
}

void Camera::pos_input(vec3 input_vector) {
	// Rotate the input by the direction the camera is looking
	// This is so that if you walk forwards, you move forwards relative to the
	// view, rather than the cardinal direction which is forwards.
	vec3 yaw_axis = vec3(0.0f, -1.0f, 0.0f);
	mat4 rotation = rotate(mat4(1.0), dir.x, yaw_axis);
	// This is just rotation * input, except rotation is mat4 and input is vec3
	pos += vec3(rotation * vec4(input_vector, 1.0));
}

void Camera::dir_input(vec2 input_vector) {
	dir += input_vector;
}

const vec3 Camera::get_position() {
	return pos;
}

void Camera::set_position(vec3 position) {
	pos = position;
}