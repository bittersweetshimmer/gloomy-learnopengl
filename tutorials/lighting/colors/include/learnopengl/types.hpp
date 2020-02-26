#include <glm/glm.hpp>
#include <gloomy/Attribute.hpp>

namespace learnopengl {
    using Position = gloomy::Attribute<struct PositionTag, glm::vec3>;
    using Attributes = gloomy::Attributes<Position>;
    using Vertex = gloomy::Vertex<Position>;
}