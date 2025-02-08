#include <chunk.hpp>

// int rand_int () {
//     std::random_device randomNum; // obtain a random number from hardware
//     std::mt19937 randomGenerator(randomNum); // seed the generator (with random number if we want)
//     std::mt19937 seededGenerator(21); // seed the generator (with same number if we want)
//     std::uniform_int_distribution<> distribution(25, 63); // define the range (min, max)

//     return distribution(randomGenerator); //returns a random value in distribution using generator

// }




std::string make_hash(float x, float z) {
    return std::to_string(x) + "," + std::to_string(z);
} // use this to avoid making a hash function for the std::pair


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



std::vector<VertexData> Plane::createVertices(float width, float length, int widthSegments, int lengthSegments) {
    //to have it go from -half_width to +half_width in model space
    float width_half = width / 2;
    float length_half = length / 2;

    // store segments
    int gridX = widthSegments;
    int gridZ = lengthSegments;

    // add +1 to create enough vertices
    int gridX1 = gridX + 1;
    int gridZ1 = gridZ + 1;

    // calculate segment width and length
    float segment_width = width / gridX;
    float segment_length = length / gridZ;

    // create arrays for vertex data
    std::vector<glm::vec3> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;


    float x,z;
    for(int iz = 0; iz < gridZ1; iz++) {

        z = iz * segment_length - length_half;

        for(int ix = 0; ix < gridX1; ix++) {

            x = ix * segment_width - width_half;

            //! carefull with the -z (since we look towards the -z)
            positions.push_back(glm::vec3(x, 0, -z));

            normals.push_back(glm::vec3(0, 0, 1));

            uvs.push_back( glm::vec2( ix / gridX ));
            uvs.push_back( glm::vec2( 1 - ( iz / gridZ )) );

        }

    }

    // indice data
    unsigned int a,b,c,d;
    for (int iz = 0; iz < gridZ; iz ++ ) {

        for (int ix = 0; ix < gridX; ix ++ ) {

            a = ix + gridX1 * iz;
            b = ix + gridX1 * ( iz + 1 );
            c = ( ix + 1 ) + gridX1 * ( iz + 1 );
            d = ( ix + 1 ) + gridX1 * iz;

            indices.push_back(glm::vec3 (a, b, d) );
            indices.push_back(glm::vec3 (b, c, d) );

        }

    }

    // pack data to vertex data
    std::vector<VertexData> vertexData;
    for (auto index :indices) {
        vertexData.push_back(VertexData(positions.at(index.x), uvs.at(index.x) ,normals.at(index.x)));
        vertexData.push_back(VertexData(positions.at(index.y), uvs.at(index.y) ,normals.at(index.y)));
        vertexData.push_back(VertexData(positions.at(index.z), uvs.at(index.z) ,normals.at(index.z)));
    }

    return vertexData;
}

Plane::Plane(float width, float length, int widthSegments, int lengthSegments) : 
        width(width),
        length(length),
        widthSegments(widthSegments),
        lengthSegments(lengthSegments) {

        std::vector<VertexData> vertexData;

        // create Vertices
        vertexData = createVertices(width, length, widthSegments, lengthSegments);

        // create mesh
        mesh = std::make_shared<Mesh>(vertexData);


        };



oldTerrain::oldTerrain(int size) {
    // createChunk(size);
}





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

    // RandomNoise rndNoise(seed, 120);
    //TODO Change this to be configurable in height generation init
    //! carefull with -z because we look towards -z!
    SmoothHill smoothHill(21, glm::vec2(5,-5), 5, 10.0f);
    
    
    // to find max and min height
    float minHeight = FLT_MAX, maxHeight = -FLT_MAX;

    for(auto pos : positions){
        
        float x = pos.x;
        float z = pos.z;

        float noiseValue = 0.0f;

        //use noise function (x,z) to get a value
        //? could use multiple noises here to calculate
        // noiseValue = rndNoise.calculateNoise(x,z);
        noiseValue = smoothHill.calculateNoise(x,z);
        
        //Put final height to heightMap
        float height = noiseValue; 


        // check for min,max value
        minHeight = std::min(minHeight, height);
        maxHeight = std::max(maxHeight, height);

        //store height
        returnHeightMap[std::make_pair(x,z)] = height;

    }

    int possize = positions.size();
    int mapsize = returnHeightMap.size();

    if (possize != mapsize) {
        std::cout << "what?" <<std::endl;
    }

    // normalizeHeightmap heightmap
    
    // if they are the same then add 1 to avoid dividing by 0
    if ((maxHeight - minHeight) == 0){
        maxHeight=1;
    }

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


