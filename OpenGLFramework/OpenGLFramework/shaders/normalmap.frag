// a normal map fragment shader
#version 410
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;
out vec4 FragColour;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; // material ambient
uniform vec3 Kd; // material diffuse
uniform vec3 Ks; // material specular
uniform float specularPower;
uniform vec3 Ia; // light ambient
uniform vec3 Id[2]; // light diffuse
uniform vec3 Is[2]; // light specular
uniform vec3 lightPosition[2];
uniform vec3 cameraPosition;
void main() {

vec3 N = normalize(vNormal);
vec3 T = normalize(vTangent);
vec3 B = normalize(vBiTangent);

vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;


vec3 totalLight = vec3(0);
//loop through lights
for(int i = 0; i < 2; ++i)
{

	vec3 L = normalize(lightPosition[i] - vPosition.xyz);

	mat3 TBN = mat3(T,B,N);

	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

	N = TBN * (texNormal * 2 - 1);

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ) );

	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

	// calculate each light property
	vec3 ambient = Ia * Ka * texDiffuse;
	vec3 diffuse = Id[i] * Kd * texDiffuse * lambertTerm;
	vec3 specular = Is[i] * Ks * texSpecular * specularTerm;
	totalLight = totalLight + (Id[i] * lambertTerm * diffuse) + specular;
}
	FragColour = vec4(Ia * texDiffuse + totalLight, 1);

}