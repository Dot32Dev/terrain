#include "terrain.h"
#include <fstream>

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
	terrain->set_scale(1.0, 1.0, 1.0);
	return terrain;
}

void Terrain::set_scale(float x, float y, float z) {
	this->scale_x = x;
	this->scale_y = y;
	this->scale_z = z;
	this->generate_buffers();
}

const unsigned char* Terrain::get_terrain_data() {
	return this->terrain_data.data();
}

void Terrain::generate_buffers() {
	vertex_buffer.clear();
	index_buffer.clear();

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int z = terrain_data[y * size + x];
			vertex_buffer.push_back((x - size / 2) * scale_x);
			vertex_buffer.push_back((y - size / 2) * scale_y);
			vertex_buffer.push_back(z * scale_z);
			vertex_buffer.push_back(x / (float)size);
			vertex_buffer.push_back(y / (float)size);
		}
	}

	for (int y = 0; y < size-1; y++) {
		for (int x = 0; x < size-1; x++) {
			// Triangle one
			index_buffer.push_back(y * size + x); // Top left of tile
			index_buffer.push_back(y * size + x + 1); // Top right of tile
			index_buffer.push_back((y + 1) * size + x); // Bottom left of tile

			// Triangle two
			index_buffer.push_back(y * size + x + 1); // Top right of tile
			index_buffer.push_back((y + 1) * size + x + 1); // Bottom right
			index_buffer.push_back((y + 1) * size + x); // Bottom left of tile
		}
	}
}

float* Terrain::get_vertex_data() {
	return this->vertex_buffer.data();
}

unsigned int* Terrain::get_index_data() {
	return this->index_buffer.data();
}

int Terrain::get_index_count() {
	return this->index_buffer.size();
}

int Terrain::get_vertex_component_count() {
	return this->vertex_buffer.size();
}