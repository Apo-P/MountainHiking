#include <noiseGenerators.hpp>

float RandomNoise::noise() {
    return distribution(seededGenerator);
} ;

float RandomNoise::calculateHeight(float x, float z, int maxHeight) {
        
    float noiseValue = 0.0f;

    //use noise function (x,z) to get a value
    noiseValue = this->noise() * maxHeight;

    return noiseValue;
    
}



float SmoothHill::smoothstep(float edge0, float edge1, float x) {
    float t = (x - edge0) / (edge1 - edge0);
    t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t); // Clamp t to [0, 1]
    return t * t * (3.0f - 2.0f * t); // Smoothstep equation
}

float SmoothHill::noise(float x, float z) {
    //using smoothstep

    glm::vec2 position = glm::vec2(x,z);

    // calculate distance from center
    float distance = glm::length(hillCenter- position);
    
    // find height factor
    float heightFactor = smoothstep(0.0f, hillRadius, distance);

    

    // return height
    float height = maxHeight * (1.0f - heightFactor);
    // we subtract in order to get the center to be the greatest height
     
    

    return height;
} ;

float SmoothHill::calculateHeight(float x, float z, int maxHeight) {
        
    float noiseValue = 0.0f;

    //use noise function (x,z) to get a value
    noiseValue = this->noise(x,z) * maxHeight;

    return noiseValue;
    
}



float SimplexNoise::noise(float x, float z) {


    // as we increase scale the number of hills decreases
    float scale = 200; 

    float xScaled = x / scale;
    float zScaled = z / scale;

    //! Set scale, persistence and the rest of parameters in init function
    
    // Set the persistence and octaves
    //? dont know why but some use it
    const float G = pow(2,-persistence);

    // Multiple octaves with persistence
    double noiseValue = 0.0;

    //? these could be configurable
    double amplitude = 1.0;  // Initial amplitude for first octave
    double frequency = 1.0;  // Initial frequency for first octave

    float lacunarity = 2; // for fast to increase freq
    //used to calculate normalization
    float maxAmplitude = 0;

    //multiple passes for better result
    for (int i = 0; i < octaves; ++i) {

        noiseValue += amplitude * simplex.noise2(xScaled, zScaled);

        frequency *= lacunarity;  // Increase frequency based on lacunarity

        maxAmplitude += amplitude; //keep adding amplitude to know max value

        amplitude *= persistence;  // *=G // Reduce amplitude for next octave
    }

    // Normalize from (-maxAmplitude,+maxAmplitude) to [0, 1]
    double normalizedHeight = (noiseValue + maxAmplitude) / (2 * maxAmplitude);

    // exponentiation //makes transition bigger between vallies and hills
    // return pow(normalizedHeight, exponentiation);

    return normalizedHeight;
};

float SimplexNoise::calculateHeight(float x, float z, int maxHeight) {
        
    float noiseValue = 0.0f;

    //use noise function (x,z) to get a value
    noiseValue = this->noise(x,z) * maxHeight;

    return noiseValue;
    
}

float SimplexNoise::calculateRadius(float x, float z, float maxRadius) {

    float noiseValue = 0.0f;

    //use noise function (x,z) to get a value
    noiseValue = this->noise(x,z) * maxRadius;

    return noiseValue;

}

