#version 130
// OpenGL 3.2 replace above with #version 150
// Some drivers require the following
precision highp float;

uniform float emissive;
uniform sampler2D textureUnit0;

in vec2 ex_TexCoord;

layout(location = 0) out vec4 out_Color;

// GLSL versions after 1.3 remove the built in type gl_FragColor
// If using a shader lang version greater than #version 130
// you *may* need to uncomment the following line:
// out vec4 gl_FragColor
 
void main(void) {
	vec4 colour = texture(textureUnit0, ex_TexCoord);
	float transparency = colour.a;
	colour = colour* emissive;
	colour.a = transparency;
	out_Color = colour;
}