#include <terrainChunk.hpp>


TerrainChunk::TerrainChunk(HeightGenerator& heightGenerator, float chunkX, float chunkZ, int chunkSize, int resolution) :
    heightGenerator(heightGenerator),
    chunkX(chunkX),
    chunkZ(chunkZ),
    chunkSize(chunkSize),
    resolution(resolution) {

        // set initial textures
        //? this should be done my terrain chunk manager or by chunk its self at creation
        //? for example chunk could have special values, flags
        textureHeight0 = AssetManager::LoadTexture("dryDirt.png", "resources/textures/dryDirt.png"); //!use file name as key for now
        textureHeight1 = AssetManager::LoadTexture("grass1.png", "resources/textures/grass1.png"); //!use file name as key for now
        textureHeight2 = AssetManager::LoadTexture("snow1.png", "resources/textures/snow1.png"); //!use file name as key for now

        // std::cout << "chunk created" << std::endl;
}

float TerrainChunk::getHeight(float x, float z) {
    //if point exists in heightmap
    if (heightMap[std::make_pair(x, z)]){
        return heightMap[std::make_pair(x, z)];
    }

    std::cout << "point not found!" << std::endl;
    //! could throw an error here
    return false;
}

bool TerrainChunk::updateHeight(float x, float z, float newValue) {
    //if point exists in heightmap
    if (heightMap[std::make_pair(x, z)]){
        heightMap[std::make_pair(x, z)] = newValue;
        return true;
    }
    std::cout << "couldn't update point!" << std::endl;
    //! could throw an error here
    return false;
}

void TerrainChunk::recalculateHeight(){

    //! We need to have update the height generator or else it would'nt change something
    // // !TEST
    // heightGenerator = HeightGenerator(21,0,246);
    
    for(auto &vertex : mesh.get()->getModifiableVertexData()) {

        // calculate new height
        float newHeight = heightGenerator.calculateHeight(vertex.position.x,vertex.position.z);

        // try to update height
        if (updateHeight(vertex.position.x,vertex.position.z, newHeight)){
            // if succefull then add it to vertex position
            vertex.position.y = newHeight;    
        }
    }

    //? should mesh updating be a different func ?
    mesh.get()->updateVram();

}

void TerrainChunk::reCalculateNormals( std::vector<VertexData> &vertices){
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
    segment_width = width / gridX;
    segment_length = length / gridZ;

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

    // TODO use less loops and make more happen at verticies creation


    // create vertices
    std::vector<VertexData> vertices = createVertices();

    //? should change this to reduce complexity;

    // get vertices positions
    std::vector<glm::vec3> positions;
    for (auto vertex : vertices){
        positions.push_back(vertex.position);
    }

    // generate heightMap
    heightMap = heightGenerator.makeHeightMap(positions, seed);

    // update vertices
    for (auto& vertex : vertices){
        vertex.position.y = getHeight(vertex.position.x, vertex.position.z);
    } 

    //? this is a bit inefficient it's better to calculate normals at face creation
    //TODO make this all happen in create vertex class, first generate heightmap and the make vertices 
    reCalculateNormals(vertices);

    // generate Mesh
    mesh = std::make_shared<Mesh>(vertices);

}



