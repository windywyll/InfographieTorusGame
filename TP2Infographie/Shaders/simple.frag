#version 130
// OpenGL 3.2 replace above with #version 150
// Some drivers require the following
precision highp float;


struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform lightStruct light;
uniform materialStruct material;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;

in float ex_att;

uniform vec4 colour;

layout(location = 0) out vec4 out_Color;

// GLSL versions after 1.3 remove the built in type gl_FragColor
// If using a shader lang version greater than #version 130
// you *may* need to uncomment the following line:
// out vec4 gl_FragColor
 
void main(void) {

	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));

	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	// Fragment colour
	vec4 tmpColour = colour;
	float transparency = tmpColour.a;
	tmpColour = (ambientI + (diffuseI + specularI)* ex_att) *tmpColour;
	tmpColour.a = transparency;
	out_Color = tmpColour;
	gl_FragColor = out_Color;
}