#ifndef HEIGHTGENERATOR_HPP
#define HEIGHTGENERATOR_HPP

#include <common.hpp>
#include <noiseGenerators.hpp>


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

        //? make a function to give heightMap without storing it and make it virtual so inherited generators can override it
        virtual std::unordered_map<std::pair<float,float>, float, FloatPairHash> makeHeightMap (std::vector<glm::vec3> positions, int seed=21);


};


#endif