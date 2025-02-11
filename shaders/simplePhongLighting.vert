#version 330 core

// Model Space In
layout(location = 0) in vec3 modelSpace_vertexPosition;
layout(location = 1) in vec3 modelSpace_vertexNormal;
layout(location = 2) in vec3 modelSpace_vertexUV;

uniform vec3 cameraPosition;

// Model * View * Projection
uniform mat4 M;

// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};

// foward to next stage (fragment)
out vec3 Normal;
out vec3 worldSpace_vertexPosition;


void main()
{
    //! BE CAREFUL WITH MATRIX MULTIPLICATION ORDER
    gl_Position = P * V * M * vec4(modelSpace_vertexPosition, 1.0);

    //send date to next stage (fragment)
    Normal = modelSpace_vertexNormal;

    worldSpace_vertexPosition = vec3(M * vec4(modelSpace_vertexPosition, 1.0));
}