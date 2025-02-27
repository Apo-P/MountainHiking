#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "texture.hpp"

struct Material {
    std::shared_ptr<Texture> albedo = nullptr; //surface color or diffuse surface texture
    std::shared_ptr<Texture> normal = nullptr; // surface normals (microfacets)
    std::shared_ptr<Texture> metallic = nullptr; // specifies how metallic is each texel    
    std::shared_ptr<Texture> roughness = nullptr; // specifies how rough is each texel
    std::shared_ptr<Texture> ao = nullptr; // ambient occlusion (how difficult is for light to escape)


    Material() = default;

    // construct by giving each texture directly
    Material(
        const std::shared_ptr<Texture> albedo, const std::shared_ptr<Texture> normal, const std::shared_ptr<Texture> metallic,
        const std::shared_ptr<Texture> roughness, const std::shared_ptr<Texture> ao) : 
        albedo(albedo),
        normal(normal),
        metallic(metallic),
        roughness(roughness),
        ao(ao)
    {};

    // construct a material by finding its components
    Material(const std::string materialPath);

    // bind the textures
    void bind();
};

#endif