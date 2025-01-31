#version 330 core

// Model Space In
layout(location = 0) in vec3 modelSpace_vertexPosition;

uniform vec3 cameraPosition;

// Model * View * Projection
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


void main()
{
    
    gl_Position = M * V * P *vec4(modelSpace_vertexPosition, 1.0);;
}