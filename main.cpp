#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "terrain.h"

const int WIN_W = 800;
const int WIN_H = 600;

void resize(GLFWwindow* window, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Terrain", NULL, NULL);
	if (window == NULL) return -1;
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -2;

	resize(window, WIN_W, WIN_H);
	glfwSetFramebufferSizeCallback(window, resize);

	Terrain* terrain = Terrain::from_raw("height128.raw", 128);
	if (!terrain) return -3;

	glm::mat4 transform = glm::mat4(1.0);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}