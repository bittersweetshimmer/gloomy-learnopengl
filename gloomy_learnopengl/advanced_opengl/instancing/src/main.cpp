#include <array>

#include <gloomy/gloomy.hpp>
#include <learnopengl/learnopengl.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr auto SCR_WIDTH = 1280;
constexpr auto SCR_HEIGHT = 720;

namespace learnopengl {
  inline void setup_camera(learnopengl::CameraContext& context, GLFWwindow& window);
  inline void process_input(learnopengl::Camera& camera, float delta_time, GLFWwindow& window);
}

int main(int argc, char *argv[]) {
  return learnopengl::glfw_context(SCR_WIDTH, SCR_HEIGHT, "gloomy", [&](GLFWwindow &window) {
    gloomy::gl::enable(gloomy::Capability::DEPTH_TEST);
    gloomy::gl::enable(gloomy::Capability::CULL_FACE);

    const auto planet_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/planet/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/planet/FragmentShader.hlsl").value())
    );

    const auto asteroid_shader = gloomy::make_ready<gloomy::Program>(
      gloomy::make_ready<gloomy::VertexShader>(learnopengl::load_vertex_shader("assets/shaders/asteroids/VertexShader.hlsl").value()),
      gloomy::make_ready<gloomy::FragmentShader>(learnopengl::load_fragment_shader("assets/shaders/asteroids/FragmentShader.hlsl").value())
    );
    
    auto planet = learnopengl::load_model("assets/models/planet/planet.obj").value();
    auto rock = learnopengl::load_model("assets/models/rock/rock.obj").value();

    constexpr auto amount = 100000;
    
    auto model_matrices = std::make_unique<glm::mat4[]>(amount);

    {
      srand(glfwGetTime()); // initialize random seed	
      float radius = 150.0;
      float offset = 25.0f;

      for (unsigned int i = 0; i < amount; i++) {
          glm::mat4 model = glm::mat4(1.0f);
          // 1. translation: displace along circle with 'radius' in range [-offset, offset]
          float angle = (float)i / (float)amount * 360.0f;
          float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float x = sin(angle) * radius + displacement;
          displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
          displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float z = cos(angle) * radius + displacement;
          model = glm::translate(model, glm::vec3(x, y, z));

          // 2. scale: Scale between 0.05 and 0.25f
          float scale = (rand() % 20) / 100.0f + 0.05;
          model = glm::scale(model, glm::vec3(scale));

          // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
          float rotAngle = (rand() % 360);
          model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

          // 4. now add to list of matrices
          model_matrices[i] = model;
      }
    }

    const auto instance_buffer = gloomy::make_ready<gloomy::VertexBuffer>(gloomy::src::BufferView{
      .data = reinterpret_cast<const std::byte*>(model_matrices.get()),
			.size = amount * sizeof(glm::mat4)
    });

    gloomy::use([&] {
      for (auto& rock_mesh: rock.meshes) {
        auto& vao = rock_mesh.vertex_array;

        gloomy::use([&] { vao.enable_attributes<learnopengl::InstanceAttributes>(); }, vao);
      }
    }, instance_buffer);

    auto camera = learnopengl::Camera(glm::vec3(0.0f, 50.0f, 155.0f));
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

      gloomy::gl::clear_color(0.0f, 0.0f, 0.0f, 1.0f);
      gloomy::gl::clear(gloomy::BufferBit::COLOR | gloomy::BufferBit::DEPTH);

      glm::mat4 view = camera.get_view();
      glm::mat4 projection = glm::perspective(glm::radians(camera.get_fov()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 1000.0f);

      gloomy::use([&] {
        planet_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));
        planet_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

        planet_shader.uniform("u_model").matrix<4>(glm::value_ptr(model));

        planet.draw(planet_shader);
      }, planet_shader);

      gloomy::use([&] {
        asteroid_shader.uniform("u_view").matrix<4>(glm::value_ptr(view));
        asteroid_shader.uniform("u_projection").matrix<4>(glm::value_ptr(projection));

        constexpr auto DIFFUSE_SLOT = 0;
        asteroid_shader.uniform("u_material.diffuse").sampler(DIFFUSE_SLOT);

        rock.meshes[0].material.get().diffuse.get().bind_at_slot(DIFFUSE_SLOT);
        for (auto& rock_mesh: rock.meshes) {
          auto& vao = rock_mesh.vertex_array;
          
          vao.bind();
          glDrawElementsInstanced(GL_TRIANGLES, rock_mesh.indices.size(), GL_UNSIGNED_INT, 0, 100000);
        }

      }, asteroid_shader);
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