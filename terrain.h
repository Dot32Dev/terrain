#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

using std::string;
using std::vector;
using glm::vec3;

class Terrain {
	public:
		/// @brief from_raw
		/// Creates a terrain from a heightmap texture in the .raw file format
		/// It is assumed that the image is a square of size by size dimensions.
		/// @param file_name The name of the file to laod
		/// @param size The size of one side
		/// @return A pointer to a terrain object which can be drawn, or nullptr
		static Terrain* from_raw(string file_name, int size);

		/// @brief Procedurally generates a heightmap
		/// @param seed The random seed to use
		/// @param iter The number of iterations
		/// @param fir The FIR filter parameter
		/// @param size The size of the terrain
		/// @return A pointer to a terrain object which can be drawn, or nullptr
		static Terrain* from_fault_gen(int seed, int iter, float fir, int size);

		/// @brief Draws the terrain
		void draw();

		/// @brief Set the scale of the terrain
		/// @param scale The scale to draw the terrain
		void set_scale(vec3 scale);

		/// @brief Get the height of the terrain at a given location
		/// Accounts for scaling of terrain in the world.
		/// If outside of the borders of the world, the height returned will be
		/// clamped to the edges.
		/// This function does nothing with the Y coordinate passed in, and
		/// returns what the Y coordinate would be if the player was on the 
		/// ground.
		/// @param location 
		/// @return The height of the terrain at the given position
		float get_height_at_location(vec3 location);
	private:
		vector<unsigned char> terrain_data;
		// Vertex attributes are three floats for position and 2 for UVs
		vector<float> vertex_buffer;
		vector<unsigned int> index_buffer;
		vec3 scale;
		int size;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		unsigned int texture;
		// Don't use the constructor, use a static method
		Terrain();
		void generate_buffers();
		void renderer_init();
		void update_vao();
};

#endif TERRAIN_H