#include <array>

#include <gloomy/gloomy.hpp>
#include <learnopengl/learnopengl.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr auto SCR_WIDTH = 800;
constexpr auto SCR_HEIGHT = 600;

namespace learnopengl {
  inline void setup_camera(learnopengl::CameraContext& context, GLFWwindow& window);
  inline void process_input(learnopengl::Camera& camera, float delta_time, GLFWwindow& window);
}

int main(int argc, char *argv[]) {
  return learnopengl::glfw_context(SCR_WIDTH, SCR_HEIGHT, "gloomy", [&](GLFWwindow &window) {
    gloomy::gl::enable(gloomy::Capability::DEPTH_TEST);

    const auto cube = learnopengl::make_cube();

    const auto container = gloomy::make_ready<gloomy::Texture2D>(learnopengl::load_image("assets/images/container2.png").value());
    const auto container_specular = gloomy::make_ready<gloomy::Texture2D>(learnopengl::load_image("assets/images/container2_specular.png").value());

    constexpr auto DIFFUSE_SLOT = 0;
    constexpr auto SPECULAR_SLOT = 1;

    const auto lighting_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/multiple_lights/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/multiple_lights/FragmentShader.hlsl").value())
    );

    const auto lamp_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/lamp/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/lamp/FragmentShader.hlsl").value())
    );

    const auto vertex_buffer = gloomy::make_ready<gloomy::VertexBuffer>(cube);
    const auto cube_vertex_array = gloomy::make_generated<gloomy::VertexArray>(learnopengl::CubeAttributes::dynamic());
    const auto light_vertex_array = gloomy::make_generated<gloomy::VertexArray>(learnopengl::LightAttributes::dynamic());

    gloomy::use([&] {
      gloomy::use([&] { cube_vertex_array.commit(); }, cube_vertex_array);
      gloomy::use([&] { light_vertex_array.commit(); }, light_vertex_array);
    }, vertex_buffer);

    gloomy::use([&] {
      lighting_shader.uniform("u_material.diffuse").sampler(DIFFUSE_SLOT);
      lighting_shader.uniform("u_material.specular").sampler(SPECULAR_SLOT);
    }, lighting_shader);

    auto camera = learnopengl::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto last_position = glm::vec2(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
    bool first_mouse = true;
    auto context = learnopengl::CameraContext{camera, last_position, first_mouse};

    learnopengl::setup_camera(context, window);

    auto delta_time = 0.0f;
    auto last_frame = 0.0f;

    auto spot_light = learnopengl::SpotLight{
        .position = camera.get_position(),  
        .direction = camera.get_front(),
        .cutoff = glm::cos(glm::radians(12.5f)),
        .outer_cutoff = glm::cos(glm::radians(15.0f)),
    
        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.8f, 0.8f, 0.0f),
        .specular = glm::vec3(0.8f, 0.8f, 0.0f),
        
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f
    };

    const auto point_light_colors = std::array<glm::vec3, 4>{
      glm::vec3(1.0f, 0.6f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 0.0f),
      glm::vec3(0.2f, 0.2f, 1.0f)
    };

    const auto point_lights = std::array<learnopengl::PointLight, 4>{
       learnopengl::PointLight{
          .position = glm::vec3(0.7f, 0.2f, 2.0f), 
          .ambient = point_light_colors[0] * 0.1f,
          .diffuse = point_light_colors[0],
          .specular = point_light_colors[0],
          .constant = 1.0f,
          .linear = 0.09f,
          .quadratic = 0.032f
        },
        learnopengl::PointLight{
          .position = glm::vec3(2.3f, -3.3f, -4.0f), 
          .ambient = point_light_colors[1] * 0.1f,
          .diffuse = point_light_colors[1],
          .specular = point_light_colors[1],
          .constant = 1.0f,
          .linear = 0.09f,
          .quadratic = 0.032f
        },
        learnopengl::PointLight{
          .position = glm::vec3(-4.0f,  2.0f, -12.0f), 
          .ambient = point_light_colors[2] * 0.1f,
          .diffuse = point_light_colors[2],
          .specular = point_light_colors[2],
          .constant = 1.0f,
          .linear = 0.09f,
          .quadratic = 0.032f
        },
        learnopengl::PointLight{
          .position = glm::vec3( 0.0f,  0.0f, -3.0f), 
          .ambient = point_light_colors[3] * 0.1f,
          .diffuse = point_light_colors[3],
          .specular = point_light_colors[3],
          .constant = 1.0f,
          .linear = 0.09f,
          .quadratic = 0.032f
        }
    };

    const auto directional_light = learnopengl::DirectionalLight{
        .direction = glm::vec3(-0.2f, -1.0f, -0.3f),  
    
        .ambient = glm::vec3(0.3f, 0.24f, 0.14f),
        .diffuse = glm::vec3(0.7f, 0.42f, 0.26f),
        .specular = glm::vec3(0.5f, 0.5f, 0.5f),
    };

    const auto cube_positions = std::array<glm::vec3, 10>{
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    learnopengl::loop(window, [&](double time) {
      auto current_frame = static_cast<float>(time);
      delta_time = current_frame - last_frame;
      last_frame = current_frame;
      
      learnopengl::process_input(camera, delta_time, window);

      gloomy::gl::clear_color(0.75f, 0.52f, 0.3f, 1.0f);
      gloomy::gl::clear(gloomy::BufferBit::COLOR | gloomy::BufferBit::DEPTH);

      glm::mat4 view = camera.get_view();
      glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);

      spot_light.position = camera.get_position();
      spot_light.direction = camera.get_front();

      gloomy::use([&] {
        lighting_shader.uniform("u_material.shininess").fvec_values(64.0f);

        lighting_shader.uniform("u_directional_light.direction").fvec<3>(glm::value_ptr(directional_light.direction));
        lighting_shader.uniform("u_directional_light.ambient").fvec<3>(glm::value_ptr(directional_light.ambient));
        lighting_shader.uniform("u_directional_light.diffuse").fvec<3>(glm::value_ptr(directional_light.diffuse));
        lighting_shader.uniform("u_directional_light.specular").fvec<3>(glm::value_ptr(directional_light.specular));

        for (auto i = 0; i < point_lights.size(); ++i) {
          const auto u_light = "u_point_lights[" + std::to_string(i) + "]";
          const auto& point_light = point_lights[i];
          
          lighting_shader.uniform(u_light + ".position").fvec<3>(glm::value_ptr(point_light.position));
          lighting_shader.uniform(u_light + ".ambient").fvec<3>(glm::value_ptr(point_light.ambient));
          lighting_shader.uniform(u_light + ".diffuse").fvec<3>(glm::value_ptr(point_light.diffuse));
          lighting_shader.uniform(u_light + ".specular").fvec<3>(glm::value_ptr(point_light.specular));
          lighting_shader.uniform(u_light + ".constant").fvec_values(point_light.constant);
          lighting_shader.uniform(u_light + ".linear").fvec_values(point_light.linear);
          lighting_shader.uniform(u_light + ".quadratic").fvec_values(point_light.quadratic);
        }

        lighting_shader.uniform("u_spot_light.position").fvec<3>(glm::value_ptr(spot_light.position));
        lighting_shader.uniform("u_spot_light.direction").fvec<3>(glm::value_ptr(spot_light.direction));
        lighting_shader.uniform("u_spot_light.cutoff").fvec_values(spot_light.cutoff);
        lighting_shader.uniform("u_spot_light.outer_cutoff").fvec_values(spot_light.outer_cutoff);
        lighting_shader.uniform("u_spot_light.ambient").fvec<3>(glm::value_ptr(spot_light.ambient));
        lighting_shader.uniform("u_spot_light.diffuse").fvec<3>(glm::value_ptr(spot_light.diffuse));
        lighting_shader.uniform("u_spot_light.specular").fvec<3>(glm::value_ptr(spot_light.specular));
        lighting_shader.uniform("u_spot_light.constant").fvec_values(spot_light.constant);
        lighting_shader.uniform("u_spot_light.linear").fvec_values(spot_light.linear);
        lighting_shader.uniform("u_spot_light.quadratic").fvec_values(spot_light.quadratic);

        lighting_shader.uniform("u_view_position").fvec<3>(glm::value_ptr(camera.get_position()));
        lighting_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));
        lighting_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));

        container.bind_at_slot(DIFFUSE_SLOT);
        container_specular.bind_at_slot(SPECULAR_SLOT);

        for (auto i = 0; i < cube_positions.size(); ++i) {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cube_positions[i]);
          
          float angle = 20.0f * i;
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          
          lighting_shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
          gloomy::use([&] { gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, cube.size()); }, cube_vertex_array);
        }
      }, lighting_shader);

      gloomy::use([&] {
        lamp_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));
        lamp_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));

        for (auto i = 0; i < point_lights.size(); ++i) {
          const auto u_light = "u_point_lights[" + std::to_string(i) + "]";
          const auto& point_light = point_lights[i];

          glm::mat4 model = glm::mat4(1.0f);
          model = glm::mat4(1.0f);
          model = glm::translate(model, point_light.position);
          model = glm::scale(model, glm::vec3(0.2f));
          
          lamp_shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
          lamp_shader.uniform("u_color").fvec<3>(glm::value_ptr(point_light.specular));

          gloomy::use([&] { gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, cube.size()); }, light_vertex_array);
        }
      }, lamp_shader);
    });
  });
}

