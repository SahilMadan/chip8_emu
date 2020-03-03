#include "window.h"

#include <iostream>

namespace chip8_emu {
namespace util {

Window::Window(int width, int height, const std::string& title)
    : window_(nullptr) {
  if (!glfwInit()) {
    std::cerr << "unable to initialize GLFW.\n";
    return;
  }

  // Initialize OpenGL context.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Initialize window.
  glfwSetErrorCallback(Window::glfwErrorCallback);
  window_ = glfwCreateWindow(width, height, title.c_str(),
                             /* monitor */ nullptr, /* share */ nullptr);
  if (!window_) {
    std::cerr << "GLFW window initialization failed.\n";
    return;
  }

  glfwMakeContextCurrent(window_);

  // Initialize gl3w.
  if (gl3wInit()) {
    std::cerr << "Unable to initailize gl3w.\n";
    return;
  }

  if (!gl3wIsSupported(3, 3)) {
    std::cerr << "OpenGL 3.3 is not supported.\n";
    return;
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window_,
                                 Window::glfwFramebufferResizeCallback);
}

Window::~Window() {
  if (window_) {
    glfwDestroyWindow(window_);
  }
  glfwTerminate();
}

void Window::Run(std::function<void()> loop_task) {
  while (!glfwWindowShouldClose(window_)) {
    ProcessInput();
    loop_task();
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

bool Window::IsPressed(Keyboard key) const {
  return glfwGetKey(window_, static_cast<int>(key));
}

void Window::glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW error(" << error << "): " << description << "\n";
}

void Window::glfwFramebufferResizeCallback(GLFWwindow* window, int width,
                                           int height) {
  glViewport(0, 0, width, height);
}

void Window::ProcessInput() {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window_, true);
  }
}

}  // namespace util
}  // namespace chip8_emu