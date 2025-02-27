#version 330 core

layout (location = 0) in vec3 modelSpace_vertexPosition;
layout (location = 1) in vec3 modelSpace_vertexNormal;

// Model * View * Projection
uniform mat4 M;

// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};

// out to geometry shader
out VS_OUT {
    vec3 normal;
} vs_out;


void main()
{
    gl_Position = V * M * vec4(modelSpace_vertexPosition, 1.0); 

    mat3 normalMatrix = mat3(transpose(inverse(V * M)));
    
    vs_out.normal = normalize(vec3(vec4(normalMatrix * modelSpace_vertexNormal, 0.0)));
}