#if defined(_WIN32) || defined(__CYGWIN__)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nCmdShow) {
  return main(__argc, __argv);
}
#endif

inline void learnopengl::setup_camera(learnopengl::CameraContext& context, GLFWwindow& window) {
  glfwSetWindowUserPointer(&window, &context);

  glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
  glfwSetCursorPosCallback(&window, [](GLFWwindow* window, double xpos, double ypos) {
    auto context = static_cast<learnopengl::CameraContext*>(glfwGetWindowUserPointer(window));

    if (context->first_mouse) {
      context->last_position = glm::vec2(xpos, ypos);
      context->first_mouse = false;
    }

    float xoffset = xpos - context->last_position.x;
    float yoffset = context->last_position.y - ypos;
    context->last_position = glm::vec2(xpos, ypos);

    context->camera.process_mouse_movement(xoffset, yoffset);
  });
    
  glfwSetScrollCallback(&window, [](GLFWwindow* window, double xoffset, double yoffset) {
    auto context = static_cast<learnopengl::CameraContext*>(glfwGetWindowUserPointer(window));

    context->camera.process_mouse_scroll(yoffset);
  });
}

inline void learnopengl::process_input(learnopengl::Camera& camera, float delta_time, GLFWwindow& window) {
  if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
    camera.process_keyboard(learnopengl::CameraMovement::FORWARD, delta_time);
  else if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
    camera.process_keyboard(learnopengl::CameraMovement::BACKWARD, delta_time);
  else if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
    camera.process_keyboard(learnopengl::CameraMovement::LEFT, delta_time);
  else if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
    camera.process_keyboard(learnopengl::CameraMovement::RIGHT, delta_time);
}