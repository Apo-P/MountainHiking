#include "objectPlacer.hpp"


bool PoissonDiscSampling::IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionSize, float cellSize, float radius,
    const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid) {

    // if candidate x,z is inside sampleRegion
    if (candidate.x >= 0 && candidate.x < sampleRegionSize.x && candidate.y >= 0 && candidate.y < sampleRegionSize.y) {
        // finds its cell
        int cellX = static_cast<int>(candidate.x / cellSize);
        int cellY = static_cast<int>(candidate.y / cellSize);

        // find neighbouring cells
        int searchStartX = std::max(0, cellX - 2);
        int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size()) - 1);
        int searchStartY = std::max(0, cellY - 2);
        int searchEndY = std::min(cellY + 2, static_cast<int>(grid[0].size()) - 1);

        // search neighbouring cells
        for (int x = searchStartX; x <= searchEndX; ++x) {
            for (int y = searchStartY; y <= searchEndY; ++y) {
                // get point index from grid
                int pointIndex = grid[x][y] - 1;
                // if gridcell has an index
                if (pointIndex > -1) {
                    // calculate distance
                    float sqrDst = glm::length(candidate - points[pointIndex]);
                    //if distance is less than radius
                    if (sqrDst < radius) {
                        return false;
                    }
                }
            }
        }
            // if it passed all checks return true
            return true;
    }

    return false;
}



std::vector<glm::vec2> PoissonDiscSampling::GeneratePoints(float radius,const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection) {
    
    // get cell size based on spawn radius
    float cellSize = radius / std::sqrt(2.0f);

    // calculate grid width and height
    int gridWidth = static_cast<int>(std::ceil(sampleRegionSize.x / cellSize));
    int gridHeight = static_cast<int>(std::ceil(sampleRegionSize.y / cellSize));

    // grid stores an index to the points list (so we know which which point is at that cell, IMPORTANT 0 means no point!)
    std::vector<std::vector<int>> grid(gridWidth, std::vector<int>(gridHeight, -1));

    // points store's the points we create
    std::vector<glm::vec2> points; 
    // spawn points are the pointe from which we try to create a point
    std::vector<glm::vec2> spawnPoints;

    // start at middle
    spawnPoints.push_back(sampleRegionSize / 2.0f);
    
    // Random number generation setup
    //! could use a seed here
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // while we have spawn points
    while (!spawnPoints.empty()) {
        // pick random spawn point
        int spawnIndex = static_cast<int>(dist(rng) * spawnPoints.size());

        // get spawn center
        glm::vec2 spawnCentre = spawnPoints[spawnIndex];

        // if we found a point
        bool candidateAccepted = false;

        // try to find an accepted spawn point within samples allowed
        for (int i = 0; i < numSamplesBeforeRejection; ++i) {
            //get random angle
            float angle = dist(rng) * 2.0f * M_PI;
            glm::vec2 dir(std::sin(angle), std::cos(angle));
            // calculate new candidate
            glm::vec2 candidate = spawnCentre + dir * (dist(rng) * radius + radius);
            
            // if candidate is a valid candidate
            if (IsValid(candidate, sampleRegionSize, cellSize, radius, points, grid)) {
                // add it to points list
                points.push_back(candidate);
                // add it as a new spawn point
                spawnPoints.push_back(candidate);
                // store which cell it is stored at
                grid[static_cast<int>(candidate.x / cellSize)][static_cast<int>(candidate.y / cellSize)] = points.size();

                // store we found a valid candidate to add to points
                candidateAccepted = true;
                //end loop
                break;
            }
        }

        // if we couldnt find a valid andidate
        if (!candidateAccepted) {
            // remove the current spawnpoint
            spawnPoints.erase(spawnPoints.begin() + spawnIndex);
        }
    }

    // return final list
    return points;
}


// int main() {
//     // Define parameters for the Poisson Disc Sampling
//     float radius = 2.0f;
//     glm::vec2 sampleRegionSize(5.0f, 5.0f); // Size of the area to sample in

//     // Generate points using Poisson Disc Sampling
//     auto points = PoissonDiscSampling::GeneratePoints(radius, sampleRegionSize);

//     // Print the generated points
//     std::cout << "Generated " << points.size() << " points:" << std::endl;
//     for (const auto& point : points) {
//         std::cout << "(" << point.x << ", " << point.y << ")\n";
//     }

//     return 0;
// }
