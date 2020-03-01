#ifndef CHIP8_EMU_UTIL_RENDERER_H_
#define CHIP8_EMU_UTIL_RENDERER_H_

#include <GL/gl3w.h>

#include <memory>
#include <string>
#include <vector>

namespace chip8_emu {
namespace util {

class Renderer {
 public:
  Renderer();
  ~Renderer();

  void BatchSquare();
  void Draw();

 private:
  GLuint CompileShader(const std::string& shader_content, GLenum shader_type);
  GLuint vao_;
  GLuint vbo_;
  GLuint eab_;
  GLuint vertex_;
  GLuint fragment_;
  GLuint program_;
  std::vector<GLfloat> batched_squares;
};

}  // namespace util
}  // namespace chip8_emu

#endif
