#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
	vec3 Color;
	vec3 Direction;
	mat4 projection;
	mat4 view;
};

//direction in view-space
uniform DirectionalLight directional;

uniform vec3 CameraPosition;
uniform mat4 CameraView;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;//view space
uniform sampler2D normalTexture;

//shadows
uniform mat4 TextureSpaceOffset;
uniform sampler2D ShadowMap;
uniform float shadowBias = .01;


void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;//view space

	//shadows
	mat4 lightViewProjection = TextureSpaceOffset * directional.projection * directional.view;
	vec4 shadowCoord = lightViewProjection * inverse(CameraView) * vec4(position, 1);

	//compute diffuse lighting
	vec3 lightDirection = directional.Direction;
	float d = max(dot(normal, lightDirection), 0); //lambertian term
	//shadow
	if (texture(ShadowMap, shadowCoord.xy).r < shadowCoord.z - shadowBias)
	{
		d = 0;
	}

	//compute specular lighting
	vec3 CamViewPosition = (CameraView * vec4(CameraPosition, 1)).xyz;
	vec3 E = normalize(CamViewPosition - position); //Eye vector
	vec3 R = reflect(-lightDirection, normal);//reflection vector
	float s = pow(max(dot(E, R), 0), specPower);//specular

	LightOutput = ambient + (directional.Color * d) + (directional.Color * s);
	//LightOutput = texture(ShadowMap, shadowCoord.xy).xyz;

}