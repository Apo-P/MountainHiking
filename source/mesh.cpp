#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using std::string, glm::vec2;
//* https://github.com/tinyobjloader/tinyobjloader?tab=readme-ov-file#example-code-new-object-oriented-api

/// @brief Will load vertexData from an obj file
/// @param path = string path to obj file
/// @param vertices = Outputs the vector of vertices
/// @param indices = Gives count of index, Not necessary
void loadOBJWithTiny(
    const string& path,
    std::vector<VertexData>& vertices,
    std::vector<unsigned int>& indices = VEC_UINT_DEFAULT_VALUE) {

    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path.c_str(), reader_config)) {
        if (!reader.Error().empty()) {
            throw std::runtime_error(reader.Error());
        }
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    // auto& materials = reader.GetMaterials();
    
    
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vec2 uv = vec2(0);
            if (attrib.texcoords.size() != 0) {
                uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            glm::vec3 normal = glm::vec3(0);
            if (attrib.normals.size() != 0) {
                normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            VertexData vertex = {position, uv, normal};

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    // TODO .mtl loader
}



bool getSimilarVertexIndex(
    const VertexData& packed,
    std::map<VertexData, unsigned int>& vertexToOutIndex,
    unsigned int& result
) {
    std::map<VertexData, unsigned int>::iterator it = vertexToOutIndex.find(packed);
    if (it == vertexToOutIndex.end()) {
        return false;
    } else {
        result = it->second;
        return true;
    }
}

/// @brief Will index the given vertices and output the indexed vertices for use in a vbo
void indexVBO(
    const std::vector<VertexData>& in_vertices,
    std::vector<unsigned int>& out_indices,
    std::vector<VertexData>& out_vertices
) {
    std::map<VertexData, unsigned int> vertexToOutIndex;

    for (const auto& vertex: in_vertices) {
        // Try to find a similar vertex in out_XXXX
        unsigned int index;
        bool found = getSimilarVertexIndex(vertex, vertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back(index);
        } else { // If not, it needs to be added in the output data.
            out_vertices.push_back(vertex);
            unsigned int newindex = (unsigned int) out_vertices.size() - 1;
            out_indices.push_back(newindex);
            vertexToOutIndex[vertex] = newindex;
        }
    }
}

void calculateTangents(
    std::vector<VertexData> &vertices,
    const std::vector<unsigned int>& indices
) {

    // for each face (triangles in this case)
    for (size_t i = 0; i < indices.size(); i += 3) {
        // get vertices
        VertexData& v0 = vertices[indices[i]];
        VertexData& v1 = vertices[indices[i+1]];
        VertexData& v2 = vertices[indices[i+2]];

        // get triangle edges
        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        // get delta uv
        vec2 deltaUV1 = v1.uv - v0.uv;
        vec2 deltaUV2 = v2.uv - v0.uv;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        // calculate tangent
        glm::vec3 tangent = {
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };

        // store tangent to each vertex
        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;
    }

    for (auto& vertex : vertices) {
        vertex.tangent = normalize(vertex.tangent);
    }
    
}

void Mesh::loadVram() {


    // Generate a VAO and bind it to GPU
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate VBO and bind it to GPU
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(indexedVertices[0]), indexedVertices.data(), GL_STATIC_DRAW);
    //Specify position attribute 
    glVertexAttribPointer(static_cast<int>(Shader::layoutPosition::POSITION), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), NULL);
    glEnableVertexAttribArray(static_cast<int>(Shader::layoutPosition::POSITION));
    //Specify normal attribute 
    glVertexAttribPointer(static_cast<int>(Shader::layoutPosition::NORMAL), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
    glEnableVertexAttribArray(static_cast<int>(Shader::layoutPosition::NORMAL));
    //Specify uv attribute 
    glVertexAttribPointer(static_cast<int>(Shader::layoutPosition::UV), 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
    glEnableVertexAttribArray(static_cast<int>(Shader::layoutPosition::UV));
    //Specify TANGENT attribute 
    glVertexAttribPointer(static_cast<int>(Shader::layoutPosition::TANGENT), 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
    glEnableVertexAttribArray(static_cast<int>(Shader::layoutPosition::TANGENT));


    //Generate EBO and bind it to GPU
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);


    // reset bounded VAO,VBO,EBO to 0 after we are done (Bounded VAO is active VAO In GPU) 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::updateVram() {

    //bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // re-upload new data (no need to respecify positions like initial loading)
    glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(indexedVertices[0]), indexedVertices.data(), GL_STATIC_DRAW);
    
    // unbind VAO and VBO after were are done
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::unloadVram() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh(const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& uvs,
        const std::vector<glm::vec3>& normals
){
    std::vector<VertexData> vertices;

    for (size_t i=0; i<positions.size(); i++) {
        VertexData vertex = {positions[i], uvs[i], normals[i]};
        vertices.push_back(vertex);
    }

    //Index Vertices and store them
    indexVBO(vertices, indices, indexedVertices);

    // Do other calculations if necessary

    // loadVram
    loadVram();
};

Mesh::Mesh(const std::vector<VertexData>& in_vertices){
    
 
    //Index Vertices and store them
    indexVBO(in_vertices, indices, indexedVertices);

    // Do other calculations if necessary
    // calculate tangent vectors
    calculateTangents(this->indexedVertices, indices);

    // loadVram
    loadVram();

};

Mesh::Mesh(const std::string path) {

    std::vector<VertexData> vertices;

    if (path.substr(path.size() - 3, 3) == "obj") {
        //Load vertices
        loadOBJWithTiny(path, vertices);
    } else {
        throw std::runtime_error("File format not supported: " + path);
    }

    //Index Vertices and store them
    indexVBO(vertices, indices, indexedVertices);

    // Do other calculations if necessary
    // calculate tangent vectors
    calculateTangents(this->indexedVertices, indices);

    // loadVram
    loadVram();
}

Mesh::~Mesh(){unloadVram();}

void Mesh::bind() {
    // bind Mesh VAO
    glBindVertexArray(VAO);

}

void Mesh::unbind() {
    // Reset bounded VAO to 0
    glBindVertexArray(0);
}

void Mesh::draw(Renderer& renderer) {

    // draw elements
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Reset bounded VAO after we are done drawing
    unbind();

}
