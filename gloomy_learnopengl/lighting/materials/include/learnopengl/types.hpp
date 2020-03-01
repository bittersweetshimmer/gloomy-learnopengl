#include <glm/glm.hpp>
#include <gloomy/Attribute.hpp>

namespace learnopengl {
    using Position = gloomy::Attribute<struct PositionTag, glm::vec3>;
    using Normal = gloomy::Attribute<struct NormalTag, glm::vec3>;
    using Vertex = gloomy::Vertex<Position, Normal>;

    using CubeAttributes = gloomy::Attributes<learnopengl::Position, learnopengl::Normal>;
    using LightAttributes = gloomy::Attributes<learnopengl::Position, learnopengl::Normal::Pad>;
}