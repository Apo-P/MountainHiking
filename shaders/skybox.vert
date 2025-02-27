#version 330 core

layout (location = 0) in vec3 modelSpace_vertexPosition;

// is vec3 because of cubemap
out vec3 uvCoords;

// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};


void main()
{
    // remove translation from view matrix with this trick
    mat4 View = mat4(mat3(V));

    vec4 worldSpace_vertexPosition = P * View * vec4(modelSpace_vertexPosition, 1.0);

    // Optimization set z to w so it will be set to 1 when normalized. So the depth buffer is equal to 1 and skybox is only drawn when there is nothing there
    gl_Position = worldSpace_vertexPosition.xyww;

    // set uv coords to position * time the factor we want
    uvCoords = modelSpace_vertexPosition;
}
