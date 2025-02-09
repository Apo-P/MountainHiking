#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <common.hpp>
#include <mesh.hpp>
#include <model.hpp>

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





class oldTerrain: public Model {
    public:
        /// @brief constructor
        /// ! add terrain location for loading terrain
        oldTerrain(int size);

        float chunkSize=10;

        /// @brief create chunk at x,z
        void createChunk(int x,int z){
            glm::vec2 offset = glm::vec2(x * chunkSize, z * chunkSize);
        };
};

//generating func
class oldGenerator {

    static double sat(double x){
      return glm::min(glm::max(x, 0.0), 1.0);
    }

    public:
    oldGenerator(){};

    static const int chunkCenter = 250;
    static const int maxHeight = 128;
    // static glm::vec2 chuckStartinPoint = glm::vec2(0,0);
    

    static float Get(float x, float z) {
        float distance = glm::distance(glm::vec2(0,0), glm::vec2(x,z));

        // h = length
        float h = 1.0 - sat(distance / chunkCenter);
        h = h * h * h * (h * (h * 6 - 15) + 10);

        // return length, normalization
        float length = h * maxHeight; //max length
        return length;//128; //, 1;
    }

};

class oldHeightGenerator {

    glm::vec2 offset;
    glm::vec2 radius;
    /// @brief generator
    std::shared_ptr<oldGenerator> generator;

    double sat(double x){
      return glm::min(glm::max(x, 0.0), 1.0);
    }

    public:

    oldHeightGenerator(std::shared_ptr<oldGenerator> generator, glm::vec2& new_offset, float minRadius, float maxRadius) :
        generator(generator),
        offset(new_offset)
    {
        radius = glm::vec2(minRadius, maxRadius);
    }

    std::pair< float, float > Get(const float x, const float y) {

        float distance = glm::distance(offset, glm::vec2(x,y));
        
        float normalization = 1.0 - sat(
            (distance - radius.x) / (radius.y - radius.x));


        normalization = normalization * normalization * (3 - 2 * normalization);

        float length = generator.get()->Get(x, y); 

        return {length, normalization};
    }

};

class oldTerrainChunkManager {

    public:
        int chunkGroup; //change later

    oldTerrainChunkManager(){}; //change later

};

class oldTerrainChunk {
    // constructor(params) {
    // this._params = params;
    // this._Init(params);
    // }

    glm::vec3 size;
    // how many faces will it have
    int resolution = 128;
    glm::vec3 offset;

    public:
    Plane* plane; //temporary

    oldTerrainChunk(oldTerrainChunkManager &TerrainManager, glm::vec3 offset=glm::vec3(0), float chunkSize=500 , float scale=1) {  //!Terrain will be controller

        // size
        size = glm::vec3(chunkSize * scale, 0, chunkSize * scale);

        //store offset
        offset = offset;

        //make plane 
        plane = new Plane(size.x, size.z, resolution, resolution);
        // sets plane's position
        // plane.position = offset

        // add plane mesh to chunk group in Terrain Manager

        // make plane into chunk
        makeChunk();

    }


    void makeChunk() {

        // for testing
        float _chunkSize =500;
        float x = 0,z=0;
        glm::vec2 offset = glm::vec2(x * _chunkSize, z * _chunkSize);

        std::vector<oldHeightGenerator> lengthGenerators = { oldHeightGenerator(std::make_shared<oldGenerator>(), offset, 100000, 100000 + 1) };

        // for every vertex
        int index=0;
        for (auto vertex :plane->mesh.get()->getVertexData()) {

            std::vector< std::pair<float, float> > lengthPairs {};
            float normalization = 0;
            vertex.position.z = 0;

            if (vertex.position.x >240){

            }

            

            for (auto gen :lengthGenerators) {
                lengthPairs.push_back(gen.Get(vertex.position.x + offset.x, vertex.position.y + offset.y));
                
                normalization += lengthPairs.at(lengthPairs.size() - 1 ).second;
            }

            if (normalization > 0) {
                // TODO CHANGE THIS LATER
                float newZ=0;
                for (auto h :lengthPairs) {

                    // vertex.position.z += h.first * h.second / normalization;
                    newZ +=  h.first * h.second / normalization;
                }
                if (newZ != 0) {
                    plane->mesh->updateVertexZ(index, newZ);
                }
            }
            index++;
        }


        // update mesh
        plane->mesh.get()->updateVram();


    }

};

#endif