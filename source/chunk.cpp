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

#include <OpenSimplex2S.hpp>

class SimplexNoise : public NoiseFunction {

    private:
    
        OpenSimplex2S simplex;

        int lodLevel;
        float noiseScale;
        float persistence;
        int octaves;
        // chunk starting x,z used for offset
        float chunkX,chunkZ;

        int maxHeight;

        float noise(float x, float z) {

            
            // double worldX = (m_chunkX * CHUNK_SIZE) + x;
            // double worldZ = (m_chunkZ * CHUNK_SIZE) + z;

            // Add some offset to avoid repeating patterns between chunks
            // double offsetX = chunkX * 1000.0;
            // double offsetZ = chunkZ * 1000.0;

            //for test
            maxHeight = 128;

            // as we increase scale the number of hills decreases
            float scale = 200; 

            float xScaled = x / scale;
            float zScaled = z / scale;

            
            // Set the persistence and octaves
            float persistence = 0.5;  // Example persistence
            int octaves = 4;  // Number of octaves for terrain detail


            // Multiple octaves with persistence
            double noiseValue = 0.0;
            double amplitude = 1.0;  // Initial amplitude for first octave
            double frequency = 1.0;  // Initial frequency for first octave

            float lacunarity = 2;
            float maxAmplitude = 0;

            //multiple passes for better result
            for (int i = 0; i < octaves; ++i) {

                noiseValue += amplitude * simplex.noise2(xScaled, zScaled);

                frequency *= lacunarity;  // Increase frequency based on lacunarity

                maxAmplitude += amplitude; //keep adding amplitude to know max value

                amplitude *= persistence;  // Reduce amplitude for next octave
            }

            // Normalize from (-maxAmplitude,+maxAmplitude) to [0, 1]
            double normalizedHeight = (noiseValue - maxAmplitude) / (2 * maxAmplitude);

            float height = normalizedHeight * maxHeight;


            return height;
        };

    public:

    SimplexNoise(int seed=21) : 
        simplex(seed) { //initialize open simplex
 
    
        //configure

        // Calculate the LOD level based on the distance
        // int lodLevel = calculateLODLevel(distance);

        // Use the appropriate scale based on the LOD level
        // float noiseScale = getNoiseScaleForLOD(lodLevel);

        

    };

    float calculateNoise(float x, float z) override {
        
        float noiseValue = 0.0f;
    
        //use noise function (x,z) to get a value
        noiseValue = this->noise(x,z);

        return noiseValue;
        
    }

};



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
    
    float width = chunkSize;
    float length = chunkSize;

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


        // we want -iz so chunk grows towards -z
        z = (-iz * segment_length) + chunkZ;

        for(int ix = 0; ix < gridX1; ix++) {

            x = (ix * segment_width) + chunkX;

            positions.push_back(glm::vec3(x, 0, z));

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


void ChunkManager::addChunk(int x, int z) {
    std::cout << "adding chunk x:" << x << " z:" << z << std::endl;

    int chunkSize=500;
    float chunkX, chunkZ;
    int resolution = 128;

    chunkX = x * chunkSize;
    chunkZ = z * chunkSize;

    // make requested chunk
    std::shared_ptr<TerrainChunk> newChunk = std::make_shared<TerrainChunk>(*generator.get(), chunkX, chunkZ, chunkSize, resolution);

    // here we could add special generators or something based on which grid positions it is
    // so we could make a biome system
    // We always should have a big mountain in the middle of the world

    // generate the chunk
    //? should this should be done at chunk call?
    newChunk.get()->generateChunk();

    // calculate edges of chunk to change the as required to improve performance


    // add chunk to list
    chunks[std::make_pair(x,z)] = newChunk;

}

void ChunkManager::initTerrain() {

    // initialize anything else needed for generation

    // make grid of chunks

    for (int x = -GridSizeHalf; x <= GridSizeHalf; x++) {
        for (int z = -GridSizeHalf; z <= GridSizeHalf; z++) {
            addChunk(x,z);
        }
    }

}



