#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;

out vec4 vNormal;
out vec4 vTangent;
out vec2 vTexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
    // passthrough from vertex to frag
    vNormal = Normal;
    vTangent = Tangent;
    vTexCoord = TexCoord;
    
    gl_Position = Projection * View * Model * Position;
}