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

        //? maybe since we start storing the gradient as well i would be better to just use a 2d image ?
        //? we can calculate gradient in shaders i think
        //! We can calculate the gradient of the surface from the normal of the surface
        std::unordered_map<std::pair<float,float>, float, FloatPairHash> gradientMap;

        



        void createMesh();

        std::vector<VertexData> createVertices();

        // internal method to get height in specific point (not to be used for in between points) 
        // returns false = 0 if height was not found
        float getHeight(float x, float z);

        /// @brief internal method that recalculates the normals of all the vertices given (modifies them)
        /// @param vertices vertcies to be modified
        void static reCalculateNormals( std::vector<VertexData> &vertices);


    public:
        std::shared_ptr<Mesh> mesh;


        TerrainChunk( HeightGenerator& heightGenerator, float chunkX=0, float chunkZ=0, int chunkSize=500, int resolution=128); // add LOD level later


        // an update function to update a points height (returns true or false if it managed it) (doesn't update mesh!)
        bool updateHeight(float x, float z, float newValue);

        // recalculate heightmap (and updates mesh!)
        //! if heightgenerator is the same than nothing will change. Maybe pass new height generator? Or use updateing functions
        void recalculateHeight();

        // return heightgenerator so it can be modified
        HeightGenerator& getHeightGenerator() {return heightGenerator;}
        

        void generateChunk(int seed=21);

        /// @brief  Interpolates between 2 values in a grid
        /// @param X X value
        /// @param Z Z values
        /// @param X0 bottom left x
        /// @param Z0 bottom left z
        /// @param X1 top right x
        /// @param Z1 top right z
        /// @param h00 value at bottom left (x0,z0)
        /// @param h10 value at bottom right (x1,z0)
        /// @param h01 value at top left (x0,z1)
        /// @param h11 value at top right (x1,z1)
        /// @return interpolated value
        float bilinearInterpolation(
            float X, float Z,
            float X0, float Z0,
            float X1, float Z1,
            float h00, float h10, float h01, float h11) {

            // calculate normalized position of x,z between x,z 0-1
            float normX = (X - X0) / (X1 - X0);
            float normZ = (Z - Z0) / (Z1 - Z0);

            // Interpolate along the X direction
            float botEdge = h00 * (1 - normX) + h10 * normX;  // bottom edge interpolation
            float topEdge = h01 * (1 - normX) + h11 * normX;  // top edge interpolation

            // Interpolate along the Z direction
            float H_interpolated = botEdge * (1 - normZ) + topEdge * normZ;

            return H_interpolated;
        }

        // will give the approximate height for a point
        float approximateHeight(glm::vec3 pos) {

            // find which face is on

            // x mod width
            float resx = std::floor(pos.x/segment_width);
            resx = resx * segment_width;
            float resz = - std::floor(abs(pos.z)/segment_length);
            resz = resz * segment_length;

            float height;

            // get height at the 4 coners of a grid square
            float resX0,resZ0,resX1,resZ1;
            float h1,h2,h3,h4;
            
            resX0 = resx;
            resX1 = resx + segment_width;
            resZ0 = resz;
            resZ1 = resz + segment_length;

            h1 = heightMap[std::make_pair(resX0, resZ0)];
            h2 = heightMap[std::make_pair(resX1, resZ0)];
            h3 = heightMap[std::make_pair(resX0, resZ1)];
            h4 = heightMap[std::make_pair(resX1, resZ1)];

            //! caution this will create problems when we are on the edge of the chunk!
            //! because we wont have some h values

            // use billinear interpolation to get the height

            height = bilinearInterpolation(pos.x, pos.z, resX0, resZ0, resX1, resZ1, h1,h2,h3,h4);


            return height;

        };

};

#endif