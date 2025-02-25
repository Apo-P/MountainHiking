#include "objectPlacer.hpp"

// static member must be defined in cpp to avoid linker error
NoiseFunction* PoissonDiscSampling::noiseFunc = new SimplexNoise();


bool PoissonDiscSampling::IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionSize, float cellSize, float radius,
    const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid) {

    // if candidate x,z is inside sampleRegion
    if (candidate.x >= 0 && candidate.x < sampleRegionSize.x && candidate.y >= 0 && candidate.y < sampleRegionSize.y) {
        // finds its cell
        int cellX = static_cast<int>(candidate.x / cellSize);
        int cellY = static_cast<int>(candidate.y / cellSize);

        // find neighboring cells
        int searchStartX = std::max(0, cellX - 2);
        int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size()) - 1);
        int searchStartY = std::max(0, cellY - 2);
        int searchEndY = std::min(cellY + 2, static_cast<int>(grid[0].size()) - 1);

        // search neighboring cells
        for (int x = searchStartX; x <= searchEndX; ++x) {
            for (int y = searchStartY; y <= searchEndY; ++y) {
                // get point index from grid
                int pointIndex = grid[x][y] - 1;
                // if gridcell has an index
                if (pointIndex > -1) {
                    // calculate distance
                    float candidateDst = glm::length(candidate - points[pointIndex]);

                    // calculate a minumum distance (which is the minumum radius)
                    float minDistance = radius;

                    // if candidate distance from the point is less than minimum distance
                    if (candidateDst < minDistance) {
                        // discard candidate
                        return false;
                    }
                }
            }
        }
        // if it passed all checks return true
        return true;
    }

    // discard candidate if it is outside boundaries
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


glm::vec2 PoissonDiscSampling::spawnNewPoint(const glm::vec2 &spawnPointPosition, float radius){

    // Random number generation setup
    //TODO add this to class
    //! could use a seed here
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // calculate min max radius
    float min = radius;
    float max = radius * 2.0f;

    //get random angle
    float angle = dist(rng) * 2.0f * M_PI;

    // get random distance
    float distance = (dist(rng) * (max - min)) + min;

    // calculate direction
    glm::vec2 dir(std::sin(angle), std::cos(angle));

    // calculate new candidate
    glm::vec2 candidate = spawnPointPosition + dir * distance;
    

    return candidate;
};

bool PoissonDiscSampling::IsVariableValid(const std::shared_ptr<PoisonPoint> candidate, const glm::vec2 &sampleRegionStartPosition, const glm::vec2 &sampleRegionSize, float cellSize,
                                          const std::vector<std::vector<GridCell>> grid)
{

    glm::vec2 candidatePos = candidate.get()->position;
    float candidateRadius = candidate.get()->radius;

    
    // int cellRadius = std::ceil( candidateRadius / cellSize);

    // if candidate x,z is inside sampleRegion
    if (candidatePos.x >= sampleRegionStartPosition.x && candidatePos.x < sampleRegionSize.x && candidatePos.y >= sampleRegionStartPosition.y && candidatePos.y < sampleRegionSize.y) {

        // finds its cell
        int cellX = static_cast<int>(candidatePos.x / cellSize);
        int cellY = static_cast<int>(candidatePos.y / cellSize);

        // centerIndex is where candidate is


        // find neighboring cells
        int searchStartX = std::max(0, cellX - 2);
        int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size()) - 1);
        int searchStartY = std::max(0, cellY - 2);
        int searchEndY = std::min(cellY + 2, static_cast<int>(grid[0].size()) - 1);



        // Check if the circle is too close to any in the prospective cell and the neighboring cells.
        // search neighboring cells
        for (int x = searchStartX; x <= searchEndX; ++x)
        {
            for (int y = searchStartY; y <= searchEndY; ++y)
            {

                // if candidate intersects the grid square AND intersects something within that square
                // then discard it

                // we check the collision now cause the radius could be to small to worry about other points
                glm::vec2 gridCellPos (x * cellSize, y * cellSize);

                if (boxCircleCollision(gridCellPos, cellSize, cellSize, candidatePos, candidateRadius))
                {

                    GridCell GridCell = grid[x][y];

                    // check if we intersect a point inside grid the cell
                    for(auto& point : GridCell.sharedPoints) {

                        float candidateDst = glm::length(candidatePos - point.get()->position);

                        // if candidate distance from the point is less than minimum distance
                        if (candidateDst < candidateRadius) {
                            // discard candidate
                            return false;
                    }

                    }
                }
            }
        }

        return true;
    }

    // discard candidate if it is outside boundaries
    return false;

};



