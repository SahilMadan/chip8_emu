#include "graphics.h"

#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {

Graphics::Graphics() { Clear(); }

void Graphics::Clear() {
  for (PixelState& pixel : graphics_) {
    pixel = PixelState::kOff;
  }
}

bool Graphics::set(int x, int y, PixelState state) {
  AssertPixelPosition(x, y);
  auto& pixel = graphics_[GetIndex(x, y)];
  const auto collision = pixel && state;
  pixel = static_cast<PixelState>(pixel ^ state);
  return collision;
}

PixelState Graphics::get(int x, int y) const {
  AssertPixelPosition(x, y);
  return graphics_[GetIndex(x, y)];
}

bool Graphics::Draw(int start_x, int start_y, const Sprite& sprite) {
  start_x = start_x % kWidth;
  start_y = start_y % kHeight;

  bool collision = false;
  for (int sprite_y = 0; sprite_y < sprite.height(); sprite_y++) {
    if (sprite_y >= kHeight) {
      break;
    }
    for (int sprite_x = 0; sprite_x < sprite.width(); sprite_x++) {
      if (sprite_x >= kWidth) {
        break;
      }

      const auto sprite_pixel = sprite.get(sprite_x, sprite_y);

      const auto x = start_x + sprite_x;
      const auto y = start_y + sprite_y;
      collision = collision || (get(x, y) && sprite_pixel);
      set(x, y, sprite_pixel);
    }
  }
  return collision;
}

void Graphics::AssertPixelPosition(int x, int y) const {
  if (x >= kWidth || y >= kHeight) {
    throw std::out_of_range("Out of range pixel pos: (" + std::to_string(x) +
                            ", " + std::to_string(y) + ")");
  }
}

int Graphics::GetIndex(int x, int y) const { return y * kWidth + x; }

}  // namespace system
}  // namespace chip8_emu
