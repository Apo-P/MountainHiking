#ifndef NOISEGENERATORS_HPP
#define NOISEGENERATORS_HPP

#include <common.hpp>

#include <OpenSimplex2S.hpp>

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

        float noise();

    public:
        // create random noise within given range
        RandomNoise(int seed, int range) :
        seededGenerator(seed),
        distribution(0, range)  {  
        };

        float calculateNoise(float x, float z) override;


};

class SmoothHill : public NoiseFunction {

    private:
        glm::vec2 hillCenter;

        float hillRadius;

        int maxHeight;

        

        // Define the smoothstep function to smooth step x Between edge0 and edge1
        float smoothstep(float edge0, float edge1, float x) ;


        float noise(float x, float z) ;
    public:

        // create random noise within given range
        SmoothHill(int seed=21, glm::vec2 hillCenter=glm::vec2(250,-250), float hillRadius = 250, float maxHeight=128) :
            hillCenter(hillCenter),
            hillRadius(hillRadius),
            maxHeight(maxHeight)
            {  
            
            };

        float calculateNoise(float x, float z) override ;


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

        float noise(float x, float z);

    public:

    SimplexNoise(int seed=21) : 
        simplex(seed) { //initialize open simplex
 
    
        //configure

        // Calculate the LOD level based on the distance
        // int lodLevel = calculateLODLevel(distance);

        // Use the appropriate scale based on the LOD level
        // float noiseScale = getNoiseScaleForLOD(lodLevel);

        

    };

    float calculateNoise(float x, float z);

};

#endif