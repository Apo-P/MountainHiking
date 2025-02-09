#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <common.hpp>
#include <mesh.hpp>
#include <model.hpp>

#include <OpenSimplex2S.hpp>

// convert float value to bits
inline uint32_t floatToBits(float f) {
    uint32_t result;
    std::memcpy(&result, &f, sizeof(result));  // Same as using a union for type-punning
    return result;
}

// Custom hash function for FloatPair
struct FloatPairHash {
    // operator when called
    size_t operator()(const std::pair<float,float>& pair) const {
        // convert floats to bits
        uint32_t xBits = floatToBits(pair.first);
        uint32_t yBits = floatToBits(pair.second);

        // Combine the bits using a prime multiplier to minimize collisions
        size_t hash = xBits;
        hash = hash * 31 + yBits;  // 31 is a small prime number

        return hash;
    }
};

// Custom hash function for IntPair
struct IntPairHash {
    // operator when called
    size_t operator()(const std::pair<int,int>& pair) const {
        // convert ints to bits
        uint32_t xBits = floatToBits(pair.first);
        uint32_t yBits = floatToBits(pair.second);

        // Combine the bits using a prime multiplier to minimize collisions
        size_t hash = xBits;
        hash = hash * 31 + yBits;  // 31 is a small prime number

        return hash;
    }
};


class Plane {

    private:
    int width;
	int length;
	int widthSegments;
	int lengthSegments;

    

    std::vector<VertexData> createVertices(float width = 1, float length = 1, int widthSegments = 1, int lengthSegments = 1);

    public:
    // TODO: make getter
    std::shared_ptr<Mesh> mesh;

    Plane(float width = 1, float length = 1, int widthSegments = 1, int lengthSegments = 1);
};

class NoiseFunction {

    public:
        // initializer (dont forget seed)
        NoiseFunction(){};

        // Calculate noise and return max noise value - Interface 
        virtual float calculateNoise(float x, float z){};


};

class RandomNoise : public NoiseFunction {

    private:
        std::mt19937 seededGenerator;
        std::uniform_int_distribution<> distribution;

        float noise() {
            return distribution(seededGenerator);
        } ;

    public:
        // create random noise within given range
        RandomNoise(int seed, int range) :
        seededGenerator(seed),
        distribution(0, range)  {  
        };

        float calculateNoise(float x, float z) override {
        
            float noiseValue = 0.0f;
        
            //use noise function (x,z) to get a value
            noiseValue = this->noise();

            return noiseValue;
            
        }


};

class SmoothHill : public NoiseFunction {

    private:
        glm::vec2 hillCenter;

        float hillRadius;

        int maxHeight;

        

        // Define the smoothstep function to smooth step x Between edge0 and edge1
        float smoothstep(float edge0, float edge1, float x) {
            float t = (x - edge0) / (edge1 - edge0);
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t); // Clamp t to [0, 1]
            return t * t * (3.0f - 2.0f * t); // Smoothstep equation
        }


        float noise(float x, float z) ;
    public:

        // create random noise within given range
        SmoothHill(int seed=21, glm::vec2 hillCenter=glm::vec2(250,-250), float hillRadius = 250, float maxHeight=128) :
            hillCenter(hillCenter),
            hillRadius(hillRadius),
            maxHeight(maxHeight)
            {  
            
            };

        float calculateNoise(float x, float z) override {
        
            float noiseValue = 0.0f;
        
            //use noise function (x,z) to get a value
            noiseValue = this->noise(x,z);

            return noiseValue;
            
        }

};

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



class HeightGenerator {

    private:
        // The max and min height we want to normalize to
        float MAX_HEIGHT, MIN_HEIGHT;

        // a hash map for each x,z pair. Stored to speed up process
        
        std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;
        // makes an unorded map with a string as a key and a float as a value
        // we use a string to vaoid making a hash function
        // dont forget use make_hash whe entering keys

        //? we could store the noise generators in a vector

        // generates a height map for positions and stores it
        void generateHeightMap(std::vector<glm::vec3> positions, int seed=21);
        // normalizes the heightMap
        void normalizeHeightMap();

    public:
        HeightGenerator(float MIN_HEIGHT=0, float MAX_HEIGHT=128) :
            MIN_HEIGHT(MIN_HEIGHT),
            MAX_HEIGHT(MAX_HEIGHT) {

            };

        

        // returns normalized height; 
        float getHeight(float x, float z);

        //? make a function to give heightMap without storing it
        std::unordered_map<std::pair<float,float>, float, FloatPairHash> makeHeightMap (std::vector<glm::vec3> positions, int seed=21);


};


class TerrainChunk {

    private:

        // chunks world start position
        float chunkX, chunkZ;
        // chunk size
        int chunkSize;
        // how many segments/faces will it have
        int resolution;

        //! be carefull chunks model matrix is 1 as its position is in worldspace coords

        HeightGenerator& heightGenerator;

        std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;



        void createMesh();

        std::vector<VertexData> createVertices();


    public:
        std::shared_ptr<Mesh> mesh;


        TerrainChunk( HeightGenerator& heightGenerator, float chunkX=0, float chunkZ=0, int chunkSize=500, int resolution=128); // add LOD level later

        void generateChunk(int seed=21);

};


class ChunkManager {

    private:
        // height generator for chunks
        std::shared_ptr<HeightGenerator> generator;

        int seed;

        // the half size of the grid to make (i.e if you want a grid of 9,9 this would be 3 to make from -3 to)
        int GridSizeHalf;

        // will make a grid of gridSize, of terrain chunks
        void initTerrain();

        void addChunk(int x, int z);

    public:

        // a map of all the chunks in a grid
        std::unordered_map<std::pair<int,int>, std::shared_ptr<TerrainChunk>, IntPairHash> chunks;


        ChunkManager(int seed=21, int GridSizeHalf=3) :
            seed(seed),
            GridSizeHalf(GridSizeHalf) {

                //init heightGenerator
                float minHeight = 0;
                float maxHeight = 128;
                generator = std::make_shared<HeightGenerator>(minHeight, maxHeight);

                //init terrain
                initTerrain();

            };

        

};


#endif