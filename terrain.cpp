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
	terrain->scale.x = 1.0;
	terrain->scale.y = 1.0;
	terrain->scale.z = 1.0;
	terrain->generate_buffers();
	terrain->renderer_init();
	return terrain;
}

Terrain* Terrain::from_fault_gen(int seed, int iter, float fir, int size) {
	// The temporary high-accuracy height array
	float* heights = new float[size * size];
	for (int i = 0; i < size * size; i++) {
		heights[i] = 0;
	}

	float max_height = 256;
	float min_height = 0;
	srand(seed);
	
	for (int i = 0; i < iter; i ++) {
		float jump = max_height - (max_height - min_height) * (i / (float)iter);
		// Generate two random (different!) points
		int x1 = rand()%size;
		int z1 = rand()%size;
		int x2;
		int z2;
		do {
			x2 = rand()%size;
			z2 = rand()%size;
		} while (x2 == x1 && z2 == z1);

		for (int z = 0; z < size; z++) {
			for (int x = 0; x < size; x++) {
				// 2D cross product, aka dot product with perpendicular vector
				int y = (x2 - x1) * (z - z1) - (z2 - z1) * (x - x1);
				if (y > 0) {
					heights[z * size + x] += jump;
					// heights[z * size + x] += 10.0;
				}
			}

		}
	}

	Terrain* terrain = new Terrain();
	terrain->terrain_data = vector<unsigned char>();
	// Micro-optimisation that avoids reallocating the memory all the time 
	terrain->terrain_data.resize(size * size);
	for (int i = 0; i < size * size; i++) {
		terrain->terrain_data[i] = heights[i];
	}

	delete heights;

	terrain->size = size;
	terrain->scale.x = 1.0;
	terrain->scale.y = 1.0;
	terrain->scale.z = 1.0;
	terrain->generate_buffers();
	terrain->renderer_init();
	return terrain;
}

void Terrain::set_scale(vec3 scale) {
	this->scale = scale;
	this->generate_buffers();
	this->update_vao();
}

void Terrain::generate_buffers() {
	vertex_buffer.clear();
	index_buffer.clear();

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			int y = terrain_data[z * size + x];
			vertex_buffer.push_back((x - size / 2) * scale.x);
			vertex_buffer.push_back(y * scale.y);
			vertex_buffer.push_back((z - size / 2) * scale.z);
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

float Terrain::get_height_at_location(vec3 location) {
	vec3 local_pos = location / scale + (float)size / 2.0f;
	if (local_pos.x < 0) local_pos.x = 0;
	if (local_pos.x > size - 2) local_pos.x = size - 2;
	if (local_pos.z < 0) local_pos.z = 0;
	if (local_pos.z > size - 2) local_pos.z = size - 2;
	float x = local_pos.x;
	float z = local_pos.z;
	int floor_x = x;
	int floor_z = z;
	int ceil_x = x + 1.0;
	int ceil_z = z + 1.0;
	float x_step = x - floor_x;
	float z_step = z - floor_z;
	float top_left = terrain_data[floor_z * size + floor_x] * scale.y;
	float top_right = terrain_data[floor_z * size + ceil_x] * scale.y;
	float bottom_left = terrain_data[ceil_z * size + floor_x] * scale.y;
	float bottom_right = terrain_data[ceil_z * size + ceil_x] * scale.y;
	// Linear interpolation between x at both z
	float top = top_left + (top_right - top_left) * x_step;
	float bottom = bottom_left + (bottom_right - bottom_left) * x_step;
	// Linear interpolation between z at the found x
	float height = top + (bottom - top) * z_step;
	return height;
}