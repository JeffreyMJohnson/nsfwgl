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
uniform vec2 ShadowMapSize = vec2(1280, 720);

float Texture2DCompare(sampler2D depths, vec2 uv, float compare)
{
	float depth = texture2D(depths, uv).r;
	return step(compare, depth);
}

float Texture2DShadowLERP(sampler2D depths, vec2 size, vec2 uv, float compare, float bias)
{
	vec2 texelSize = vec2(1.0) / size;
	vec2 f = fract(uv * size + 0.5);
	vec2 centroidUV = floor(uv * size + 0.5) / size;

	float lb = Texture2DCompare(depths, centroidUV + texelSize * vec2(0.0, 0.0), compare - bias);
	float lt = Texture2DCompare(depths, centroidUV + texelSize * vec2(0.0, 1.0), compare - bias);
	float rb = Texture2DCompare(depths, centroidUV + texelSize * vec2(1.0, 0.0), compare - bias);
	float rt = Texture2DCompare(depths, centroidUV + texelSize * vec2(1.0, 1.0), compare - bias);
	float a = mix(lb, lt, f.y);
	float b = mix(rb, rt, f.y);
	float c = mix(a, b, f.x);
	return c;
}

float PCF(sampler2D depths, vec2 size, vec2 uv, float compare, float bias)
{
	float result = 0.0;
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			vec2 off = vec2(x, y) / size;
			result += Texture2DCompare(depths, uv + off, compare - bias);
		}
	}
	return result / 25.0;
}

float PCF2(sampler2D depths, vec2 size, vec2 uv, float compare, float bias)
{
	float result = 0.0;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			vec2 off = vec2(x, y) / size;
			result += Texture2DShadowLERP(depths, size, uv + off, compare, bias);
		}
	}
	return result / 9.0;
}

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

	//float visibility = Texture2DShadowLERP(ShadowMap, ShadowMapSize, shadowCoord.xy, shadowCoord.z, ShadowBias);
	//float visibility = PCF(ShadowMap, ShadowMapSize, shadowCoord.xy, shadowCoord.z, ShadowBias);
	float visibility = PCF2(ShadowMap, ShadowMapSize, shadowCoord.xy, shadowCoord.z, ShadowBias);

	//compute specular lighting
	vec3 CamViewPosition = (CameraView * vec4(CameraPosition, 1)).xyz;
	vec3 E = normalize(CamViewPosition - position); //Eye vector
	vec3 R = reflect(-lightDirection, normal);//reflection vector
	float s = pow(max(dot(E, R), 0), SpecPower);//specular

	vec2 UVCoords;
	float foo = d + visibility;

	LightOutput = AmbientColor + (Directional.Color * foo) + (Directional.Color * s);

}