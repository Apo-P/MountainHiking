#ifndef MESH_HPP
#define MESH_HPP

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <shader.hpp>

static std::vector<unsigned int> VEC_UINT_DEFAULT_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAULT_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAULT_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};

/// @brief Stores the position, uv, normal data for a vertex
struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    /// @brief Create a VertexData obj with given vertex data
    /// @param position vertex position
    /// @param uv vertex uv (default=0)
    /// @param normal vertex uv (default=0)
    VertexData( 
        const glm::vec3& position,
        const glm::vec2& uv = glm::vec2(0),
        const glm::vec3& normal = glm::vec3(0)
    ) {
        this->position = position;
        this->uv = uv;
        this->normal = normal;
    }

    bool operator<(const VertexData that) const {
        return std::memcmp((void*) this, (void*) &that, sizeof(VertexData)) > 0;
    };

};

/// @brief this is temporary to let compiler know there is a class named renderer
class Renderer;

/// Mesh class has mesh data
class Mesh{
    
    /// @brief Mesh's indexed vertices
    std::vector<VertexData> indexedVertices;
    /// @brief Mesh's indices for indexed Vertices
    std::vector<unsigned int> indices;
    /// @brief Meshe's Buffer objects
    GLuint VAO, VBO, EBO;

    public:

        /// @brief Constructor takes vectors of position, uvs, normals vertices
        /// @param positions positions of vertices
        /// @param uvs uvs of vertices (default = empty vector)
        /// @param normals normals of vertices (default = empty vector)
        Mesh(
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec2>& uvs = VEC_VEC2_DEFAULT_VALUE,
            const std::vector<glm::vec3>& normals = VEC_VEC3_DEFAULT_VALUE
        );

        /// @brief Constructor that takes a vector of VertexData vertices
        Mesh(const std::vector<VertexData>& vertices);

        /// @brief Constructor takes obj file path
        Mesh(
            const std::string path
        );

        /// @brief Deconstructor unloads data and deletes
        ~Mesh();


        /// @brief load Stored indices to Vram
        void loadVram();
        /// @brief updates Vram (by resending indexed vertices)
        void updateVram();
        /// @brief unload Vram
        void unloadVram();
        /// @brief bind mesh VAO
        void bind();
        /// @brief Reset bounded VAO to 0
        void unbind();

        /// @brief returns index count
        int indexCount() const {return indices.size();}
        /// @brief returns vertices count
        int vertexCount() const {return indexedVertices.size();}
        /// @brief returns vertexData (indexedVertices) read only
        const std::vector<VertexData>& getVertexData() {return indexedVertices;} 

        // Todo: change this to a better method
        // Todo: allow manipulation without needing this update function
        /// @brief Modifies the z of a indexed vertex
        /// @param index index
        /// @param newZ 
        void updateVertexZ(int index , float newZ) {
            indexedVertices.at(index).position.z = newZ;
        }

        /// @brief draws Mesh triangles to screen. 
        /// NOTE ALSO UNBINDS VAO AFTER DRAWING
        /// @param renderer renderer reference is required to let compiler know we wont change constant ref
        void draw(Renderer& renderer);


};

#endif