#version 450
in vec4 vertexColor;
in vec2 texCoord;
out vec4 color;
uniform sampler2D tex_buffer;
void main() {
	color = texture(tex_buffer, texCoord);
}