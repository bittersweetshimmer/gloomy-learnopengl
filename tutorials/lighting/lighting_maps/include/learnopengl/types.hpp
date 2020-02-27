#include <glm/glm.hpp>
#include <gloomy/Attribute.hpp>

namespace learnopengl {
    using Position = gloomy::Attribute<struct PositionTag, glm::vec3>;
    using Normal = gloomy::Attribute<struct NormalTag, glm::vec3>;
    using TextureCoord = gloomy::Attribute<struct TextureCoordTag, glm::vec2>;
    using Vertex = gloomy::Vertex<Position, Normal, TextureCoord>;

    using CubeAttributes = gloomy::Attributes<Position, Normal, TextureCoord>;
    using LightAttributes = gloomy::Attributes<Position, Normal::Pad, TextureCoord::Pad>;
}