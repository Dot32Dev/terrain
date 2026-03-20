#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Terrain {
	public:
		/// @brief from_raw
		/// Creates a terrain from a heightmap texture in the .raw file format
		/// It is assumed that the image is a square of size by size dimensions.
		/// @param file_name The name of the file to laod
		/// @param size The size of one side
		/// @return A pointer to a terrain object which can be drawn, or nullptr
		static Terrain* from_raw(string file_name, int size);

		/// @brief Set the scale of the terrain
		/// @param x The X scale
		/// @param y The Y scale
		/// @param z The Z scale
		void set_scale(float x, float y, float z);

		/// @brief Get the raw terrain heightmap data
		/// @return The raw terrain data
		const unsigned char* get_terrain_data();

		/// @brief Get the vertex data
		/// @return The vertex data
		float* get_vertex_data();

		/// @brief Get the index data
		/// @return The index data
		unsigned int* get_index_data();

		/// @brief Get index count
		/// @return index count
		int get_index_count(); 

		/// @brief Gets the count of individual items in the vertex buffer
		/// Not to be confused with vertex count, as each vertex has 5 floats
		/// @return The number of floats in the vertex buffer
		int get_vertex_component_count();
	private:
		vector<unsigned char> terrain_data;
		// Vertex attributes are three floats for position and 2 for UVs
		vector<float> vertex_buffer;
		vector<unsigned int> index_buffer;
		float scale_x;
		float scale_y;
		float scale_z;
		int size;
		// Don't use the constructor, use a static method for either image or
		// generated
		Terrain();
		void generate_buffers();
};

#endif TERRAIN_H