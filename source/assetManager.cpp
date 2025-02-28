#include "assetManager.hpp"

// Instantiate static variables
std::map<std::string, std::shared_ptr<Shader>>  AssetManager::Shaders;
std::map<std::string, std::shared_ptr<Texture>> AssetManager::Textures;
std::map<std::string, std::shared_ptr<Material>> AssetManager::Materials;
std::map<std::string, std::shared_ptr<Texture>> AssetManager::MaterialTextures;
std::map<std::string, std::shared_ptr<Cubemap>> AssetManager::Cubemaps;



std::shared_ptr<Shader> AssetManager::LoadShader( std::string name, const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

    // check if we loaded it already
    if (Shaders.count(name)!=0) return Shaders[name];

    // load shader
    Shaders[name] = std::make_shared<Shader>(vShaderFile,fShaderFile, gShaderFile);

    return Shaders[name];
}

std::shared_ptr<Shader> AssetManager::GetShader(std::string name)
{
    if (Shaders.count(name)==0) throw std::runtime_error("Tried to get a shader that hasn't been loaded yet!");
    return Shaders[name];
}

std::shared_ptr<Texture> AssetManager::LoadTexture(std::string name, const char *file)
{
    // check if we loaded it already
    if (Textures.count(name)!=0) return Textures[name];

    // load shader
    Textures[name] = std::make_shared<Texture>(file);

    return Textures[name];
}

std::shared_ptr<Texture> AssetManager::GetTexture(std::string name)
{
    if (Textures.count(name)==0) throw std::runtime_error("Tried to get a texture that hasn't been loaded yet!");
    return Textures[name];
}

void AssetManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        // get value from iterator (.second)
        iter.second->release();
    // (properly) delete all textures
    for (auto iter : Textures)
        // get value from iterator (.second)
        iter.second->release();

}