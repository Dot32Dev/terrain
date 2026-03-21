#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

class Camera {
	public:
		/// @brief Spawns a camera at the location provided
		/// Will face the direction of the identity matrix.
		/// This class just manages a view matrix, use get_view_matrix() and 
		/// send it to your shader's view uniform.
		/// @param pos 
		Camera(vec3 pos);

		/// @brief Returns the cameras view matrix to be sent to the shader
		/// @return The matrix to be sent to the shader
		mat4 get_view_matrix();

		/// @brief Input a movement vector to the camera
		/// The camera will move by adding this vector to its current location,
		/// but accounting for the camera's current look direction.
		/// If you give an input vector where z = 1, the camera will move 
		/// forward one unit in the direction you are looking. However, the 
		/// camera's vertical movement does not take into account the look dir
		/// as I prefer for up to always be up, and moving horizontally to 
		/// always be horizontal.
		/// @param input_vector The input movement vector
		void pos_input(vec3 input_vector);

		/// @brief Input direction vector to the camera
		/// Should represent the current input that the user is taking. Will
		/// update the cameras direction by the value provided.
		/// @param input_vector The input direction vector
		void dir_input(vec2 input_vector);

		/// @brief Get the current position of the camera
		/// @return The vec3 position of the camera
		const vec3 get_position(); 

		/// @brief Sets the position of the camera
		/// @param position The position to move the camera to
		void set_position(vec3 position);
	private:
		Camera();
		vec3 pos;
		vec2 dir;
};

#endif