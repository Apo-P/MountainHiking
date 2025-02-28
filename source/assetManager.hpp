#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include "common.hpp"

#include "texture.hpp"
#include "shader.hpp"
#include "material.hpp"
#include "cubemap.hpp"


// A static singleton AssetManager class . 
// Loads and stores all textures and material so the are not loaded again
//All functions and resources are static and no public constructor is defined.
class AssetManager
{
    public:
        // resource storage //use a map 
        static std::map<std::string, std::shared_ptr<Shader>>  Shaders;
        static std::map<std::string, std::shared_ptr<Texture>> Textures;
        static std::map<std::string, std::shared_ptr<Material>> Materials;
        static std::map<std::string, std::shared_ptr<Texture>> MaterialTextures;
        static std::map<std::string, std::shared_ptr<Cubemap>> Cubemaps;

        // could store a list of all possible names somewhere


        // loads shader into storage (if it isn't loaded already) and retrieves it
        static std::shared_ptr<Shader> LoadShader(std::string name, const char *vShaderFile, const char *fShaderFile, const char *gShaderFile="");
        // retrieves a stored shader
        static std::shared_ptr<Shader> GetShader(std::string name);
        
        // loads Texture into storage (if it isn't loaded already) and retrieves it
        static std::shared_ptr<Texture> LoadTexture(std::string name, const char *file);
        // retrieves a stored texture
        static std::shared_ptr<Texture> GetTexture(std::string name);

        //TODO implement Material Loading
        // can check if material is loaded, if not and need to load a new material we could also check if we have the same material texture already

        //TODO impelement Cubemap Loading

        // properly de-allocates all loaded resources
        static void      Clear();
    private:
        // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        AssetManager() { }
    };

#endif