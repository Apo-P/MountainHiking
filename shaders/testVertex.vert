#version 330 core

// Model Space In
layout(location = 0) in vec3 modelSpace_vertexPosition;
layout(location = 1) in vec3 modelSpace_vertexNormal;
layout(location = 2) in vec2 modelSpace_vertexUV;

uniform vec3 cameraPosition;

// Model * View * Projection
uniform mat4 M;

// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};

// forward to next stage (fragment)
out vec2 uvCoords;


void main()
{
    //! BE CAREFUL WITH MATRIX MULTIPLICATION ORDER
    gl_Position = P * V * M * vec4(modelSpace_vertexPosition, 1.0);

    // don't forget to forward to next stage the out vars
    uvCoords = modelSpace_vertexUV;
}