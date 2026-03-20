#include "terrain.h"
#include <fstream>
#include <glad/glad.h>

using std::ifstream;
using std::ios;

Terrain::Terrain() {}

Terrain* Terrain::from_raw(string file_name, int size) {
	ifstream infile(file_name, ios::binary);
	if (!infile) return nullptr;

	// Create terrain object to be returned
	Terrain* terrain = new Terrain();

	// Using STD vector so that I don't have to think about the rule of 3 or 5
	terrain->terrain_data = vector<unsigned char>();

	// Get length of file
	infile.seekg(0, ios::end);
	int length = infile.tellg();

	// Read data as block
	infile.seekg(0, ios::beg);
	terrain->terrain_data.resize(length);
	infile.read(reinterpret_cast<char*>(terrain->terrain_data.data()), length);
	infile.close();
	terrain->size = size;
	terrain->scale_x = 1.0;
	terrain->scale_y = 1.0;
	terrain->scale_z = 1.0;
	terrain->generate_buffers();
	terrain->renderer_init();
	return terrain;
}

void Terrain::set_scale(float x, float y, float z) {
	this->scale_x = x;
	this->scale_y = y;
	this->scale_z = z;
	this->generate_buffers();
	this->update_vao();
}

void Terrain::generate_buffers() {
	vertex_buffer.clear();
	index_buffer.clear();

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			int y = terrain_data[z * size + x];
			vertex_buffer.push_back((x - size / 2) * scale_x);
			vertex_buffer.push_back(y * scale_y);
			vertex_buffer.push_back((z - size / 2) * scale_z);
			vertex_buffer.push_back(x / (float)size);
			vertex_buffer.push_back(z / (float)size);
		}
	}

	for (int z = 0; z < size-1; z++) {
		for (int x = 0; x < size-1; x++) {
			// Triangle one
			index_buffer.push_back(z * size + x); // Top left of tile
			index_buffer.push_back(z * size + x + 1); // Top right of tile
			index_buffer.push_back((z + 1) * size + x); // Bottom left of tile

			// Triangle two
			index_buffer.push_back(z * size + x + 1); // Top right of tile
			index_buffer.push_back((z + 1) * size + x + 1); // Bottom right
			index_buffer.push_back((z + 1) * size + x); // Bottom left of tile
		}
	}
}

int Terrain::get_vertex_component_count() {
	return this->vertex_buffer.size();
}

void Terrain::renderer_init() {
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO);

	// Index buffer (EBO)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int) * index_buffer.size(), 
		index_buffer.data(), 
		GL_STATIC_DRAW
	); 

	// Vertex buffer (VBO)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * vertex_buffer.size(), 
		vertex_buffer.data(), 
		GL_STATIC_DRAW
	);

	// Vertex layout
	// Position
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		5 * sizeof(float), 
		(void*)0
	);
	glEnableVertexAttribArray(0);
	// Texture coordinates
	glVertexAttribPointer(
		1, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		5 * sizeof(float), 
		(void*)(3 * sizeof(float))
	);
	glEnableVertexAttribArray(1);

	// Texture setup
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RED, 
		128, 
		128, 
		0, 
		GL_RED, 
		GL_UNSIGNED_BYTE, 
		terrain_data.data()
	);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Terrain::draw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, index_buffer.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::update_vao() {
	glBindVertexArray(VAO);

	// Index buffer (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int) * index_buffer.size(), 
		index_buffer.data(), 
		GL_STATIC_DRAW
	); 

	// Vertex buffer (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * vertex_buffer.size(), 
		vertex_buffer.data(), 
		GL_STATIC_DRAW
	);
}