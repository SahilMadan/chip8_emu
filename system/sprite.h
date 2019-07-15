#ifndef CHIP8_EMU_SYSTEM_SPRITE_H_
#define CHIP8_EMU_SYSTEM_SPRITE_H_

#include <array>
#include <cstdint>
#include <vector>
#include "pixel_state.h"

namespace chip8_emu {
namespace system {

class Sprite {
 public:
  Sprite(std::vector<std::uint8_t> bytes);

  PixelState get(int x, int y) const;
  int height() const;
  int width() const;

 private:
  std::vector<std::array<PixelState, 8>> pixels_;

  void AssertPixelPosition(int x, int y) const;
};

}  // namespace system
}  // namespace chip8_emu

#endif
