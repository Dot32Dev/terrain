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

		void set_scale(float x, float y, float z);
	private:
		vector<unsigned char> terrain_data;
		float scale_x;
		float scale_y;
		float scale_z;
		int size;
		// Don't use the constructor, use a static method for either image or
		// generated
		Terrain();
};

#endif TERRAIN_H