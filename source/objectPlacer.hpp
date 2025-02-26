#ifndef OBJECTPLACER_HPP
#define OBJECTPLACER_HPP

#include "common.hpp"
#include "noiseGenerators.hpp"


// A uniform Poisson disc sampler to use to place objects
class PoissonDiscSampling {

    private:
        // function to check if candidate should be accepted (meets spawn criteria)

        // random generators
        //! Dont forget to use seededGenerator.seed
        static std::mt19937 seededGenerator;
        static std::uniform_real_distribution<float> floatDistribution;

        /// @brief Function to validate if candidate should be accepted (meets spawn criteria)
        /// @param candidate candidate pos
        /// @param sampleRegionStartPos the starting x,z of ot region
        /// @param sampleRegionSize region size
        /// @param cellSize size of a cell
        /// @param radius radius of object (will determine the minimum distance 2 objects should be apart)
        /// @param points all current points
        /// @param grid grid of points
        /// @return returns true or false
        static bool IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, float cellSize, float radius,
                            const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid);


    public:

        // initialize seeders
        static void initialize(int seed=21, int min = 0, int max = 1) {
            
            seededGenerator.seed(seed); 
            floatDistribution = std::uniform_real_distribution<float>(min, max);
        
        }


        /// @brief generates spawn points with a minimum radius between them inside the region specified
        /// @param radius minumum radius between points
        /// @param sampleRegionStartPos the starting x,z of ot region
        /// @param sampleRegionSize Region size to spawn objects
        /// @param numSamplesBeforeRejection how many time sto try to randomly find a point before rejection
        /// @return vector of points within region    
        static std::vector<glm::vec2> GeneratePoints(float radius, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection = 30);

};


class VariablePoissonDiscSampling {
    private:
        // function to check if candidate should be accepted (meets spawn criteria)

        // random generators
        //! Dont forget to use seededGenerator.seed
        static std::mt19937 seededGenerator;
        static std::uniform_real_distribution<float> floatDistribution;

        // noise function (opensimplex)
        static NoiseFunction* noiseFunc;

        // A simple struct that store points position and radius
        struct PoisonPoint
        {
            glm::vec2 position;
            float radius;
    
            PoisonPoint(glm::vec2 position,float radius) : position(position), radius(radius) {};
        };

        // A simple grid cell store its staring position and a list of its contained points
        struct GridCell
        {

            glm::vec2 position;
    
            // a list of shared points 
            // that point to the list of all the points
            std::vector<std::shared_ptr<PoisonPoint>> pointsNeighbourhood;


        };

        /// @brief returns if candidate is inside defined region
        static bool isInsideRegion(const glm::vec2 &candidatePos, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize);

        /// @brief returns if candidate collides with a gridsquare
        static bool collidesWithGridSquare(const glm::vec2 &gridSquarePos, const float &cellSize, const glm::vec2 &candidatePos, const float &candidateRadius);

        /// @brief returns if candidate is colliding with another point 
        static bool collidesWithPoint(const glm::vec2 &pointPos, const float &pointRadius, const glm::vec2 &candidatePos, const float &candidateRadius);

        /// @brief returns if candidate is inside neighbourhood of any point inside neighbouring cells (neighbourhood is the collection of points in a cell)
        static bool isInNeighbourhood(const glm::vec2 &candidatePos, const float &candidateRadius, const std::vector<std::shared_ptr<PoisonPoint>> &neighbourhood);

        /// @brief will spawn a random new point within given radius
        static glm::vec2 spawnNewPoint(const glm::vec2 &spawnPointPosition, float radius);

        /// @brief checks if candidate is a valid candidate
        static bool isValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, float cellSize, float candidateRadius,
                    const std::vector<std::vector<GridCell>> &grid);


    public:

        // initialize seeders
        static void initialize(int seed=21, int min = 0, int max = 1) {
            
            seededGenerator.seed(seed); 
            floatDistribution = std::uniform_real_distribution<float>(min, max);
            noiseFunc = new SimplexNoise(seed);
        
        }

        // min radius is the min radius of the obj so they dont collide (also can be modified to decrease run time)
        /// @brief generates a number of varying points
        static std::vector<glm::vec2> GeneratePoints(float minumumRadius, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection = 4);


   
};



#endif