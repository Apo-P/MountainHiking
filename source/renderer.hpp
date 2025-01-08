#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>

class Renderer{
    private:
        int shader;
    public:
        Renderer();

        void Render();

        ~Renderer();
};

#endif