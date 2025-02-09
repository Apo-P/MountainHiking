#ifndef TERRAINCHUNKMANAGER_HPP
#define TERRAINCHUNKMANAGER_HPP

#include <terrainChunk.hpp>

class ChunkManager {

    private:
        // height generator for chunks
        std::shared_ptr<HeightGenerator> generator;

        int seed;

        // the half size of the grid to make (i.e if you want a grid of 9,9 this would be 3 to make from -3 to)
        int GridSizeHalf;

        // will make a grid of gridSize, of terrain chunks
        void initTerrain();

        void addChunk(int x, int z);

    public:

        // a map of all the chunks in a grid
        std::unordered_map<std::pair<int,int>, std::shared_ptr<TerrainChunk>, IntPairHash> chunks;


        ChunkManager(int seed=21, int GridSizeHalf=3) :
            seed(seed),
            GridSizeHalf(GridSizeHalf) {

                //init heightGenerator
                float minHeight = 0;
                float maxHeight = 128;
                generator = std::make_shared<HeightGenerator>(minHeight, maxHeight);

                //init terrain
                initTerrain();

            };

        

};


#endif