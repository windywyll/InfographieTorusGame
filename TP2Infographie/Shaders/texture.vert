#version 130
// OpenGL 3.2 replace above with #version 150

uniform mat4x4 projection;
uniform mat4x4 modelview;

in  vec3 in_Position;
in	vec2 in_TexCoord;

out vec2 ex_TexCoord;

void main(void) {

    // vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
	gl_Position = projection * vertexPosition;
	ex_TexCoord = in_TexCoord;
}
