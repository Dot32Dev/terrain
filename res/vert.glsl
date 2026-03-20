#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv_in;

out vec2 uv;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(pos, 1.0);
	uv = uv_in;
}