#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {}

Camera::Camera(glm::vec3 pos) :
	pos(pos),
	dir(glm::vec2(0.0f, 0.0f))
{}

glm::mat4 Camera::get_view_matrix() {
	glm::mat4 matrix = glm::mat4(1.0);
	// Rotate by the camera's dir
	glm::vec3 pitch_axis = glm::vec3(1.0f, 0.0f, 0.0f);
	matrix = glm::rotate(matrix, dir.y, pitch_axis);
	glm::vec3 yaw_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	matrix = glm::rotate(matrix, dir.x, yaw_axis);
	// Translate by the camera's pos
	// I found that negating it made the camera position more natural, e.g a 
	// position of Y=10 should put you up in the air, not below the ground
	matrix = glm::translate(matrix, -pos);
	return matrix;
}

void Camera::pos_input(glm::vec3 input_vector) {
	// Rotate the input by the direction the camera is looking
	// This is so that if you walk forwards, you move forwards relative to the
	// view, rather than the cardinal direction which is forwards.
	glm::vec3 yaw_axis = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), dir.x, yaw_axis);
	// This is just rotation * input, except rotation is mat4 and input is vec3
	pos += glm::vec3(rotation * glm::vec4(input_vector, 1.0));
}

void Camera::dir_input(glm::vec2 input_vector) {
	dir += input_vector;
}

const glm::vec3 Camera::get_position() {
	return pos;
}

void Camera::set_position(glm::vec3 position) {
	pos = position;
}