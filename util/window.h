#ifndef CHIP8_EMU_UTIL_WINDOW_H_
#define CHIP8_EMU_UTIL_WINDOW_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace chip8_emu {
namespace util {

enum class Keyboard {
  KEYBOARD_KEY_1 = GLFW_KEY_1,
  KEYBOARD_KEY_2 = GLFW_KEY_2,
  KEYBOARD_KEY_3 = GLFW_KEY_3,
  KEYBOARD_KEY_4 = GLFW_KEY_4,
  KEYBOARD_KEY_Q = GLFW_KEY_Q,
  KEYBOARD_KEY_W = GLFW_KEY_W,
  KEYBOARD_KEY_E = GLFW_KEY_E,
  KEYBOARD_KEY_R = GLFW_KEY_R,
  KEYBOARD_KEY_A = GLFW_KEY_A,
  KEYBOARD_KEY_S = GLFW_KEY_S,
  KEYBOARD_KEY_D = GLFW_KEY_D,
  KEYBOARD_KEY_F = GLFW_KEY_F,
  KEYBOARD_KEY_Z = GLFW_KEY_Z,
  KEYBOARD_KEY_X = GLFW_KEY_X,
  KEYBOARD_KEY_C = GLFW_KEY_C,
  KEYBOARD_KEY_V = GLFW_KEY_V
};

class Window {
 public:
  Window(int width, int height, const std::string& title);
  ~Window();

  void Run(std::function<void()> render_task);
  bool IsPressed(Keyboard key) const;

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