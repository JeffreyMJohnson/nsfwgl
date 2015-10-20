#version 410

in vec4 vNormal;
in vec4 vTangent;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Diffuse;

void main()
{

    FragColor = texture(Diffuse, vTexCoord);
    //FragColor = vec4(vTexCoord, 0.5f, 1);
    //FragColor = vec4(vNormal.xyz, 1);
}