#include <heightGenerator.hpp>


void HeightGenerator::generateHeightMap(std::vector<glm::vec3> positions, int seed) {

    // initialize noiseFunction with seed
    std::mt19937 seededGenerator(seed);

    std::uniform_int_distribution<> distribution(10, 20); // define the range
    

    for(auto pos : positions){
        
        float x = pos.x;
        float z = pos.z;

        float maxNoiseValue = 10.0f; // maximum possible noise value
        float noiseValue = 0.0f;

        //use noise function (x,z) to get a value
        noiseValue = distribution(seededGenerator);
        

        //Put final height to heightMap
        float height = noiseValue / maxNoiseValue; // normalize with maximum height

        heightMap[std::make_pair(x,z)] = height;

    }

}

void HeightGenerator::normalizeHeightMap() {

    // set max and min to start search
    float minHeight = FLT_MAX, maxHeight = -FLT_MAX;


    // for every entry in height map
    for (auto entry :heightMap) {

        float value = heightMap[entry.first];
        
        // find min and max
        minHeight = std::min(minHeight, value);
        maxHeight = std::max(maxHeight, value);
    }


    // Normalize to our desire range
    float range = MAX_HEIGHT - MIN_HEIGHT;

    for (auto entry :heightMap) {

        float value = heightMap[entry.first];
        
        //normalize all values to 0-1
        heightMap[entry.first] = (value - minHeight) / (maxHeight - minHeight); 


        // scale them to desired ranges
        heightMap[entry.first] = heightMap[entry.first] * range + MIN_HEIGHT;


        
    
    }


}


std::unordered_map<std::pair<float,float>, float, FloatPairHash> HeightGenerator::makeHeightMap (std::vector<glm::vec3> positions, int seed) { 
    // std::cout << "TODO" << std::endl;

    std::unordered_map<std::pair<float,float>, float, FloatPairHash> returnHeightMap;

    // initialize noiseFunction with seed


    //TODO Change this to be configurable in height generation init
    //! carefull with -z because we look towards -z!

    SmoothHill smoothHill(21, glm::vec2(250,-250), 500, 128);

    // add some random noise //? with a weight(range) of 5 
    RandomNoise rndNoise(seed, 5);

    // test SimpleNoise
    SimplexNoise smpNoise(21);

    
    // to find max and min height
    float minHeight = FLT_MAX, maxHeight = -FLT_MAX;

    for(auto pos : positions){
        
        float x = pos.x;
        float z = pos.z;

        float noiseValue = 0.0f;

        //use noise function (x,z) to get a value
        //? could use multiple noises here to calculate
    
        // make a big hill at center (can multiply weight here so it can have a bigger effect)
        noiseValue += 4 * smoothHill.calculateNoise(x,z);

        //if we are on a hill add noise
        // if (noiseValue!=0) {
        //     // noiseValue += rndNoise.calculateNoise(x,z);
        // }

        // use simplex noise as test
        noiseValue += smpNoise.calculateNoise(x,z);

        
        //Put final height to heightMap
        float height = noiseValue; 

        //store height
        returnHeightMap[std::make_pair(x,z)] = height;

    }

    int possize = positions.size();
    int mapsize = returnHeightMap.size();

    // test
    if (possize != mapsize) {
        std::cout << "what?" <<std::endl;
    }

    // normalizeHeightmap heightmap
    
    // estimated max and min height is based on noise function used
    // estimated regardless of actual values
    //! should find better way then to do it by hand
    minHeight = 0;
    maxHeight = 128 + 128; //128 by the smooth hill and 128 by the simplex noise

    // Normalize to our desire range
    float range = MAX_HEIGHT - MIN_HEIGHT;

    for (auto entry :returnHeightMap) {

        float value = returnHeightMap[entry.first];
        
        //normalize all values to 0-1
        
        returnHeightMap[entry.first] = (value - minHeight) / (maxHeight - minHeight); 


        // scale them to desired ranges
        returnHeightMap[entry.first] = returnHeightMap[entry.first] * range + MIN_HEIGHT;

    
    }

    return returnHeightMap;

}


float HeightGenerator::getHeight(float x, float z) {
    if (heightMap.empty()) {
        throw std::logic_error("heightMap is empty!");
    }
    // get Y from height map
    return heightMap[std::make_pair(x,z)];
}

