#include "objectPlacer.hpp"

// helper functions

// this should be inside another header like "simple_collisions.hpp"
// returns if circle collided with a square
bool boxCircleCollision(glm::vec2 boxPos, float boxWidth, float boxHeight, glm::vec2 circlePos, float circleRadius) {


    // Find the closest point on the box to the circle's center
    float closestX = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + boxWidth));
    float closestY = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + boxHeight));

    glm::vec2 testPoint(closestX, closestY);

    // Calculate the distance from the circle's center to this closest point
    float distance = glm::length(testPoint- circlePos);

    // return true or false if distance is smaller than circle radius (point is in the circle)
    return distance <= circleRadius;



};

bool circleCircleCollision(const glm::vec2 &circle1Pos, const float &circle1Radius, const glm::vec2 &circle2Pos, const float &circle2Radius) {


    // Calculate the distance between centers
    float distance = glm::length(circle1Pos - circle2Pos);

    // calculate combined radius
    float combinedRadius = circle1Radius + circle2Radius;

    // return true or false if distance is smaller that combined circle radius
    return distance <= combinedRadius;

}



//Constant radius poisson

// static member must be defined in cpp to avoid linker error
// allocate memory for the static members (this need to be done because inside the class no memory is allocated they are only defined)
//! Dont forget to initialize them later!
std::mt19937 PoissonDiscSampling::seededGenerator;
std::uniform_real_distribution<float> PoissonDiscSampling::floatDistribution;

