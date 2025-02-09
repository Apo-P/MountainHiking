#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <common.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <heightGenerator.hpp>


class TerrainChunk {

    private:

        // chunks world start position
        float chunkX, chunkZ;
        // chunk size
        int chunkSize;
        // how many segments/faces will it have
        int resolution;

        //! be carefull chunks model matrix is 1 as its position is in worldspace coords

        HeightGenerator& heightGenerator;

        std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;



        void createMesh();

        std::vector<VertexData> createVertices();


    public:
        std::shared_ptr<Mesh> mesh;


        TerrainChunk( HeightGenerator& heightGenerator, float chunkX=0, float chunkZ=0, int chunkSize=500, int resolution=128); // add LOD level later

        void generateChunk(int seed=21);

};

#endif