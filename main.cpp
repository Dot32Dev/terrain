#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "terrain.h"
#include "shader.h"
#include "camera.h"

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using glm::vec2;
using glm::vec3;
using glm::mat4;

const int WIN_W = 1280;
const int WIN_H = 720;

bool line_mode = false;
Uniform* projection_uniform_pointer;

void resize(GLFWwindow* window, int width, int height);
void key(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Terrain", NULL, NULL);
	if (window == NULL) return -2;
	
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -3;

	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, key);

	// Terrain* terrain = Terrain::from_raw("res/height128.raw", 128);
	Terrain* terrain = Terrain::from_fault_gen(32, 256, 0.01, 128);
	if (!terrain) return -4;

	terrain->set_scale(vec3(5.0f, 0.5f, 5.0f));

	Shader shader("res/vert.glsl", "res/frag.glsl");
	shader.use(); 

	mat4 model = mat4(1.0);
	Uniform model_uniform = shader.get_uniform("model");
	model_uniform.send(model);

	// Camera
	Camera camera(vec3(0.0f, 100.0f, 300.0f));
	Uniform view_uniform = shader.get_uniform("view");
	view_uniform.send(camera.get_view_matrix());

	mat4 projection = mat4(1.0);
	projection = glm::perspective(
		glm::radians(90.0f), 
		(float)WIN_W/WIN_H, 
		0.1f, 
		100.0f
	);

	Uniform projection_uniform = shader.get_uniform("projection");
	projection_uniform_pointer = &projection_uniform;
	projection_uniform.send(projection);

	resize(window, WIN_W, WIN_H);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	double delta_time = 0.0;
	double last_frame = 0.0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Delta time
		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Movement
		float movement_speed = 15 * delta_time;
		float rot_speed = 1.5 * delta_time;

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			movement_speed *= 2.0;
		}

		vec3 pos_input = vec3(0.0f, 0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pos_input += vec3(0.0f, 0.0f, -movement_speed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pos_input += vec3(0.0f, 0.0f, movement_speed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pos_input += vec3(-movement_speed, 0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pos_input += vec3(movement_speed, 0.0f, 0.0f);

		vec2 dir_input = vec2(0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			dir_input += vec2(0.0f, -rot_speed);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			dir_input += vec2(0.0f, rot_speed);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			dir_input += vec2(-rot_speed, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			dir_input += vec2(rot_speed, 0.0f);

		// Send input to camera
		camera.pos_input(pos_input);
		camera.dir_input(dir_input);
		// Place camera on ground
		vec3 cam_pos = camera.get_position();
		float height = terrain->get_height_at_location(cam_pos);
		cam_pos.y = height + 1.8; // Height of an average person is 1.8m ish
		camera.set_position(cam_pos);
		// Draw
		view_uniform.send(camera.get_view_matrix());
		terrain->draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	mat4 projection = mat4(1.0);
	projection = glm::perspective(
		glm::radians(90.0f), 
		(float)width/height, 
		0.2f, 
		1000.0f
	);
	projection_uniform_pointer->send(projection);
}

void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		line_mode = !line_mode;

		if (line_mode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}