bool PoissonDiscSampling::IsValid(const glm::vec2 &candidate, const glm::vec2 &sampleRegionStartPos,  const glm::vec2 &sampleRegionSize, float cellSize, float radius,
    const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid) {

    // if candidate x,z is inside sampleRegion
    if (candidate.x >= sampleRegionStartPos.x && candidate.x < sampleRegionStartPos.x + sampleRegionSize.x && candidate.y >= sampleRegionStartPos.y && candidate.y < sampleRegionStartPos.y + sampleRegionSize.y) {
        // finds its cell
        int cellX = static_cast<int>((candidate.x - sampleRegionStartPos.x) / cellSize);
        int cellY = static_cast<int>((candidate.y - sampleRegionStartPos.y) / cellSize);

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


std::vector<glm::vec2> PoissonDiscSampling::GeneratePoints(float radius, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection) {
    
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
    glm::vec2 middle =sampleRegionStartPos + (sampleRegionSize / 2.0f);
    spawnPoints.push_back(middle);

    // Initialize random generators
    initialize(21);

    // while we have spawn points
    while (!spawnPoints.empty()) {
        // pick random spawn point
        int spawnIndex = static_cast<int>(floatDistribution(seededGenerator) * spawnPoints.size());

        // get spawn center
        glm::vec2 spawnCentre = spawnPoints[spawnIndex];

        // if we found a point
        bool candidateAccepted = false;

        // try to find an accepted spawn point within samples allowed
        for (int i = 0; i < numSamplesBeforeRejection; ++i) {
            //get random angle
            float angle = floatDistribution(seededGenerator) * 2.0f * M_PI;
            glm::vec2 dir(std::sin(angle), std::cos(angle));
            // calculate new candidate
            glm::vec2 candidate = spawnCentre + dir * (floatDistribution(seededGenerator) * radius + radius);
            
            // if candidate is a valid candidate
            if (IsValid(candidate, sampleRegionStartPos, sampleRegionSize, cellSize, radius, points, grid)) {
                // add it to points list
                points.push_back(candidate);
                // add it as a new spawn point
                spawnPoints.push_back(candidate);
                // store which cell it is stored at
                grid[static_cast<int>((candidate.x -sampleRegionStartPos.x)  / cellSize)][static_cast<int>((candidate.y - sampleRegionStartPos.y) / cellSize)] = points.size();

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



// Variable radius poisson

// static member must be defined in cpp to avoid linker error
//! change later
NoiseFunction* VariablePoissonDiscSampling::noiseFunc = new SimplexNoise(21);

// allocate memory for the static members (this need to be done because inside the class no memory is allocated they are only defined)
//! Dont forget to initialize them later!
std::mt19937 VariablePoissonDiscSampling::seededGenerator;
std::uniform_real_distribution<float> VariablePoissonDiscSampling::floatDistribution;

bool VariablePoissonDiscSampling::isInsideRegion(const glm::vec2 &candidate, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize){

    return (candidate.x >= sampleRegionStartPos.x && candidate.x < sampleRegionStartPos.x + sampleRegionSize.x && candidate.y >= sampleRegionStartPos.y && candidate.y < sampleRegionStartPos.y + sampleRegionSize.y) ;
}

glm::vec2 VariablePoissonDiscSampling::spawnNewPoint(const glm::vec2 &spawnPointPosition, float radius){
    // calculate min max radius
    float min = radius;
    float max = 2.0f * radius;

    //get random angle
    float angle = floatDistribution(seededGenerator) * 2.0f * M_PI;

    // get random distance
    float distance = (floatDistribution(seededGenerator) * (max - min)) + min;

    // calculate direction
    glm::vec2 dir(std::sin(angle), std::cos(angle));

    // calculate new candidate
    glm::vec2 candidate = spawnPointPosition + dir * distance;
    

    return candidate;
}

bool VariablePoissonDiscSampling::collidesWithGridSquare(const glm::vec2 &gridSquarePos, const float &cellSize, const glm::vec2 &candidatePos, const float &candidateRadius){
    return boxCircleCollision(gridSquarePos, cellSize, cellSize, candidatePos, candidateRadius);
}

bool VariablePoissonDiscSampling::collidesWithPoint(const glm::vec2 &pointPos, const float &pointRadius, const glm::vec2 &candidatePos, const float &candidateRadius){
    return circleCircleCollision(pointPos, pointRadius, candidatePos, candidateRadius);
}

bool VariablePoissonDiscSampling::isInNeighbourhood(const glm::vec2 &candidatePos, const float &candidateRadius, const std::vector<std::shared_ptr<PoisonPoint>> &neighbourhood){
    
    // check all points
    for(auto& point : neighbourhood){
        
        // if candidate collides with point return true
        if (collidesWithPoint(point.get()->position, point.get()->radius, candidatePos, candidateRadius)) return true;


    }

    // if we could find a collision
    return true;
}

bool VariablePoissonDiscSampling::isValid(const glm::vec2 &candidatePos, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, float cellSize, float candidateRadius,
    const std::vector<std::vector<GridCell>> &grid){

    // if candidate x,z is inside sampleRegion
    if(isInsideRegion(candidatePos, sampleRegionStartPos, sampleRegionSize)) {

        // finds its cell
        int cellX = static_cast<int>((candidatePos.x - sampleRegionStartPos.x) / cellSize);
        int cellY = static_cast<int>((candidatePos.y - sampleRegionStartPos.y) / cellSize);

        // find neighboring cells
        int searchStartX = std::max(0, cellX - 2);
        int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size()) - 1);
        int searchStartY = std::max(0, cellY - 2);
        int searchEndY = std::min(cellY + 2, static_cast<int>(grid[0].size()) - 1);

        // search neighboring cells
        for (int x = searchStartX; x <= searchEndX; ++x) {
            for (int y = searchStartY; y <= searchEndY; ++y) {

                // get current grid cell
                GridCell gridCell = grid[x][y];


                // check if it collides with square (if not then no need to check inside)
                if(!collidesWithGridSquare(gridCell.position, cellSize, candidatePos, candidateRadius)) {
                    continue;
                }
                // if grid has no points inside skip cell
                if (gridCell.pointsNeighbourhood.empty()){
                    continue;
                }


                // check if it collides with any other point inside the cell
                if (isInNeighbourhood(candidatePos , candidateRadius, gridCell.pointsNeighbourhood)){
                    return false;
                }

                //! Can add more checks here


            }
        }
    
        // if it passed all checks return true
        return true;

    }

    // discard candidate if it is outside boundaries
    return false;

}



std::vector<glm::vec2> VariablePoissonDiscSampling::GeneratePoints(float minumumRadius, const glm::vec2 &sampleRegionStartPos, const glm::vec2 &sampleRegionSize, int numSamplesBeforeRejection) {


    //TODO IMPORTANT MAKE WE INCREASE TOWARDS -Z!!!!!

    // how many time we can make radius bigger (used in noise calculations)
    float RADIUS_VARIABILITY = 4;
    // maxRadius doesn't play much role but help make the grid size 
    // (normally it is calculated by max radius that could happen for noise function, Say we get a point of radius 20 and its the highest, the 20 should be the maxRadius)
    float maxRadius = RADIUS_VARIABILITY * minumumRadius;

    // get cell size based on maximum radius
    float cellSize = maxRadius / std::sqrt(2.0f);

    // calculate grid width and height
    //! Important we assume region is square throughout the program for now!
    int gridWidth = static_cast<int>(std::ceil(sampleRegionSize.x / cellSize));
    int gridHeight = static_cast<int>(std::ceil(sampleRegionSize.y / cellSize));


    // grid stores an 2Darray grid cell 
    // initialize grid of gridWidth and gridHeight
    std::vector<std::vector<GridCell>> grid(gridWidth, std::vector<GridCell>(gridHeight));

    // initialize grid positions
    for (int x=0; x < gridWidth; x++){
        for(int y=0; y < gridHeight; y++)
            grid[x][y].position = (sampleRegionStartPos + glm::vec2(x * cellSize, y * cellSize));
    }

    // points store's the points we create
    std::vector<std::shared_ptr<PoisonPoint>> points; 
    // spawn points are the points from which we try to create a point
    std::vector<std::shared_ptr<PoisonPoint>> spawnPoints;

    

    // calculate middle pos
    glm::vec2 middlePos(sampleRegionStartPos + (sampleRegionSize / 2.0f));
    // get middle radius
    //! radius should always be at least minumum and -1 minimum from maximum (so we dont exceed maximum)
    float middleRadius = minumumRadius + noiseFunc->calculateRadius(middlePos.x, middlePos.y) * minumumRadius * (RADIUS_VARIABILITY-1);

    // make middle point
    std::shared_ptr<PoisonPoint>middle = std::make_shared<PoisonPoint>(middlePos, middleRadius);
    // add it as first spawn point
    spawnPoints.push_back(middle);
    // add middle point to its cell
    int middleX = static_cast<int>((middlePos.x - sampleRegionStartPos.x) / cellSize);
    int middleY = static_cast<int>((middlePos.y - sampleRegionStartPos.y) / cellSize);
    grid[middleX][middleY].pointsNeighbourhood.push_back(middle);

    // Initialize random generators
    initialize(21);

    // while we have spawn points
    while (!spawnPoints.empty()) {

        // pick random spawn point
        int spawnIndex = static_cast<int>(floatDistribution(seededGenerator) * spawnPoints.size());

        // get spawn center
        glm::vec2 spawnCentre = spawnPoints[spawnIndex].get()->position;
        // get spawn Radius
        float spawnRadius = spawnPoints[spawnIndex].get()->radius;

        // if we found a point
        bool candidateAccepted = false;

        // try to find an accepted spawn point within samples allowed
        for (int i = 0; i < numSamplesBeforeRejection; ++i) {

            // spawn new candidate
            glm::vec2 candidatePos = spawnNewPoint(spawnCentre, spawnRadius);

            // get candidates radius
            float candidateRadius = minumumRadius + noiseFunc->calculateRadius(candidatePos.x, candidatePos.y) * minumumRadius * (RADIUS_VARIABILITY-1);

            // check if candidate is a valid candidate
            if (isValid(candidatePos, sampleRegionStartPos, sampleRegionSize, cellSize, candidateRadius, grid)) { 


                // make Candidate point
                std::shared_ptr<PoisonPoint> candidatePoint = std::make_shared<PoisonPoint>(candidatePos, candidateRadius);

                // add it to points list
                points.push_back(candidatePoint);
                // add it as a new spawn point
                spawnPoints.push_back(candidatePoint);

                // finds its cell
                int cellX = static_cast<int>((candidatePos.x - sampleRegionStartPos.x) / cellSize);
                int cellY = static_cast<int>((candidatePos.y - sampleRegionStartPos.y) / cellSize);
                // store it to its cell
                grid[cellX][cellY].pointsNeighbourhood.push_back(candidatePoint);

                // store we found a valid candidate to add to points
                candidateAccepted = true;
                //end loop
                break;
            }
        }


        // if we couldn't find a valid candidate
        if (!candidateAccepted) {
            // remove the current spawn point
            spawnPoints.erase(spawnPoints.begin() + spawnIndex);

        }

    }


    // make a final list of position vectors
    std::vector<glm::vec2> finalPoints;

    //? could it be a bit more efficient to change storage structure to skip this?
    for(auto& point : points) {
        finalPoints.push_back(point->position);
    }

    // return point positions
    return finalPoints;

}
