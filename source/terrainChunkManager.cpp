#include <terrainChunkManager.hpp>

void ChunkManager::addChunk(int x, int z) {
    // std::cout << "adding chunk x:" << x << " z:" << z << std::endl;

    int chunkSize=500;
    float chunkX, chunkZ;
    int resolution = 128;

    chunkX = x * chunkSize;
    chunkZ = z * chunkSize;

    // make requested chunk
    std::shared_ptr<TerrainChunk> newChunk = std::make_shared<TerrainChunk>(*generator.get(), chunkX, chunkZ, chunkSize, resolution);

    // here we could add special generators or something based on which grid positions it is
    // so we could make a biome system
    // We always should have a big mountain in the middle of the world

    // generate the chunk
    //? should this should be done at chunk call?
    newChunk.get()->generateChunk();

    // calculate edges of chunk to change the as required to improve performance


    // add chunk to list
    chunks[std::make_pair(x,z)] = newChunk;

}

void ChunkManager::initTerrain() {

    // initialize anything else needed for generation
    std::cout << "Generating Terrain!"<< std::endl;

    // make grid of chunks

    for (int x = -GridSizeHalf; x <= GridSizeHalf; x++) {
        for (int z = -GridSizeHalf; z <= GridSizeHalf; z++) {
            addChunk(x,z);
        }
    }

}