TerrainChunk::TerrainChunk(HeightGenerator& heightGenerator, float chunkX, float chunkZ, int chunkSize, int resolution) :
    heightGenerator(heightGenerator),
    chunkX(chunkX),
    chunkZ(chunkZ),
    chunkSize(chunkSize),
    resolution(resolution) {

        std::cout << "chunk created" << std::endl;
}

std::vector<VertexData> TerrainChunk::createVertices() {

    // float width, float length, int widthSegments, int lengthSegments
    // total width and length is starting point + size
    float width = chunkX + chunkSize;
    float length = chunkZ + chunkSize;

    // store segments
    int gridX = resolution;
    int gridZ = resolution;

    // add +1 to create enough vertices
    int gridX1 = gridX + 1;
    int gridZ1 = gridZ + 1;

    // calculate segment width and length
    float segment_width = width / gridX;
    float segment_length = length / gridZ;

    // create arrays for vertex data
    std::vector<glm::vec3> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;


    float x,z;
    for(int iz = 0; iz < gridZ1; iz++) {

        z = iz * segment_length;

        for(int ix = 0; ix < gridX1; ix++) {

            x = ix * segment_width;

            positions.push_back(glm::vec3(x, 0, -z));

            normals.push_back(glm::vec3(0, 0, 1));

            uvs.push_back( glm::vec2( ix / gridX ));
            uvs.push_back( glm::vec2( 1 - ( iz / gridZ )) );

        }

    }

    // indice data
    unsigned int a,b,c,d;
    for (int iz = 0; iz < gridZ; iz ++ ) {

        for (int ix = 0; ix < gridX; ix ++ ) {

            a = ix + gridX1 * iz;
            b = ix + gridX1 * ( iz + 1 );
            c = ( ix + 1 ) + gridX1 * ( iz + 1 );
            d = ( ix + 1 ) + gridX1 * iz;

            indices.push_back(glm::vec3 (a, b, d) );
            indices.push_back(glm::vec3 (b, c, d) );

        }

    }

    //? could use indices vertexData from the start to increase performance

    // pack data to vertex data
    std::vector<VertexData> vertexData;
    for (auto index :indices) {
        vertexData.push_back(VertexData(positions.at(index.x), uvs.at(index.x) ,normals.at(index.x)));
        vertexData.push_back(VertexData(positions.at(index.y), uvs.at(index.y) ,normals.at(index.y)));
        vertexData.push_back(VertexData(positions.at(index.z), uvs.at(index.z) ,normals.at(index.z)));
    }

    return vertexData;
}

void TerrainChunk::generateChunk(int seed) {

    //? Make alternative version to add height to heightmap as position is created to reduce complexity
    //? so not to first create everything and the modify it


    std::vector<VertexData> vertices = createVertices();

    //? should change this to reduce complexity;

    std::vector<glm::vec3> positions;
    for (auto vertex : vertices){
        positions.push_back(vertex.position);
    }

    // get heightMap
    heightMap = heightGenerator.makeHeightMap(positions, seed);

    // update vertices
    for (auto& vertex : vertices){
        vertex.position.y = heightMap[std::make_pair(vertex.position.x, vertex.position.z)];
    } 

    // generate Mesh
    mesh = std::make_shared<Mesh>(vertices);

}


// #include <functional> // for std::function
// could using this instead of typedef 

#include <utility> // for std::pair

// define a pointer to a function that takes 2 floats and return 1 float
typedef float (*generatorGet) (float, float);


