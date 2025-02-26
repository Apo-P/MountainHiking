#ifndef HEIGHTGENERATOR_HPP
#define HEIGHTGENERATOR_HPP

#include <common.hpp>
#include <noiseGenerators.hpp>


class HeightGenerator {

    private:
        // The max and min height we want to normalize to
        float MAX_HEIGHT, MIN_HEIGHT;

        //TODO Change this to be configurable in height generation init
        int seed = 21;

        //? we could store the noise generators in a vector

        //! carefull with -z because we look towards -z!
        // smoothHill in center
        SmoothHill* smoothHill; // smoothHill(seed, glm::vec2(250,-250), 500, 128); //? make a smooth hill with a center of 250,250 of radius 500 and height 128

        // random noise
        RandomNoise* rndNoise; // rndNoise(seed, 5); // add some random noise //? with a (range) of 5 

        // SimplexNoise
        SimplexNoise* smpNoise; //smpNoise(seed);
        

        


    public:
        HeightGenerator(int seed=21, float MIN_HEIGHT=0, float MAX_HEIGHT=3 * 128) :
            seed(seed),
            MIN_HEIGHT(MIN_HEIGHT),
            MAX_HEIGHT(MAX_HEIGHT)  
            {
                smoothHill = new SmoothHill(seed, glm::vec2(250,-250), 600, 168); //use direct initialization instead of copy in the body cause its faster
                rndNoise = new RandomNoise(seed, 5);
                smpNoise = new SimplexNoise(seed, 500, 1, 4, 3);

                //HeighGenerator made
            };

        //TODO make an updater for noise functions
        void updateHill(glm::vec2 newPos, float newRadius=500, float newHeight=128){ 
            this->smoothHill->updateValues(newPos,newRadius,newHeight); 
        };
        void updateSimplex(float noiseScale=200, float persistence = 0.5, int octaves=4, float exponentiation=3){ 
            this->smpNoise->updateValues(noiseScale, persistence, octaves, exponentiation); 
        };



        float calculateHeight(float x, float z);

        //? make a function to give heightMap without storing it and make it virtual so inherited generators can override it
        // a hash map for each x,z pair.
        // std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;
        // makes an unordered map with a pair of floats as a key and a float as a value
        // dont forget use make_PAIR whe entering keys
        virtual std::unordered_map<std::pair<float,float>, float, FloatPairHash> makeHeightMap (std::vector<glm::vec3> positions, int seed=21);


};


#endif