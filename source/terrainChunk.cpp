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

            // set normals to 0 and recalculate them after heightmap
            normals.push_back(glm::vec3(0, 0, 0));

            // set uv coords
            // u = (ix / gridX) * resolution  | So we increase U based on X and then we multiply by our resolution to tile the texture on each face
            // v = 1 - (iz / gridZ)* resolution | So we increa V based on inverse of z (the more it goes to -z) and then we multiply by our resolution to tile the texture on each face
            uvs.push_back(glm::vec2((ix / float(gridX)) * (resolution), 1 - (iz / float(gridZ))* (resolution)));


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

            // Be carefull with order for face culling
            indices.push_back(glm::vec3 (a, d, b) );
            indices.push_back(glm::vec3 (b, d, c) );

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

    //! TEST
    // this is inefficient make this all happen in create vertex class, first generate heightmap and the make vertices 

    //recalculate normals 

    // face positions
    glm::vec3 pA = glm::vec3(), pB = glm::vec3(), pC = glm::vec3();

    glm::vec3 cb = glm::vec3(), ab = glm::vec3();

    for ( int i = 0, il = vertices.size(); i < il; i += 3 ) {

        // get vertex data
        VertexData& vA = vertices.at( i + 0 );
        VertexData& vB = vertices.at( i + 1 );
        VertexData& vC = vertices.at( i + 2 );

        // get positions
        pA = vA.position;
        pB = vB.position;
        pC = vC.position;

        // calculate face normal
        cb =  pC - pB;
        ab =  pA - pB;
        cb = glm::cross(cb, ab);

        // store new normal
        vA.normal = glm::normalize(glm::vec3(cb.x, cb.y, cb.z ));
        vB.normal = glm::normalize(glm::vec3(cb.x, cb.y, cb.z ));
        vC.normal = glm::normalize(glm::vec3(cb.x, cb.y, cb.z ));

    }


    // generate Mesh
    mesh = std::make_shared<Mesh>(vertices);

}



