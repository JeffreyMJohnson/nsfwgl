#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
	vec3 Color;
	vec3 Direction;
	mat4 Projection;
	mat4 View;
};

//direction in view-space
uniform DirectionalLight Directional;

uniform vec3 CameraPosition;
uniform mat4 CameraView;
uniform float SpecPower;

uniform vec3 AmbientColor;

uniform sampler2D PositionMap;//view space
uniform sampler2D NormalMap;

//shadows
uniform mat4 TextureSpaceOffset;
uniform sampler2D ShadowMap;
uniform float ShadowBias = .01;


void main()
{
	vec3 normal = normalize(texture(NormalMap, vTexCoord).xyz);
	vec3 position = texture(PositionMap, vTexCoord).xyz;//view space

	//shadows
	mat4 lightViewProjection = TextureSpaceOffset * Directional.Projection * Directional.View;
	vec4 shadowCoord = lightViewProjection * inverse(CameraView) * vec4(position, 1);

	//compute diffuse lighting
	vec3 lightDirection = Directional.Direction;
	float d = max(dot(normal, lightDirection), 0); //lambertian term

	//shadow
	if (texture(ShadowMap, shadowCoord.xy).r < shadowCoord.z - ShadowBias)
	{
		d = 0;
	}

	//compute specular lighting
	vec3 CamViewPosition = (CameraView * vec4(CameraPosition, 1)).xyz;
	vec3 E = normalize(CamViewPosition - position); //Eye vector
	vec3 R = reflect(-lightDirection, normal);//reflection vector
	float s = pow(max(dot(E, R), 0), SpecPower);//specular

	LightOutput = AmbientColor + (Directional.Color * d) + (Directional.Color * s);

}