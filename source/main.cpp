#include <iostream>
#include <cstdlib>


#include <vector>
#include <string>
#include <cstring> //for memcmp
#include <map>
#include <memory>

//include glew before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

//include headers
#include <renderer.hpp>

#define W_WIDTH 1024
#define W_HEIGHT 768
#define W_TITLE "MountainHiking"

GLFWwindow* initialize_window(){

    // check if glfw Initialized
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    // Configure GLFW with hints
    // Set OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set OpenGL Core profile to use all new functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // create window
    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, W_TITLE, NULL, NULL);

    // Check if window opened
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error(std::string(std::string("Failed to open GLFW window.") +
                            " If you have an Intel GPU, they are not 3.3 compatible." +
                            "Try the 2.1 version.\n"));
    }

    // Introduce the indow to the current context
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW so it configures OpenGl (Could use GLAD here!)
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW\n");
    }


    //Specify the viewport of openGL in the window
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // Set background color (gray) [r, g, b, a]
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    return window;

}


static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{};
static std::vector<glm::vec3> VEC_VEC3_DEFAUTL_VALUE{};
static std::vector<glm::vec2> VEC_VEC2_DEFAUTL_VALUE{};
static std::map<std::string, GLuint> MAP_STRING_GLUINT_DEFAULT_VALUE{};

struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

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


class Mesh{
    
    std::vector<VertexData> indexedVertices;
    std::vector<unsigned int> indices;

    Mesh(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& uvs = VEC_VEC2_DEFAUTL_VALUE,
        const std::vector<glm::vec3>& normals = VEC_VEC3_DEFAUTL_VALUE
    );

};

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

Mesh::Mesh(const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& uvs = VEC_VEC2_DEFAUTL_VALUE,
        const std::vector<glm::vec3>& normals = VEC_VEC3_DEFAUTL_VALUE
){
    std::vector<VertexData> vertices;

    for (size_t i=0; i<positions.size(); i++) {
        VertexData vertex = {positions[i], uvs[i], normals[i]};
        vertices.push_back(vertex);
    }

    indexVBO(vertices, indices, indexedVertices);
};



int main()
{
    std::cout << "Hello World!" << std::endl;
    try
    {
        //Enter Initializers
        
        // initialize window
        GLFWwindow* window = initialize_window();

        // initialize graphics pipeline / renderer
        Renderer renderer;
        // initialize and load meshes and models

        // initialize other systems


        //Enter main loop
        do
        {
            //calculate dt

            // Poll mouse and keyboard
            glfwPollEvents();

            // Clear Buffers / Screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Event handling / Update game state

            // Render into framebuffer

            // Swap Buffers
            glfwSwapBuffers(window);
        } 
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                    glfwWindowShouldClose(window) == 0);
        
        
        //Enter Exit procedure

        // Terminate glfw before exiting
        glfwDestroyWindow(window); //? Terminate the window. Is it necessary if glfwTerminate() will delete all windows?
        glfwTerminate();

    } catch (std::exception& ex) {
        // If error occurred print error and terminate program
        std::cout << ex.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}