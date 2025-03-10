#ifndef POST_PROCESSOR_HPP
#define POST_PROCESSOR_HPP

#include "common.hpp"

// #include "sprite_renderer.h"

#include "shader.hpp"
#include "bufferTexture.hpp"


// PostProcessor hosts all PostProcessing effects 
// It renders the game on a textured quad after which one can
// enable specific effects by enabling the effects flags.
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.




class PostProcessor
{
public:
    // state
    Shader PostProcessingShader;
    BufferTexture Texture;
    unsigned int Width, Height;
    
    // Effect flags
    bool Bobbing, Distortion;

    // constructor
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // should be called after rendering the game, so it stores all the rendered data into a texture object
    void EndRender();
    // renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render(float time);
private:
    // render state
    unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int colorBuffer; // color buffer is used for multisampled color buffer
    unsigned int depthBuffer; //depthBuffer is used for multisampled depthBuffer
    unsigned int VAO;
    // initialize quad for rendering postprocessing texture
    void initRenderData();
};

#endif