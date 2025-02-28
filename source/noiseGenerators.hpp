#ifndef NOISEGENERATORS_HPP
#define NOISEGENERATORS_HPP

#include <common.hpp>

#include <OpenSimplex2S.hpp>

class NoiseFunction {

    public:
        // initializer (dont forget seed)
        NoiseFunction(){};

        // Calculate noise and return max noise value - Interface 
        virtual float calculateHeight(float x, float z, int maxHeight=128){};

        virtual float calculateRadius(float x, float z, float maxRadius=3){};


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

        // Updates range
        void updateValues(int range) { this->distribution = std::uniform_int_distribution<>(0, range); }

        float calculateHeight(float x, float z, int maxHeight=1) override;


};

class SmoothHill : public NoiseFunction {

    private:
        glm::vec2 hillCenter;

        float hillRadius;

        int hillHeight;

        

        // Define the smoothstep function to smooth step x Between edge0 and edge1
        float smoothstep(float edge0, float edge1, float x) ;


        float noise(float x, float z) ;
    public:

        // create random noise within given range
        SmoothHill(int seed=21, glm::vec2 hillCenter=glm::vec2(250,-250), float hillRadius = 250, float hillHeight=128) :
            hillCenter(hillCenter),
            hillRadius(hillRadius),
            hillHeight(hillHeight)
            {  
            
            };


        // Updates hillCenter, hillRadius, maxHeight
        void updateValues(glm::vec2 hillCenter=glm::vec2(250,-250), float hillRadius = 250, float hillHeight=128) { 
            this->hillCenter = hillCenter;
            this->hillRadius = hillRadius;
            this->hillHeight = hillHeight;
        }


        float calculateHeight(float x, float z, int maxHeight=1) override ;


};

class SimplexNoise : public NoiseFunction {

    private:
    
        OpenSimplex2S simplex;

        // level of detail
        //!not used
        int lodLevel;

        // how often pattern repeats (also makes terrain smoother)
        float noiseScale;
        // how much every octave contributes (if positive every octave contribute's more cause of higher frequancy) (range -3-5)
        float persistence;
        // Number of octaves for terrain detail (the more the more jagged) (range 1-10)
        int octaves;
        // flatten terrain so we have flatter vallies and sharper peaks (range 1-10) (usually 3)
        float exponentiation;
        // how fast to increase freq (usually 2)
        float lacunarity = 2;

        // chunk starting x,z used for offset
        //!not used
        float chunkX,chunkZ;

        float noise(float x, float z);

    public:

    SimplexNoise(int seed=21, float noiseScale=200, float persistence = 0.5, int octaves=4, float exponentiation=3) : 
        simplex(seed),  noiseScale(noiseScale), persistence(persistence), octaves(octaves), exponentiation(exponentiation) { //initialize open simplex
 
    
        //configure

        // Calculate the LOD level based on the distance
        // int lodLevel = calculateLODLevel(distance);

        // Use the appropriate scale based on the LOD level
        // float noiseScale = getNoiseScaleForLOD(lodLevel);

        

    };

    // change parameter values
    void updateValues(float noiseScale=500, float persistence = 1, int octaves=4, float exponentiation=2) {
        this->noiseScale = noiseScale;
        this->persistence = persistence;
        this->octaves = octaves;
        this->exponentiation = exponentiation;
    }

    float calculateHeight(float x, float z, int maxHeight=128);

    float calculateRadius(float x, float z, float maxRadius=3);

};

#endif