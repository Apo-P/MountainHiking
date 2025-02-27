#version 330 core

// structs and constants

// define pbr lights 
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

const float PI = 3.14159265359;


// IN VARIABLES -------------------

//! careful with position
// texture samplers (starting from pos =0)
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// Camera Position
uniform vec3 cameraPosition;

// LIGHTS UBO
layout(std140) uniform Lights{
	PBRDirectionalLight dirLights[MAX_DIR_LIGHTS];
	PBRPointLight pointLights[MAX_POINT_LIGHTS];
	int dirLightsCount;
	int pointLightsCount;
};

// IN FROM VERTEX
in vec2 uvCoords;
in mat3 TBN;

in WORLD_SPACE {
	vec3 position;
	vec3 normal;
	vec3 tangent;
} worldSpace;

in CAMERA_SPACE {
	vec3 position;
	vec3 normal;
	vec3 lightDirections[MAX_DIR_LIGHTS];
	vec3 lightPositions[MAX_POINT_LIGHTS];
} cameraSpace;

// functions   ---------------

// converts a normal from a normal map to worldSpace
vec3 tangentSpaceNormal(vec3 normal) {
	// input is a normal from normal map in range [0,1]

	// transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0); // this normal is in tangent space

	// multiply to get to world space
	normal = normalize(TBN * normal); // this normal is in world space

	return normal;
}


float DistributionGGX(float NdotH, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CookToranceBRDF(
		vec3 N,
		vec3 V,
		vec3 L,
		vec3 H,
		vec3 radiance,
		vec3 albedo,
		float metallic,
		float roughness,
		vec3 F0
	) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);
	float VdotH = max(dot(V, H), 0.0);

	float NDF = DistributionGGX(NdotV, roughness);
	float G   = GeometrySmith(NdotV, NdotL, roughness);
	vec3 F    = fresnelSchlick(VdotH, F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular     = numerator / denominator;

	specular *= metallic;

	vec3 diffuse = kD * albedo / PI;

	// add to outgoing radiance Lo
	return (diffuse + specular) * radiance * NdotL;
}

vec3 pbrLighting(float visibility, vec3 defaultF0) {
	// Lighting parameters for fragment
	vec3 albedo     = texture(albedoMap, uvCoords).rgb;
    float metallic  = texture(metallicMap, uvCoords).r;
	vec3 normal = texture(normalMap, uvCoords).rgb;
    float roughness = texture(roughnessMap, uvCoords).r;
    float ao        = texture(aoMap, uvCoords).r;

	// vec3 N = normalize(worldSpace.normal); // Fragment normal
	vec3 N = tangentSpaceNormal(normal);
	vec3 V = normalize(cameraPosition - worldSpace.position); // View vector

	vec3 F0 = mix(defaultF0, albedo, metallic);

	// Reflectance equation output
	vec3 Lo = vec3(0.0);

	for (int i=0; i<dirLightsCount; i++) {
		vec3 L = normalize(-dirLights[i].direction.xyz); // Light vector
		vec3 H = normalize(V+L); // Halfway vector

		vec3 radiance = dirLights[i].colorPower.xyz * dirLights[i].colorPower.w;

		Lo += CookToranceBRDF(N, V, L, H, radiance, albedo, metallic, roughness, F0);
	}

	for (int i=0; i<pointLightsCount; i++) {
		vec3 L = pointLights[i].position.xyz - worldSpace.position; // Light vector
		float distance = length(L);

		L = normalize(L); // Normalized light vector
		vec3 H = normalize(V+L); // Halfway vector

		float attenuation = 1.0 / (distance * distance); // inverse square law
		vec3 radiance = pointLights[i].colorPower.xyz * pointLights[i].colorPower.w * attenuation;

		Lo += CookToranceBRDF(N, V, L, H, radiance, albedo, metallic, roughness, F0);
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

	return color;
}


vec3 hdrToneMapping(vec3 color) {
	return color / (color + vec3(1.0));
}
vec3 gammaCorrection(vec3 color) {
	return pow(color, vec3(1.0/2.2));
}

//? Why is this?
float shadows() {
	return 1.0;
}

void main() {
    // where is visibility used?
	float visibility = shadows();

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 

	vec3 color = pbrLighting(visibility, F0);

	color = hdrToneMapping(color);

	color = gammaCorrection(color);

	gl_FragColor = vec4(color, 1.0);

}
