#ifndef CHIP8_EMU_UTIL_WINDOW_H_
#define CHIP8_EMU_UTIL_WINDOW_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace chip8_emu {
namespace util {

class Window {
 public:
  Window(int width, int height, const std::string& title);
  ~Window();

  void MainLoop(std::function<void()> render_task);

 private:
  static void glfwErrorCallback(int error, const char* description);
  static void glfwFramebufferResizeCallback(GLFWwindow* window, int width,
                                            int height);

  void ProcessInput();

  GLFWwindow* window_;
};

}  // namespace util
}  // namespace chip8_emu

#endif