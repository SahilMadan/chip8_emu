#include "renderer.h"

#include <GL/gl3w.h>

#include <array>
#include <iostream>
#include <string>

namespace {
std::string kVertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vertex_pos;\n"
    "out vec3 color;\n"
    "void main() {\n"
    "  gl_Position = vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0);\n"
    "  color = vec3(1.0f, 0.5f, 0.2f);\n"
    "}\n";

std::string kFragmentShader =
    "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(color, 1.0f);\n"
    "}\n";
}  // namespace

namespace chip8_emu {
namespace util {

Renderer::Renderer()
    : vao_(0), vbo_(0), eab_(0), vertex_(0), fragment_(0), program_(0) {
  vertex_ = CompileShader(kVertexShader, GL_VERTEX_SHADER);
  fragment_ = CompileShader(kFragmentShader, GL_FRAGMENT_SHADER);
  program_ = glCreateProgram();
  glAttachShader(program_, vertex_);
  glAttachShader(program_, fragment_);
  glLinkProgram(program_);

  GLint success = 0;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetProgramInfoLog(program_, 512, nullptr, &infoLog[0]);
    program_ = 0;
  }

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glGenBuffers(1, &eab_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
};

Renderer::~Renderer() {
  if (vao_) {
    glDeleteVertexArrays(1, &vao_);
  }

  if (vbo_) {
    glDeleteBuffers(1, &vbo_);
  }

  if (eab_) {
    glDeleteBuffers(1, &eab_);
  }

  if (vertex_) {
    glDeleteShader(vertex_);
  }

  if (fragment_) {
    glDeleteShader(fragment_);
  }

  if (program_) {
    glDeleteProgram(program_);
  }
}

void Renderer::BatchSquare(float x, float y, float width, float height) {
  const GLfloat right_x = -1.0f + x * 2.0f;
  const GLfloat top_y = 1.0f - (y * 2.0f);
  const GLfloat left_x = right_x + (width * 2.0f);
  const GLfloat bottom_y = top_y - (2.0f * height);

  std::array<GLfloat, 12> square = {right_x,  bottom_y, 0.0f,    left_x,
                                    bottom_y, 0.0f,     right_x, top_y,
                                    0.0f,     left_x,   top_y,   0.0f};
  batched_squares_.insert(batched_squares_.end(), square.begin(), square.end());
}

void Renderer::Draw() {
  if (!program_) {
    return;
  }

  glEnable(GL_CULL_FACE);
  glUseProgram(program_);

  glBindVertexArray(vao_);
  glBufferData(GL_ARRAY_BUFFER, batched_squares_.size() * sizeof(GLfloat),
               &batched_squares_[0], GL_STATIC_DRAW);

  std::vector<GLuint> indices(batched_squares_.size() / 2);
  GLuint initial = 0;
  for (std::size_t i = 0; i < indices.size(); i += 6) {
    indices[i] = initial;
    indices[i + 1] = initial + 1;
    indices[i + 2] = initial + 2;
    indices[i + 3] = initial + 1;
    indices[i + 4] = initial + 3;
    indices[i + 5] = initial + 2;

    initial += 4;
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               &indices[0], GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  batched_squares_.clear();
}

GLuint Renderer::CompileShader(const std::string& shader_content,
                               GLenum shader_type) {
  const auto shader = glCreateShader(shader_type);
  const auto content = shader_content.c_str();
  glShaderSource(shader, 1, &content, nullptr);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, &infoLog[0]);
    std::cerr << "Error compiling shader (" << std::hex << shader_type
              << "): " << infoLog << "\n";
    return 0;
  }
  return shader;
}

}  // namespace util
}  // namespace chip8_emu