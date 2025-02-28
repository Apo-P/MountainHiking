
#include "postProcessor.hpp"


PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height) 
    : PostProcessingShader(shader), Texture(), Width(width), Height(height), Bobbing(false), Distortion(false)
{
    // initialize renderbuffer/framebuffer object
    glGenFramebuffers(1, &this->MSFBO); //MSFBO Multisampled Frame Buffer Object
    
    // initialize renderbuffer storage with a multisampled color buffer 
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);


    //! initialize MSFBO components (we just want depth and color, could and stencil later)

    int samples = 4; // samples to take in msfbo

    // depth buffer - needs to be multisampled
    //! IMPORTANT dont forget depth buffer or else depth test wont
    
    glGenRenderbuffers(1, &this->depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);


    // color buffer - needs to be multisampled
    glGenRenderbuffers(1, &this->colorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->colorBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->colorBuffer);


    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

    // initialize anti aliased fbo (output of msfbo)
        // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)

    glGenFramebuffers(1, &this->FBO); // Frame Buffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->Texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0); // attach texture to framebuffer as its color attachment

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // initialize render data and uniforms
    this->initRenderData();

    this->PostProcessingShader.SetInteger("scene", 0, true);

    // store offsets
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right    
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.getProgramId(), "offsets"), 9, (float*)offsets);

    // store edge detection kernel
    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->PostProcessingShader.getProgramId(), "edge_kernel"), 9, edge_kernel);
    
    // store blur kernel
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(this->PostProcessingShader.getProgramId(), "blur_kernel"), 9, blur_kernel);    
}

void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);

    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // clear both buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}
void PostProcessor::EndRender()
{
    // now resolve multisampled color-buffer into intermediate FBO to store to texture
    // bind msfbo
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    // bind fbo
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    // transfer contents of msfbo to fbo
    glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, // input pos to output pos
         GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST); // Which buffer to copy, what filtering to use if needed
    
    // bind back to default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer

    

}

void PostProcessor::Render(float time)
{

    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    // set uniforms/options
    this->PostProcessingShader.bind();
    this->PostProcessingShader.SetFloat("time", time);
    this->PostProcessingShader.SetInteger("bobbing", this->Bobbing);
    this->PostProcessingShader.SetInteger("distortion", this->Distortion);

    // render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->Texture.Bind();	
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    //? or could blit fbo to screen buffer?
}

void PostProcessor::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;

    // vertices of of framebuffer (vertices of screen basically)
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}