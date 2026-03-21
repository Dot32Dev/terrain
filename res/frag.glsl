#version 330 core
out vec4 frag_colour;

in vec2 uv;

uniform sampler2D texture_in;

void main() {
    // frag_colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	float val = texture(texture_in, uv).x;
    frag_colour = vec4(uv.x, uv.y, 0.2f, 1.0f) * val;
	// frag_colour = vec4(val, val, val, 1.0);
}