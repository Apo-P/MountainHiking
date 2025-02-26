#include <heightGenerator.hpp>

float HeightGenerator::calculateHeight(float x, float z) {


    //init final noise value
    float noiseValue = 0.0f;

    //use noise function (x,z) to get a value
    //? could use multiple noises here to calculate

    // make a big hill at center (can multiply weight here so it can have a bigger effect)
    // noiseValue += 4 * smoothHill.calculateHeight(x,z);

    //if we are on a hill add noise
    // if (noiseValue!=0) {
    //     // noiseValue += rndNoise.calculateHeight(x,z);
    // }

    // use simplex noise as test
    float simplexWeight = 128;
    noiseValue += smpNoise.calculateHeight(x,z, simplexWeight);



    //! should find better way then to do it by hand
    // noise range
    float noiseMin = 0;
    float noiseMax = 128 + 128; //128 by the smooth hill and 128 by the simplex noise

    // Normalize to our desire range
    float range = MAX_HEIGHT - MIN_HEIGHT;

    //normalize all values to 0-1 (clamp all above 1)
    noiseValue = std::min((noiseValue - noiseMin) / (noiseMax - noiseMin), 1.0f); 

    // scale them to desired ranges
    float height = noiseValue * range + MIN_HEIGHT;

    return height;

}

std::unordered_map<std::pair<float,float>, float, FloatPairHash> HeightGenerator::makeHeightMap (std::vector<glm::vec3> positions, int seed) { 
    // std::cout << "TODO" << std::endl;

    std::unordered_map<std::pair<float,float>, float, FloatPairHash> returnHeightMap;

    // initialize noiseFunction with seed

    for(auto pos : positions){

        float height = calculateHeight(pos.x, pos.z);
        returnHeightMap[std::make_pair(pos.x,pos.z)] = height;

    }

    return returnHeightMap;

}


