#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "terrain.h"

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

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

	Terrain* terrain = Terrain::from_raw("res/height128.raw", 128);
	if (!terrain) return -3;

	glm::mat4 transform = glm::mat4(1.0);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO); 

	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Load shaders from file
	ifstream vs_file("res/vert.glsl");
	stringstream vs_buffer;
	vs_buffer << vs_file.rdbuf();
	string vert_shader_string = vs_buffer.str();
	ifstream fs_file = ifstream("res/frag.glsl");
	stringstream fs_buffer;
	fs_buffer << fs_file.rdbuf();
	string frag_shader_string = fs_buffer.str();
	const char* vert_shader_chars = vert_shader_string.c_str();
	const char* frag_shader_chars = frag_shader_string.c_str();

	// Compile the vertex fella
	unsigned int vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_id, 1, &vert_shader_chars, NULL);
	glCompileShader(vert_shader_id);
	int  success;
	char info_log[512];
	glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vert_shader_id, 512, NULL, info_log);
		cout << "ERROR::SHADER::VERT::COMPILATION_FAILED\n" << info_log << endl;
	}

	// Compile the fragment fella
	unsigned int frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_id, 1, &frag_shader_chars, NULL);
	glCompileShader(frag_shader_id);
	glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag_shader_id, 512, NULL, info_log);
		cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << info_log << endl;
	}

	unsigned int shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, vert_shader_id);
	glAttachShader(shader_program, frag_shader_id); 
	glLinkProgram(shader_program);
	glDeleteShader(vert_shader_id);
	glDeleteShader(frag_shader_id); 
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cout << "ERROR::SHADER_PROGRAM_FAILED\n" << info_log << endl;
	}

	glUseProgram(shader_program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}