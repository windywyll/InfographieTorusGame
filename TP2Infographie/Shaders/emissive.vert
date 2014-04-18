#version 130
// OpenGL 3.2 replace above with #version 150

uniform mat4 modelview;
uniform mat4 projection;

in vec2 in_TexCoord;
out vec2 ex_TexCoord;

in  vec3 in_Position;

void main(void) {
   // vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);

	ex_TexCoord = in_TexCoord;

    gl_Position = projection * vertexPosition;
}
