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

        // x axis
        float segment_width;
        // z axis
        float segment_length;

        //! be carefull chunks model matrix is 1 as its position is in worldspace coords

        HeightGenerator& heightGenerator;

        std::unordered_map<std::pair<float,float>, float, FloatPairHash> heightMap;


        



        void createMesh();

        std::vector<VertexData> createVertices();


    public:
        std::shared_ptr<Mesh> mesh;


        TerrainChunk( HeightGenerator& heightGenerator, float chunkX=0, float chunkZ=0, int chunkSize=500, int resolution=128); // add LOD level later

        void generateChunk(int seed=21);

        // will give the approximate height for a point
        float approximateHeight(glm::vec3 pos) {

            // find which face is on

            // x mod width
            float resx = std::floor(pos.x/segment_width);
            resx = resx * segment_width;
            float resz = - std::floor(abs(pos.z)/segment_length);
            resz = resz * segment_length;

            float height;

            // gets only left corner height
            // height = heightMap[std::make_pair(resx, resz)];

            // get average of 4 corners
            height = heightMap[std::make_pair(resx, resz)] + heightMap[std::make_pair(resx+segment_width, resz)] + heightMap[std::make_pair(resx, resz+segment_length)] + heightMap[std::make_pair(resx+segment_width, resz+segment_length)];
            height = height / 4;

            return height;

        };

};

#endif