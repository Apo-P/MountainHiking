
//include headers
#include <engine.hpp>

// #include <renderer.hpp>
// #include <shader.hpp>
// #include <mesh.hpp>






// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>






int main()
{

    GameEngine engine{};

    std::cout << "Hello World!" << std::endl;

    int exit_condition; // will store if game exited properly or not

    exit_condition = engine.startGame();

    return exit_condition;
}