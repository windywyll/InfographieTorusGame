#version 130
// OpenGL 3.2 replace above with #version 150
// Some drivers require the following
precision highp float;

in vec2 ex_TexCoord;

uniform sampler2D textureUnit0;

// GLSL versions after 1.3 remove the built in type gl_FragColor
// If using a shader lang version greater than #version 130
// you *may* need to uncomment the following line:
// out vec4 gl_FragColor
 
void main(void) {
	gl_FragColor = texture2D(textureUnit0, ex_TexCoord);
}