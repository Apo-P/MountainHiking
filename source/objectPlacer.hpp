#ifndef OBJECTPLACER_HPP
#define OBJECTPLACER_HPP

#include "common.hpp"
#include "noiseGenerators.hpp"

/// @brief Poisson disc sampling using a square grid to make it faster
class PoissonDiscSampling {
    public:        

        // we make a grid of cell size radius to have an easier time to check points

        // noise function (opensimplex)
        static NoiseFunction* noiseFunc;

        //TODO:allow offset for x,z

        /// @brief generates spawn points with a minimum radius between them inside the region specified
        /// @param radius minumum radius between points
        /// @param sampleRegionSize Region size to spawn objects
        /// @param numSamplesBeforeRejection how many time sto try to randomly find a point before rejection
        /// @return vector of points within region
        static std::vector<glm::vec2> GeneratePoints(float radius, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection = 30);
    

        static glm::vec2 spawnNewPoint(const glm::vec2 &spawnPointPosition, float radius);

        static std::vector<glm::vec2> GenerateVariablePoints(const glm::vec2 &sampleRegionStartPosition, const glm::vec2 &sampleRegionSize, float maxRadius=4, float minRadius =2, int numSamplesBeforeRejection = 30);

    private:
        // function to check if candidate should be accepted (meets spawn criteria)

        /// @brief Function to validate if candidate should be accepted (meets spawn criteria)
        /// @param candidate candidate pos
        /// @param sampleRegionSize region size
        /// @param cellSize size of a cell
        /// @param radius radius of object (will determine the minimum distance 2 objects should be apart)
        /// @param points all current points
        /// @param grid grid of points
        /// @return returns true or false
        static bool IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionSize, float cellSize, float radius,
                            const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid);



        struct PoisonPoint
        {
            glm::vec2 position;
            float radius;
    
            PoisonPoint(glm::vec2 position,float radius) : position(position), radius(radius) {};
        };
    
        struct GridCell
        {
            
            // a list the indexes of the points in the cell
            // (so we know which which point is at that cell, IMPORTANT -1 means no point!)
            std::vector<int>points = std::vector<int>(1,-1);
    
            // a list of shared points 
            // because a point can be in multiple cells
            std::vector<std::shared_ptr<PoisonPoint>> sharedPoints;


        };

        static bool boxCircleCollision(glm::vec2 boxPos, float boxWidth, float boxHeight, glm::vec2 circlePos, float circleRadius) {


            // Find the closest point on the box to the circle's center
            float closestX = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + boxWidth));
            float closestY = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + boxHeight));

            glm::vec2 testPoint(closestX, closestY);

            // Calculate the distance from the circle's center to this closest point
            float distance = glm::length(testPoint- circlePos);

            // return true or false if distance is greater that circle radius (point is in the circle)
            return distance <= circleRadius;



        };

        static bool IsVariableValid(const std::shared_ptr<PoisonPoint> candidate, const glm::vec2 &sampleRegionStartPosition, const glm::vec2 &sampleRegionSize, float cellSize,
            const std::vector<std::vector<GridCell>> grid);
        
                            
    };

#endif