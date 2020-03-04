#pragma once

#include <optional>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <stb/stb_image.h>

#include <gloomy/gloomy.hpp>

#include <learnopengl/model.hpp>

namespace learnopengl {
    inline std::optional<gloomy::src::Image> load_image(std::filesystem::path path) {
        int32_t width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        uint8_t* data = stbi_load(path.string().data(), &width, &height, &channels, 4);
        auto size = static_cast<size_t>(static_cast<int64_t>(width) * static_cast<int64_t>(height) * channels);
        auto pointer = std::unique_ptr<std::byte[]>(reinterpret_cast<std::byte*>(data));

        if (data == nullptr) return std::nullopt;
        return gloomy::src::Image{
            .data = std::move(pointer),
            .size = size,
            .width = width,
            .height = height
        };
    }
    
	template<gloomy::ShaderKind Kind>
	inline std::optional<gloomy::src::Shader<Kind>> load_shader(std::filesystem::path path) {
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			file.open(path);
			std::stringstream string_stream;

			string_stream << file.rdbuf();
			file.close();

			return gloomy::src::Shader<Kind>(string_stream.str());
		}
		catch (std::ifstream::failure e) {
            std::cerr << "learnopengl: error while loading shader from \"" << path.string() << "\": " << e.code().message() << std::endl;
			assert(false);

			return std::nullopt;
		}
	};

    inline std::optional<gloomy::src::Shader<gloomy::ShaderKind::VERTEX>> load_vertex_shader(std::filesystem::path path) {
        return load_shader<gloomy::ShaderKind::VERTEX>(path);
    }

    inline std::optional<gloomy::src::Shader<gloomy::ShaderKind::FRAGMENT>> load_fragment_shader(std::filesystem::path path) {
        return load_shader<gloomy::ShaderKind::FRAGMENT>(path);
    }

    inline std::optional<learnopengl::Model> load_model(std::filesystem::path path) {
        objl::Loader loader;
        const std::string path_str = path.string();

        std::cerr << "learnopengl: loading model from \"" << path_str << "..." << std::endl;

        if (!loader.LoadFile(path_str)) {
            std::cerr << "learnopengl: loading model failed. File needs to be available in the provided path and have \".obj\" extenstion." << std::endl;
            return std::nullopt;
        }

        std::vector<learnopengl::Mesh> meshes;
        std::unordered_map<std::string, learnopengl::Material> materials;
        std::unordered_map<std::string, gloomy::Texture2D::Owned> textures;

        meshes.reserve(loader.LoadedMeshes.size());
        materials.reserve(loader.LoadedMaterials.size());

        const std::string directory = path_str.substr(0, path_str.find_last_of('/'));

        for (const auto& objl_material: loader.LoadedMaterials) {
            auto material_found = materials.find(objl_material.name);
            if (material_found != materials.end()) continue;

            std::cerr << "learnopengl: loading material \"" << objl_material.name << "\"." << std::endl;

            auto diffuse_path = std::filesystem::path(directory + "/" + objl_material.map_Kd);
            auto specular_path = std::filesystem::path(directory + "/" +objl_material.map_Ks);
            
            std::optional<std::reference_wrapper<const gloomy::Texture2D>> diffuse_texture = std::nullopt;
            std::optional<std::reference_wrapper<const gloomy::Texture2D>> specular_texture = std::nullopt;
            float shininess = objl_material.illum;

            auto diffuse_found = textures.find(diffuse_path.string());
            if (diffuse_found != textures.end()) { diffuse_texture = std::ref(diffuse_found->second); }
            else {
                std::cerr << "learnopengl: loading diffuse texture from \"" << diffuse_path.string() << "\"." << std::endl;
                auto maybe_image = learnopengl::load_image(diffuse_path);
                
                if (maybe_image.has_value()) {
                    auto texture = gloomy::make_ready<gloomy::Texture2D>(maybe_image.value());
                    textures.insert(std::make_pair(diffuse_path.string(), std::move(texture)));
                    diffuse_texture = textures.at(diffuse_path.string());
                }
                else {
                    std::cerr << "learnopengl: no valid diffuse texture found at \"" << diffuse_path.string() << "\"." << std::endl;
                }
            }

            auto specular_found = textures.find(specular_path.string());
            if (specular_found != textures.end()) { specular_texture = std::ref(specular_found->second); }
            else {
                std::cerr << "learnopengl: loading specular texture from \"" << specular_path.string() << "\"." << std::endl;
                auto maybe_image = learnopengl::load_image(specular_path);
                
                if (maybe_image.has_value()) {
                    auto texture = gloomy::make_ready<gloomy::Texture2D>(maybe_image.value());
                    textures.insert(std::make_pair(specular_path.string(), std::move(texture)));
                    specular_texture = textures.at(specular_path.string());
                }
                else {
                    std::cerr << "learnopengl: no valid specular texture found at \"" << specular_path.string() << "\"." << std::endl;
                }
            }

            if (!diffuse_texture.has_value()) std::cerr << "learnopengl: diffuse texture unavailable." << std::endl;
            if (!specular_texture.has_value()) std::cerr << "learnopengl: specular texture unavailable." << std::endl;

            assert(diffuse_texture.has_value() && specular_texture.has_value());

            auto material = learnopengl::Material(diffuse_texture.value().get(), specular_texture.value().get(), shininess);

            materials.insert(std::make_pair(objl_material.name, std::move(material)));
        }

        for (const auto& objl_mesh: loader.LoadedMeshes) {
            std::cerr << "learnopengl: loading mesh \"" << objl_mesh.MeshName << "\"." << std::endl;

            std::vector<learnopengl::Vertex> vertices;
            std::vector<learnopengl::Index> indices;

            vertices.reserve(objl_mesh.Vertices.size());
            indices.reserve(objl_mesh.Indices.size());

            for (const auto& objl_vertex: objl_mesh.Vertices) {
                vertices.emplace_back(
                    Position(objl_vertex.Position.X, objl_vertex.Position.Y, objl_vertex.Position.Z),
                    Normal(objl_vertex.Normal.X, objl_vertex.Normal.Y, objl_vertex.Normal.Z),
                    TextureCoord(objl_vertex.TextureCoordinate.X, objl_vertex.TextureCoordinate.Y)
                );
            }

            for (const auto& objl_index: objl_mesh.Indices) {
                indices.push_back(static_cast<learnopengl::Index>(objl_index));
            }

            meshes.emplace_back(std::move(vertices), std::move(indices), materials.at(objl_mesh.MeshMaterial.name));
        }

        return learnopengl::Model(std::move(meshes), std::move(materials), std::move(textures));
    }
}