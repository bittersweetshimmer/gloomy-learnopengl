#include <glm/glm.hpp>
#include <gloomy/Attribute.hpp>

namespace learnopengl {
    using Position = gloomy::Attribute<struct PositionTag, glm::vec3>;
    using Normal = gloomy::Attribute<struct NormalTag, glm::vec3>;
    using TextureCoord = gloomy::Attribute<struct TextureCoordTag, glm::vec2>;
    using Vertex = gloomy::Vertex<Position, Normal, TextureCoord>;
    using Index = gloomy::U32;

    using ModelAttributes = gloomy::Attributes<Position, Normal, TextureCoord>;
    
    using Transform = gloomy::Attribute<struct TransformTag, glm::mat4, 16, true>;
    using InstanceAttributes = gloomy::Attributes<Transform>;
}