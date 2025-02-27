#version 330 core

// define pbr lights -----------

// a point light
struct PBRPointLight {
    vec4 colorPower; // first three floats are color, last one is power
    vec4 position; // Position of the light
};

// a directional light (mainly the sun)
struct PBRDirectionalLight {
    vec4 colorPower; // first three floats are color, last one is power
    vec4 direction; // direction of the light
};

// max lights we handle
#define MAX_DIR_LIGHTS 1 // 1 directional light
#define MAX_POINT_LIGHTS 1 // 1 point light

// IN VARIABLES -------------------

// Model Space In (per vertex)
layout(location = 0) in vec3 modelSpace_vertexPosition;
layout(location = 1) in vec3 modelSpace_vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent;

//TODO ADD INSTANCED RENDERING

// Model * View * Projection (don't forget that multiplications are down back wards with matrices so P * V * M)
uniform mat4 M;

// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};

// LIGHTS UBO
layout(std140) uniform Lights {
    PBRDirectionalLight dirLights[MAX_DIR_LIGHTS];
    PBRPointLight pointLights[MAX_POINT_LIGHTS];
    int dirLightsCount;
    int pointLightsCount;
};

// OUT TO FRAGMENT -------------------

// uvCoordinates
out vec2 uvCoords;
// Tangent, Bi-tangent, Normal matrix
out mat3 TBN;

// world space vectors
out WORLD_SPACE {
    vec3 position;
    vec3 normal;
    vec3 tangent;
} worldSpace;

// camera space vectors
out CAMERA_SPACE {
    vec3 position;
    vec3 normal;
    vec3 lightDirections[MAX_DIR_LIGHTS];
    vec3 lightPositions[MAX_POINT_LIGHTS];
} cameraSpace;

// main loop
void main() {
    mat4 modelMatrix = M;

    // calculate world space
    worldSpace.position = (modelMatrix * vec4(modelSpace_vertexPosition, 1)).xyz;
    worldSpace.normal = normalize(modelMatrix * vec4(modelSpace_vertexNormal, 0)).xyz;
    worldSpace.tangent = normalize(modelMatrix * vec4(vertexTangent, 0)).xyz;

    // Gram-Schmidt orthogonalization
    worldSpace.tangent = normalize(worldSpace.tangent - dot(worldSpace.tangent, worldSpace.normal) * worldSpace.normal);

    // calculate bitangent as cross product of tangent and normal
    vec3 bitangent = cross(worldSpace.normal, worldSpace.tangent);
    // pass tbn to fragment
    TBN = mat3(worldSpace.tangent, bitangent, worldSpace.normal);

    // calculate camera space
    cameraSpace.position = (V * vec4(worldSpace.position, 1)).xyz;
    cameraSpace.normal = normalize(V * vec4(worldSpace.normal, 0)).xyz;

    // loop through for each light
    for (int i=0; i<dirLightsCount; i++) {
        cameraSpace.lightDirections[i] = (V * dirLights[i].direction).xyz;
    }
    for (int i=0; i<pointLightsCount; i++) {
        cameraSpace.lightPositions[i] = (V * pointLights[i].position).xyz;
    }


    // pass uv coords to fragment
    uvCoords = vertexUV;

    // calculate view space (set gl position)
    gl_Position = P * V * M * vec4(modelSpace_vertexPosition, 1);
}