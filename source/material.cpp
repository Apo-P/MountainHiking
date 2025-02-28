#include "material.hpp"

#include <filesystem>

// TODO UPDATE TO USE ASSET loader
Material::Material(const std::string materialPath) {
    const std::filesystem::path directory(materialPath);

    for (auto const& dir_entry : std::filesystem::directory_iterator(directory)) {
        if (!dir_entry.is_regular_file()) continue;

        std::string filepath = dir_entry.path();

        if (
            filepath.find("albedo") != std::string::npos ||
            filepath.find("alb") != std::string::npos ||
            filepath.find("Color") != std::string::npos
        ) {
            albedo = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("normal") != std::string::npos ||
            filepath.find("NormalGL") != std::string::npos
        ) {
            normal = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("metallic") != std::string::npos ||
            filepath.find("metalness") != std::string::npos ||
            filepath.find("Metallic") != std::string::npos
        ) {
            metallic = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("roughness") != std::string::npos ||
            filepath.find("rough") != std::string::npos ||
            filepath.find("Roughness") != std::string::npos
        ) {
            roughness = std::make_shared<Texture>(filepath);
        }
        else if (
            filepath.find("ao") != std::string::npos ||
            filepath.find("AmbientOcclusion") != std::string::npos
        ) {
            ao = std::make_shared<Texture>(filepath);
        }
    }
}


void Material::bind(int texUnitStartPosition) {

    if (albedo != nullptr) {
        albedo->bind(texUnitStartPosition + 0);
    }

    if (normal != nullptr) {
        normal->bind(texUnitStartPosition + 1);
    }

    if (metallic != nullptr) {
        metallic->bind(texUnitStartPosition + 2);
    } 

    if (roughness != nullptr) {
        roughness->bind(texUnitStartPosition + 3);
    } 

    if (ao != nullptr) {
        ao->bind(texUnitStartPosition +4);
    }

}


