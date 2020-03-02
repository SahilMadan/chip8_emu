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

  // Batch a square, to be drawn, at location (x, y) with width and height. All
  // values specified as 0.0f - 1.0f of viewport. Assumes (0,0) is top-left.
  void BatchSquare(float x, float y, float width, float height);
  void Draw();

 private:
  GLuint CompileShader(const std::string& shader_content, GLenum shader_type);
  GLuint vao_;
  GLuint vbo_;
  GLuint eab_;
  GLuint vertex_;
  GLuint fragment_;
  GLuint program_;
  std::vector<GLfloat> batched_squares_;
};

}  // namespace util
}  // namespace chip8_emu

#endif
