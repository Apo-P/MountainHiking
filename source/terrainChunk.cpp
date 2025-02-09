#include <terrainChunk.hpp>


TerrainChunk::TerrainChunk(HeightGenerator& heightGenerator, float chunkX, float chunkZ, int chunkSize, int resolution) :
    heightGenerator(heightGenerator),
    chunkX(chunkX),
    chunkZ(chunkZ),
    chunkSize(chunkSize),
    resolution(resolution) {

        std::cout << "chunk created" << std::endl;
}

std::vector<VertexData> TerrainChunk::createVertices() {

    // float width, float length, int widthSegments, int lengthSegments
    
    float width = chunkSize;
    float length = chunkSize;

    // store segments
    int gridX = resolution;
    int gridZ = resolution;

    // add +1 to create enough vertices
    int gridX1 = gridX + 1;
    int gridZ1 = gridZ + 1;

    // calculate segment width and length
    float segment_width = width / gridX;
    float segment_length = length / gridZ;

    // create arrays for vertex data
    std::vector<glm::vec3> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;


    float x,z;
    for(int iz = 0; iz < gridZ1; iz++) {


        // we want -iz so chunk grows towards -z
        z = (-iz * segment_length) + chunkZ;

        for(int ix = 0; ix < gridX1; ix++) {

            x = (ix * segment_width) + chunkX;

            positions.push_back(glm::vec3(x, 0, z));

            normals.push_back(glm::vec3(0, 0, 1));

            uvs.push_back( glm::vec2( ix / gridX ));
            uvs.push_back( glm::vec2( 1 - ( iz / gridZ )) );

        }

    }

    // indice data
    unsigned int a,b,c,d;
    for (int iz = 0; iz < gridZ; iz ++ ) {

        for (int ix = 0; ix < gridX; ix ++ ) {

            a = ix + gridX1 * iz;
            b = ix + gridX1 * ( iz + 1 );
            c = ( ix + 1 ) + gridX1 * ( iz + 1 );
            d = ( ix + 1 ) + gridX1 * iz;

            indices.push_back(glm::vec3 (a, b, d) );
            indices.push_back(glm::vec3 (b, c, d) );

        }

    }

    //? could use indices vertexData from the start to increase performance

    // pack data to vertex data
    std::vector<VertexData> vertexData;
    for (auto index :indices) {
        vertexData.push_back(VertexData(positions.at(index.x), uvs.at(index.x) ,normals.at(index.x)));
        vertexData.push_back(VertexData(positions.at(index.y), uvs.at(index.y) ,normals.at(index.y)));
        vertexData.push_back(VertexData(positions.at(index.z), uvs.at(index.z) ,normals.at(index.z)));
    }

    return vertexData;
}

void TerrainChunk::generateChunk(int seed) {

    //? Make alternative version to add height to heightmap as position is created to reduce complexity
    //? so not to first create everything and the modify it


    std::vector<VertexData> vertices = createVertices();

    //? should change this to reduce complexity;

    std::vector<glm::vec3> positions;
    for (auto vertex : vertices){
        positions.push_back(vertex.position);
    }

    // get heightMap
    heightMap = heightGenerator.makeHeightMap(positions, seed);

    // update vertices
    for (auto& vertex : vertices){
        vertex.position.y = heightMap[std::make_pair(vertex.position.x, vertex.position.z)];
    } 

    // generate Mesh
    mesh = std::make_shared<Mesh>(vertices);

}