std::vector<glm::vec2> PoissonDiscSampling::GenerateVariablePoints(const glm::vec2 &sampleRegionStartPosition, const glm::vec2 &sampleRegionSize, float maxRadius, float minRadius, int numSamplesBeforeRejection) {


    // get cell size based on spawn radius
    // now based on min and max radius
    float cellSize = ((minRadius + maxRadius) * 0.5f) / std::sqrt(2.0f);

    // calculate grid width and height
    int gridWidth = static_cast<int>(std::ceil(sampleRegionSize.x / cellSize));
    int gridHeight = static_cast<int>(std::ceil(sampleRegionSize.y / cellSize));

    

    

    // grid stores an grid cell (initialize gird)
    std::vector<std::vector<GridCell>> grid(gridWidth, std::vector<GridCell>(gridHeight));

    // points store's the points we create
    std::vector<std::shared_ptr<PoisonPoint>> points; 
    // spawn points are the points from which we try to create a point
    std::vector<std::shared_ptr<PoisonPoint>> spawnPoints;

    // start at middle
    std::shared_ptr<PoisonPoint>middle = std::make_shared<PoisonPoint>(sampleRegionStartPosition + (sampleRegionSize / 2.0f), maxRadius);
    spawnPoints.push_back(middle);
    
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
        std::shared_ptr<PoisonPoint> spawnPoint = spawnPoints[spawnIndex];

        // if we found a point
        bool candidateAccepted = false;

        // try to find an accepted spawn point within samples allowed
        for (int i = 0; i < numSamplesBeforeRejection; ++i) {

            // get spawn point position
            glm::vec2 spawnCentre = spawnPoint.get()->position;

            // calculate random radius using simplex noise for spawnPoint center
            float radius = minRadius + noiseFunc->calculateRadius(spawnCentre.x, spawnCentre.y) * (maxRadius - minRadius);

            // spawn a new point within radius;
            glm::vec2 candidatePosition = spawnNewPoint(spawnCentre, radius);

            // make new candidate
            std::shared_ptr<PoisonPoint> candidate = std::make_shared<PoisonPoint>(candidatePosition, radius);
            
            // if candidate is a valid candidate
            if (IsVariableValid(candidate, sampleRegionStartPosition, sampleRegionSize, cellSize, grid)) {

                // add it to points list
                points.push_back(candidate);

                // add it as a new spawn point
                spawnPoints.push_back(candidate);

                // store which cell it is stored at
                int ax = static_cast<int>(candidatePosition.x / cellSize);
                int ay = static_cast<int>(candidatePosition.y / cellSize);
                // if (ax>2 | ay>2) {
                //     // throw std::range_error("asd");
                // }
                // int cp = ax+1;
                grid[ax][ay].sharedPoints.push_back(candidate);

                // store we found a valid candidate to add to points
                candidateAccepted = true;
                //end loop
                break;
            }
        }

        // if we couldn't find a valid candidate
        if (!candidateAccepted) {
            // remove the current spawnpoint
            spawnPoints.erase(spawnPoints.begin() + spawnIndex);
        }
    }

    // make a final list of position vectors
    std::vector<glm::vec2> finalPoints;

    // could be a bit more efficient to change storage structure to skip this

    for(auto& point : points) {
        finalPoints.push_back(point->position);
    }

    // return point positions
    return finalPoints;


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
