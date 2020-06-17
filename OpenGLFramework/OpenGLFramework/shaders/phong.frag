// classic Phong fragment shader
#version 410
in vec4 vPosition;
in vec3 vNormal;
uniform vec3 Ia; // ambient light colour
uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power
uniform vec3 cameraPosition;

uniform vec3 lightDirection;
out vec4 FragColour;

in vec2 vTexCoord;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main() {
	vec3 diffuseColour = texture(diffuseTexture, vTexCoord).xyz;
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
	vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );
	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );
	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd  * texDiffuse * lambertTerm ;
	vec3 specular = Is * Ks * texSpecular * specularTerm;
	// output final colour
	FragColour = vec4( ambient + diffuse + specular, 1 );
}