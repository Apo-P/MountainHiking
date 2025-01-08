#include <renderer.hpp>

Renderer::Renderer(){
    std::cout << "Renderer class made" << std::endl;
}

Renderer::~Renderer(){
    std::cout << "Renderer class Deleted" << std::endl;
}

void Renderer::Render(){
    std::cout << "Renderer Render called" << std::endl;
}