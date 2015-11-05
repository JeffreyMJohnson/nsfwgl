#version 410

out float FragDepth;//shadow map to produce

void main()
{
	FragDepth = gl_FragCoord.z;//grab z from depth buffer and write to texture....
}