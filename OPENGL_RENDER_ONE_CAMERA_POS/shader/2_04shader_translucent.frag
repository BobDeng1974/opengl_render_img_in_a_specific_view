#version 440

in vec2 TexCoords;

out vec4 color;
uniform sampler2D translucenttexture;

void main()
{
	color  = texture(translucenttexture, TexCoords);
}