#pragma once

#include <gloomy/gloomy.hpp>
#include <learnopengl/types.hpp>
#include <objl/objloader.hpp>

#include <string>
#include <vector>
#include <functional>

namespace learnopengl {
    struct Material {
        std::reference_wrapper<const gloomy::Texture2D> diffuse;
        std::reference_wrapper<const gloomy::Texture2D> specular;
        float shininess;

        Material(const gloomy::Texture2D& diffuse, const gloomy::Texture2D& specular, float shininess)
            : diffuse(std::ref(diffuse)), specular(std::ref(specular)), shininess(shininess) {}
    };

    struct Mesh {
    public:
        std::vector<learnopengl::Vertex> vertices;
        std::vector<learnopengl::Index> indices;
        std::reference_wrapper<const learnopengl::Material> material;

        Mesh(
            std::vector<learnopengl::Vertex>&& vertices,
            std::vector<learnopengl::Index>&& indices,
            const learnopengl::Material& material
        ) :
            vertices(std::move(vertices)),
            indices(std::move(indices)),
            material(std::ref(material)) {
                this->setup();
            }

        void setup() {
            this->index_buffer = gloomy::make_ready<gloomy::IndexBuffer>(this->indices);
            this->vertex_buffer = gloomy::make_ready<gloomy::VertexBuffer>(this->vertices);
            this->vertex_array = gloomy::make_generated<gloomy::VertexArray>(learnopengl::ModelAttributes::dynamic());

            gloomy::use([&] { this->vertex_array.commit(); }, this->vertex_array, this->vertex_buffer, this->index_buffer);
        }

        void draw(const gloomy::Program& program) const {
            constexpr auto DIFFUSE_SLOT = 0;
            constexpr auto SPECULAR_SLOT = 1;

            const auto& material = this->material.get();

            program.uniform("u_material.diffuse").sampler(DIFFUSE_SLOT);
            material.diffuse.get().bind_at_slot(DIFFUSE_SLOT);

            program.uniform("u_material.specular").sampler(SPECULAR_SLOT);
            material.specular.get().bind_at_slot(SPECULAR_SLOT);

            program.uniform("u_material.shininess").fvec_values(material.shininess);

            gloomy::gl::active_texture(0);

            gloomy::use([&] { gloomy::draw_indexed<learnopengl::Index>(gloomy::PrimitiveKind::TRIANGLES, this->indices.size()); }, this->vertex_array);
        }
    //private:
        gloomy::VertexArray::Owned vertex_array;
        gloomy::VertexBuffer::Owned vertex_buffer;
        gloomy::IndexBuffer::Owned index_buffer;
    };

    struct Model {
    public:
        void draw(const gloomy::Program& program) const {
            for(const auto& mesh: this->meshes)
                mesh.draw(program);
        };

        Model(
            std::vector<learnopengl::Mesh>&& meshes,
            std::unordered_map<std::string, learnopengl::Material>&& materials,
            std::unordered_map<std::string, gloomy::Texture2D::Owned>&& textures
        )
            : meshes(std::move(meshes)), materials(std::move(materials)), textures(std::move(textures)) {}
    //private:
        std::vector<learnopengl::Mesh> meshes;
        
        std::unordered_map<std::string, learnopengl::Material> materials;
        std::unordered_map<std::string, gloomy::Texture2D::Owned> textures;
    };
}