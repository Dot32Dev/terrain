#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Texture {
public:
	/// @brief Creates and returns a Texture object from pure data.
	/// @param data The data to create an image from
	/// @param width The width for the texture to be
	/// @param height The height for the texture to be
	/// @param channels The number of channels the texture will have
	/// There must be at least width*height*channels unsigned chars in the data
	/// @return A Texture object which can be bound
	static Texture* from_data(
		vector<unsigned char> data, 
		int width, 
		int height, 
		int channels
	);

	/// @brief Creates and returns a Texture object from a RAW file
	/// @param filename The filename of the RAW file
	/// @param width The width for the texture to be
	/// @param height The height for the texture to be
	/// @param channels The number of channels the texture will have
	/// The file must be larger than width*height*channels
	/// @return A Texture object which can be bound
	static Texture* from_raw(
		string filename, 
		int width, 
		int height, 
		int channels
	);

	/// @brief Binds the texture so that it can be used in the shader
	void bind();
private:
	Texture();
	unsigned int texture_id;
};

#endif // TEXTURE_H