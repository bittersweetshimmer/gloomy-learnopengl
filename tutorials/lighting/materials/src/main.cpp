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

    const auto lighting_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/materials/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/materials/FragmentShader.hlsl").value())
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

    auto camera = learnopengl::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto last_position = glm::vec2(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
    bool first_mouse = true;
    auto context = learnopengl::CameraContext{camera, last_position, first_mouse};

    learnopengl::setup_camera(context, window);

    auto delta_time = 0.0f;
    auto last_frame = 0.0f;

    auto light_position = glm::vec3(1.2f, 1.0f, 2.0f);

    learnopengl::loop(window, [&](double time) {
      auto current_frame = static_cast<float>(time);
      delta_time = current_frame - last_frame;
      last_frame = current_frame;
      
      learnopengl::process_input(camera, delta_time, window);

      gloomy::gl::clear_color(0.1f, 0.1f, 0.1f, 1.0f);
      gloomy::gl::clear(gloomy::BufferBit::COLOR | gloomy::BufferBit::DEPTH);

      glm::mat4 view = camera.get_view();
      glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);

      light_position.x = 1.0f + sin(time) * 2.0f;
      light_position.y = sin(time / 2.0f) * 1.0f;

      gloomy::use([&] {
        lighting_shader.uniform("u_material.ambient").fvec_values(1.0f, 0.5f, 0.31f);
        lighting_shader.uniform("u_material.diffuse").fvec_values(1.0f, 0.5f, 0.31f);
        lighting_shader.uniform("u_material.specular").fvec_values(0.5f, 0.5f, 0.5f);
        lighting_shader.uniform("u_material.shininess").fvec_values(32.0f);

        glm::vec3 light_color;
        light_color.x = sin(time * 2.0f);
        light_color.y = sin(time * 0.7f);
        light_color.z = sin(time * 1.3f);
          
        glm::vec3 diffuse_color = light_color   * glm::vec3(0.5f); 
        glm::vec3 ambient_color = diffuse_color * glm::vec3(0.2f); 

        lighting_shader.uniform("u_light.ambient").fvec<3>(glm::value_ptr(ambient_color));
        lighting_shader.uniform("u_light.diffuse").fvec<3>(glm::value_ptr(diffuse_color));
        lighting_shader.uniform("u_light.specular").fvec_values(1.0f, 1.0f, 1.0f);
        lighting_shader.uniform("u_light.position").fvec<3>(glm::value_ptr(light_position));

        lighting_shader.uniform("u_view_position").fvec<3>(glm::value_ptr(camera.get_position()));

        glm::mat4 model = glm::mat4(1.0f);
        
        lighting_shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
        lighting_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));
        lighting_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));

        gloomy::use([&] { gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, cube.size()); }, cube_vertex_array);
      }, lighting_shader);

      gloomy::use([&] {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_position);
        model = glm::scale(model, glm::vec3(0.2f));
        
        lamp_shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
        lamp_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));
        lamp_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));

        gloomy::use([&] { gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, cube.size()); }, light_vertex_array);
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