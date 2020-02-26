#include <array>
#include <learnopengl/types.hpp>

namespace learnopengl {
    auto make_cube() {
        return std::array<Vertex, 36>{
            Vertex{Position(-0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f,  0.5f, -0.5f)},
            Vertex{Position( 0.5f,  0.5f, -0.5f)},
            Vertex{Position(-0.5f,  0.5f, -0.5f)},
            Vertex{Position(-0.5f, -0.5f, -0.5f)},

            Vertex{Position(-0.5f, -0.5f,  0.5f)},
            Vertex{Position( 0.5f, -0.5f,  0.5f)},
            Vertex{Position( 0.5f,  0.5f,  0.5f)},
            Vertex{Position( 0.5f,  0.5f,  0.5f)},
            Vertex{Position(-0.5f,  0.5f,  0.5f)},
            Vertex{Position(-0.5f, -0.5f,  0.5f)},

            Vertex{Position(-0.5f,  0.5f,  0.5f)},
            Vertex{Position(-0.5f,  0.5f, -0.5f)},
            Vertex{Position(-0.5f, -0.5f, -0.5f)},
            Vertex{Position(-0.5f, -0.5f, -0.5f)},
            Vertex{Position(-0.5f, -0.5f,  0.5f)},
            Vertex{Position(-0.5f,  0.5f,  0.5f)},

            Vertex{Position( 0.5f,  0.5f,  0.5f)},
            Vertex{Position( 0.5f,  0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f,  0.5f)},
            Vertex{Position( 0.5f,  0.5f,  0.5f)},

            Vertex{Position(-0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f, -0.5f)},
            Vertex{Position( 0.5f, -0.5f,  0.5f)},
            Vertex{Position( 0.5f, -0.5f,  0.5f)},
            Vertex{Position(-0.5f, -0.5f,  0.5f)},
            Vertex{Position(-0.5f, -0.5f, -0.5f)},

            Vertex{Position(-0.5f,  0.5f, -0.5f)},
            Vertex{Position( 0.5f,  0.5f, -0.5f)},
            Vertex{Position( 0.5f,  0.5f,  0.5f)},
            Vertex{Position( 0.5f,  0.5f,  0.5f)},
            Vertex{Position(-0.5f,  0.5f,  0.5f)},
            Vertex{Position(-0.5f,  0.5f, -0.5f)}
        };
    };
}