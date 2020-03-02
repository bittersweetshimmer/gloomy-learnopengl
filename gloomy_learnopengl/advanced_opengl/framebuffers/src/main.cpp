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

    const auto marble = gloomy::make_ready<gloomy::Texture2D>(learnopengl::load_image("assets/images/marble.jpg").value());
    const auto metal = gloomy::make_ready<gloomy::Texture2D>(learnopengl::load_image("assets/images/metal.png").value());

    const auto shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/shader/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/shader/FragmentShader.hlsl").value())
    );

    const auto screen_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/screen/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/screen/FragmentShader.hlsl").value())
    );

    gloomy::use([&] { shader.uniform("u_texture").sampler(0); }, shader);
    gloomy::use([&] { screen_shader.uniform("u_screen_texture").sampler(0); }, screen_shader);

    auto cube_vertices = std::array<gloomy::Float, 36 * 5>{
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    auto plane_vertices = std::array<gloomy::Float, 6 * 5>{
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    auto quad_vertices = std::array<gloomy::Float, 6 * 4>{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    const auto cube_vbo = gloomy::make_ready<gloomy::VertexBuffer>(cube_vertices);
    const auto cube_vao = gloomy::make_generated<gloomy::VertexArray>(learnopengl::Attributes::dynamic());

    gloomy::use([&] { cube_vao.commit(); }, cube_vao, cube_vbo);

    const auto plane_vbo = gloomy::make_ready<gloomy::VertexBuffer>(plane_vertices);
    const auto plane_vao = gloomy::make_generated<gloomy::VertexArray>(learnopengl::Attributes::dynamic());

    gloomy::use([&] { plane_vao.commit(); }, plane_vao, plane_vbo);

    const auto screen_quad_vbo = gloomy::make_ready<gloomy::VertexBuffer>(quad_vertices);
    const auto screen_quad_vao = gloomy::make_generated<gloomy::VertexArray>(learnopengl::ScreenQuadAttributes::dynamic());

    gloomy::use([&] { screen_quad_vao.commit(); }, screen_quad_vao, screen_quad_vbo);

    const auto screen_texture = gloomy::make_ready<gloomy::Texture2D>(gloomy::src::Image{
      .width = SCR_WIDTH,
      .height = SCR_HEIGHT,
      .internal_format = gloomy::TextureInternalFormat::RGB,
		  .pixel_format = gloomy::PixelFormat::RGB
    });
    const auto renderbuffer = gloomy::make_ready<gloomy::Renderbuffer>(SCR_WIDTH, SCR_HEIGHT, gloomy::RenderbufferInternalFormat::DEPTH24_STENCIL8);

    auto framebuffer = gloomy::make_generated<gloomy::Framebuffer>();

    gloomy::use([&] {
      framebuffer.attach_texture2d(screen_texture);
      framebuffer.attach_renderbuffer(renderbuffer);
      framebuffer.commit();

      auto status = gloomy::gl::check_framebuffer_status(gloomy::FramebufferTarget::FRAMEBUFFER);
      if (status != gloomy::FramebufferStatus::FRAMEBUFFER_COMPLETE) {
        std::cout << "learnopengl: Framebuffer is not complete: " << gloomy::from_enum(status) << std::endl;
      }
    }, framebuffer, screen_texture, renderbuffer);

    auto camera = learnopengl::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto last_position = glm::vec2(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
    bool first_mouse = true;
    auto context = learnopengl::CameraContext{camera, last_position, first_mouse};

    learnopengl::setup_camera(context, window);

    auto delta_time = 0.0f;
    auto last_frame = 0.0f;

    learnopengl::loop(window, [&](double time) {
      auto current_frame = static_cast<float>(time);
      delta_time = current_frame - last_frame;
      last_frame = current_frame;
      
      learnopengl::process_input(camera, delta_time, window);

      glm::mat4 view = camera.get_view();
      glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);

      gloomy::use([&] {
        gloomy::gl::enable(gloomy::Capability::DEPTH_TEST);
        gloomy::gl::clear_color(0.1f, 0.1f, 0.1f, 1.0f);
        gloomy::gl::clear(gloomy::BufferBit::COLOR | gloomy::BufferBit::DEPTH);

        shader.uniform("u_view").matrix<4>(glm::value_ptr(view));
        shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));

        auto model = glm::mat4(1.0f);

        gloomy::gl::active_texture(0);
        gloomy::use([&] {
          model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
          shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
          gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, 36);

          model = glm::mat4(1.0f);

          model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
          shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
          gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, 36);
        }, cube_vao, marble);

        gloomy::use([&] {
          model = glm::mat4(1.0f);
          shader.uniform("u_model").matrix<4>(glm::value_ptr(model));
          gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, 6);
        }, plane_vao, metal);
      }, framebuffer, shader);

      gloomy::use([&] {
        gloomy::gl::unbind_framebuffer(gloomy::FramebufferTarget::FRAMEBUFFER);

        gloomy::gl::disable(gloomy::Capability::DEPTH_TEST);
        gloomy::gl::clear_color(1.0f, 1.0f, 1.0f, 1.0f);
        gloomy::gl::clear(gloomy::BufferBit::COLOR);

        gloomy::use([&] {
          gloomy::gl::draw_arrays(gloomy::PrimitiveKind::TRIANGLES, 6);
        }, screen_quad_vao, screen_texture);
      }, screen_shader);

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