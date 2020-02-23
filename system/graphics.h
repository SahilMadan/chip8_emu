#ifndef CHIP8_EMU_SYSTEM_GRAPHICS_H_
#define CHIP8_EMU_SYSTEM_GRAPHICS_H_

#include <cstdint>
#include <vector>
#include "pixel_state.h"
#include "sprite.h"

#include <array>

namespace chip8_emu {
namespace system {

class Graphics {
 public:
  static constexpr int kHeight = 32;
  static constexpr int kWidth = 64;

  Graphics();
  bool set(int x, int y, PixelState state);
  PixelState get(int x, int y) const;

  bool Draw(int start_x, int start_y, const Sprite& sprite);
  void Clear();

 private:
  std::array<PixelState, kHeight * kWidth> graphics_;
  void AssertPixelPosition(int x, int y) const;
  int GetIndex(int x, int y) const;
};

}  // namespace system
}  // namespace chip8_emu

#endif
