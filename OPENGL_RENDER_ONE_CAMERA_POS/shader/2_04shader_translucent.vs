#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

void main()
{
    gl_Position = projection * view * model * vec4(position/scale, 1.0f);
	TexCoords = vec2(texCoords.x, 1.0 - texCoords.y);
}