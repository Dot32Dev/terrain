#version 330 core
out vec4 frag_colour;

in vec2 uv;

uniform sampler2D texture_in;

void main() {
	vec3 val = texture(texture_in, uv).rgb;
	// frag_colour = vec4(val, 1.0);
	frag_colour = vec4(val, 1.0);
}