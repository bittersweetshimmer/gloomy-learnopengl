#include <glm/glm.hpp>
#include <gloomy/Attribute.hpp>

namespace learnopengl {
    using Position = gloomy::Attribute<struct PositionTag, glm::vec3>;
    using Position2D = gloomy::Attribute<struct Position2DTag, glm::vec2>;
    using TextureCoord = gloomy::Attribute<struct TextureCoordTag, glm::vec2>;
    using Vertex = gloomy::Vertex<Position, TextureCoord>;
    using Index = gloomy::U32;

    using Attributes = gloomy::Attributes<Position, TextureCoord>;
    using ScreenQuadAttributes = gloomy::Attributes<Position2D, TextureCoord>;
}