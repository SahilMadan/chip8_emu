#ifndef CHIP8_EMU_SYSTEM_SPRITE_H_
#define CHIP8_EMU_SYSTEM_SPRITE_H_

#include <array>
#include <vector>
#include "pixel_state.h"

namespace chip8_emu {

class Sprite {
 public:
  Sprite(std::vector<int> bytes);

  PixelState get(int x, int y) const;
  int height() const;
  int width() const;

 private:
  std::vector<std::array<PixelState, 8>> pixels_;

  void AssertPixelPosition(int x, int y) const;
};

}  // namespace chip8_emu

#endif
