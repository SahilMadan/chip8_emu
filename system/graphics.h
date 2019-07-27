#ifndef CHIP8_EMU_SYSTEM_GRAPHICS_H_
#define CHIP8_EMU_SYSTEM_GRAPHICS_H_

#include <cstdint>
#include <vector>
#include "pixel_state.h"
#include "sprite.h"

namespace chip8_emu {
namespace system {

class Graphics {
 public:
  Graphics();
  bool set(int x, int y, PixelState state);
  PixelState get(int x, int y) const;

  bool Draw(int start_x, int start_y, const Sprite& sprite);
  void Clear();

  static const int width();
  static const int height();

 private:
  std::vector<PixelState> graphics_;
  void AssertPixelPosition(int x, int y) const;
  int GetIndex(int x, int y) const;
};

}  // namespace system
}  // namespace chip8_emu

#endif
