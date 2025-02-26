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
        SmoothHill smoothHill; // smoothHill(seed, glm::vec2(250,-250), 500, 128); //? make a smooth hill with a center of 250,250 of radius 500 and height 128

        // random noise
        RandomNoise rndNoise; // rndNoise(seed, 5); // add some random noise //? with a (range) of 5 

        // SimplexNoise
        SimplexNoise smpNoise; //smpNoise(seed);
        

        


    public:
        HeightGenerator(int seed=21, float MIN_HEIGHT=0, float MAX_HEIGHT=128) :
            MIN_HEIGHT(MIN_HEIGHT),
            MAX_HEIGHT(MAX_HEIGHT),  
            smoothHill(seed, glm::vec2(250,-250), 500, 128), //use direct initialization instead of copy in the body cause its faster
            rndNoise(seed, 5),
            smpNoise(seed) {

                //HeighGenerator made
            };

        //TODO make an updater for noise functions


        float calculateHeight(float x, float z);

        //? make a function to give heightMap without storing it and make it virtual so inherited generators can override it
        // a hash map for each x,z pair.
        // std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;
        // makes an unordered map with a pair of floats as a key and a float as a value
        // dont forget use make_PAIR whe entering keys
        virtual std::unordered_map<std::pair<float,float>, float, FloatPairHash> makeHeightMap (std::vector<glm::vec3> positions, int seed=21);


};


#endif