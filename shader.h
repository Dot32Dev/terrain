#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>

using std::ifstream;
using std::stringstream;
using std::string;
using std::map;

class Uniform {
	public:
		/// @brief Send a mat4 to a uniform of a shader
		/// Assumes the relevant shader is currently bound
		/// @param matrix The matrix to send to the shader
		void send(const glm::mat4& matrix);
	private:
		friend class Shader; // So that only the shader may construct Uniforms
		Uniform(unsigned int program_id, const string& uniform, Shader* shader);
		Uniform();
		int uniform_location;
};

class Shader {
	public:
		/// @brief Create a new shader from shader files
		/// @param vertex_path The file path to the vertex shader
		/// @param fragment_path The file path to the fragment shader
		Shader(const string& vertex_path, const string& fragment_path);

		/// @brief Enable the shader
		void use();

		/// @brief The name of the uniform to get the location of
		/// Returns a Uniform object which can be used to send values to the 
		/// shader. Never use a uniform when its related shader is not in use.
		/// Uniform colour = shader.get_uniform("colour");
		/// colour.send(1.0, 0.5, 0.1, 1.0); // Assumes colour is a vec4
		/// @param uniform The string variable name of the uniform
		/// @return a Uniform object which can be used to send values to shader
		Uniform get_uniform(const string& uniform);
	private:
		// Creating a shader with no shader files is not allowed
		Shader();
		unsigned int program_id;
};

#endif // SHADER_H