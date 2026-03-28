#include "texture.h"

#include <GLAD/glad.h>
#include <fstream>

using std::ifstream;
using std::ios;

Texture::Texture() {}

Texture Texture::from_data(
		vector<unsigned char> data, 
		int width, 
		int height, 
		int channels
) {
	Texture texture = Texture(); 
	// Texture setup
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &texture.texture_id);
	glBindTexture(GL_TEXTURE_2D, texture.texture_id);

	GLenum format = GL_RGB; // Default RGB if too many channels supplied
	if (channels == 4) format = GL_RGBA; 
	if (channels == 2) format = GL_RG; 
	if (channels == 1) format = GL_RED; 

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		format, 
		width, 
		height, 
		0, 
		format, 
		GL_UNSIGNED_BYTE, 
		data.data()
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

Texture Texture::from_raw(
	string filename, 
	int width, 
	int height, 
	int channels
) {
	ifstream infile(filename, ios::binary);
	vector<unsigned char> data = vector<unsigned char>();

	// Get length of file
	infile.seekg(0, ios::end);
	int length = infile.tellg();

	// Read data as block
	infile.seekg(0, ios::beg);
	data.resize(length);
	infile.read(reinterpret_cast<char*>(data.data()), length);
	infile.close();

	return Texture::from_data(data, width, height, channels);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, texture_id);
}