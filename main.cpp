#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include "terrain.h"
#include "shader.h"

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

const int WIN_W = 800;
const int WIN_H = 800;

bool line_mode = false;

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

	resize(window, WIN_W, WIN_H);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, key);

	Terrain* terrain = Terrain::from_raw("res/height128.raw", 128);
	if (!terrain) return -4;

	glm::mat4 transform = glm::mat4(1.0);

	// float vertices[] = {
	// 	0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  // top right
	// 	0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   // bottom right
	// 	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // bottom left
	// 	-0.5f,  0.5f, 0.0f,  0.0f,  0.0f // top left
	// };
	// unsigned int indices[] = {  // note that we start from 0!
	// 	0, 1, 3,   // first triangle
	// 	1, 2, 3    // second triangle
	// }; 

	terrain->set_scale(2.0/128, 2.0/128, 0.5/128/2);
	// float* vertices = terrain->get_vertex_data();
	// unsigned int* indices = terrain->get_index_data();
	// int index_count = terrain->get_index_count();

	// unsigned int VAO;
	// glGenVertexArrays(1, &VAO); 

	// glBindVertexArray(VAO);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, indices, GL_STATIC_DRAW); 

	// unsigned int VBO;
	// glGenBuffers(1, &VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * terrain->get_vertex_component_count(), vertices, GL_STATIC_DRAW);

	// // Load shaders from file
	// ifstream vs_file("res/vert.glsl");
	// stringstream vs_buffer;
	// vs_buffer << vs_file.rdbuf();
	// string vert_shader_string = vs_buffer.str();
	// ifstream fs_file = ifstream("res/frag.glsl");
	// stringstream fs_buffer;
	// fs_buffer << fs_file.rdbuf();
	// string frag_shader_string = fs_buffer.str();
	// const char* vert_shader_chars = vert_shader_string.c_str();
	// const char* frag_shader_chars = frag_shader_string.c_str();

	// // Compile the vertex fella
	// unsigned int vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vert_shader_id, 1, &vert_shader_chars, NULL);
	// glCompileShader(vert_shader_id);
	// int  success;
	// char info_log[512];
	// glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
	// if(!success) {
	// 	glGetShaderInfoLog(vert_shader_id, 512, NULL, info_log);
	// 	cout << "ERROR::SHADER::VERT::COMPILATION_FAILED\n" << info_log << endl;
	// }

	// // Compile the fragment fella
	// unsigned int frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(frag_shader_id, 1, &frag_shader_chars, NULL);
	// glCompileShader(frag_shader_id);
	// glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
	// if(!success) {
	// 	glGetShaderInfoLog(frag_shader_id, 512, NULL, info_log);
	// 	cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << info_log << endl;
	// }

	// unsigned int shader_program;
	// shader_program = glCreateProgram();

	// glAttachShader(shader_program, vert_shader_id);
	// glAttachShader(shader_program, frag_shader_id); 
	// glLinkProgram(shader_program);
	// glDeleteShader(vert_shader_id);
	// glDeleteShader(frag_shader_id); 
	// glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	// if(!success) {
    // 	glGetProgramInfoLog(shader_program, 512, NULL, info_log);
	// 	cout << "ERROR::SHADER_PROGRAM_FAILED\n" << info_log << endl;
	// }

	// glUseProgram(shader_program);

	Shader shader("res/vert.glsl", "res/frag.glsl");
	shader.use(); 
	Uniform model_transform = shader.get_uniform("model");
	model_transform.send(transform);


	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
	// glEnableVertexAttribArray(1);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unsigned int texture;
	// glGenTextures(1, &texture);
	// glBindTexture(GL_TEXTURE_2D, texture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 128, 0, GL_RED, GL_UNSIGNED_BYTE, terrain->get_terrain_data());
	// glGenerateMipmap(GL_TEXTURE_2D);

	// glBindTexture(GL_TEXTURE_2D, texture);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glBindVertexArray(VAO);
		// glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
		terrain->draw();

		transform = glm::rotate(transform, glm::radians(-0.2f), glm::vec3(1.0f, 0.0f, 0.0f));
		model_transform.send(transform);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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