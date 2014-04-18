#version 330

uniform mat4 modelview;
uniform mat4 projection;

uniform vec4 lightPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

in  vec3 in_Position;
in  vec3 in_Normal;

out vec3 ex_N;
out vec3 ex_V;
out vec3 ex_L;
//out float ex_att;

void main(void) {
   // vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);

	// Find V - in eye coordinates, eye is at (0,0,0)
	ex_V = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	//mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	mat3 normalmatrix = mat3(modelview);
	ex_N = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	ex_L = normalize(lightPosition.xyz - vertexPosition.xyz);

	//float distance = length(lightPosition.xyz - vertexPosition.xyz);

	//ex_att = 1/(constantAttenuation + linearAttenuation*distance + quadraticAttenuation*distance*distance);

    gl_Position = projection * vertexPosition;
}
