#ifndef OBJECTPLACER_HPP
#define OBJECTPLACER_HPP

#include "common.hpp"

/// @brief Poisson disc sampling using a square grid to make it faster
class PoissonDiscSampling {
    public:        

        // we make a grid of cell size radius to have an easier time to check points

        //TODO:allow offset for x,z

        /// @brief generates spawn points with a minimum radius between them inside the region specified
        /// @param radius minumum radius between points
        /// @param sampleRegionSize Region size to spawn objects
        /// @param numSamplesBeforeRejection how many time sto try to randomly find a point before rejection
        /// @return vector of points within region
        static std::vector<glm::vec2> GeneratePoints(float radius, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection = 30);
    
    private:
        // function to check if candidate should be accepted (meets spawn criteria)

        /// @brief Function to validate if candidate should be accepted (meets spawn criteria)
        /// @param candidate candidate pos
        /// @param sampleRegionSize region size
        /// @param cellSize size of a cell
        /// @param radius minimum radius
        /// @param points all current points
        /// @param grid grid of points
        /// @return returns true or false
        static bool IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionSize, float cellSize, float radius,
                            const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid);
    };

#endif