#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

using std::cerr;
using std::endl;

Shader::Shader(const string& vertex_path, const string& fragment_path) {
	unsigned int vert_shader_id;
	// Load from file
	ifstream vs_file(vertex_path);
	stringstream vs_buffer;
	vs_buffer << vs_file.rdbuf();
	string vs_string = vs_buffer.str();
	const char* vs_chars = vs_string.c_str();

	// Compile the fella
	vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_id, 1, &vs_chars, NULL);
	glCompileShader(vert_shader_id);

	// Check for errors
	int vs_success;
	glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &vs_success);
	if (!vs_success) {
		char err[1024]; glGetShaderInfoLog(vert_shader_id, 1024, NULL, err);
		cerr << "Vertex shader error:\n" << err << endl;
	}

	unsigned int frag_shader_id;
	// Load from file
	ifstream fs_file(fragment_path);
	stringstream fs_buffer;
	fs_buffer << fs_file.rdbuf();
	string fs_shader_string = fs_buffer.str();
	const char* fs_shader_chars = fs_shader_string.c_str();

	// Compile the fella
	frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_id, 1, &fs_shader_chars, NULL);
	glCompileShader(frag_shader_id);

	// Check for errors
	int fs_success;
	glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &fs_success);
	if (!fs_success) {
		char err[1024]; 
		glGetShaderInfoLog(frag_shader_id, 1024, NULL, err);
		cerr << "Fragment shader error:\n" << err << endl;
	}

	// Compile shader program
	program_id = glCreateProgram();
	glAttachShader(program_id, vert_shader_id);
	glAttachShader(program_id, frag_shader_id);
	glLinkProgram(program_id);

	// Check for errors
	int program_success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &program_success);
    if (!program_success) {
        char err[1024]; 
		glGetProgramInfoLog(program_id, 1024, NULL, err);
        cerr << "Shader program link error:\n" << err << endl;
    }
}

Shader::Shader() {}

void Shader::use() {
	glUseProgram(program_id);
}

Uniform Shader::get_uniform(const string& uniform) {
	return Uniform(program_id, uniform, this);
}

Uniform::Uniform(
	unsigned int program_id, 
	const string& uniform, 
	Shader* shader
) : 
	uniform_location(glGetUniformLocation(program_id, uniform.c_str()))
{}

void Uniform::send(const mat4& matrix) {
	// location, number of matrices, transposing, matrix
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, value_ptr(matrix));
}