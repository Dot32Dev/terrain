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
	terrain->scale_x = 1.0;
	terrain->scale_y = 1.0;
	terrain->scale_z = 1.0;
	return terrain;
}

void Terrain::set_scale(float x, float y, float z) {
	this->scale_x = x;
	this->scale_y = y;
	this->scale_z = z;
}

const unsigned char* Terrain::get_terrain_data() {
	return this->terrain_data.data();
}