#include "graphics.h"

#include <stdexcept>
#include <string>

constexpr int kWidth = 64;
constexpr int kHeight = 32;

namespace chip8_emu {

const int Graphics::width() { return kWidth; }
const int Graphics::height() { return kHeight; }

Graphics::Graphics() { Reset(); }

void Graphics::Reset() {
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
  start_x = start_x % width();
  start_y = start_y % height();

  bool collision = false;
  for (int sprite_y = 0; sprite_y < sprite.height(); sprite_y++) {
    if (sprite_y >= kHeight) {
      break;
    }
    for (int sprite_x = 0; sprite_x < sprite.width(); sprite_x++) {
      if (sprite_x >= kWidth) {
        break;
      }

      const auto x = start_x + sprite_x;
      const auto y = start_y + sprite_y;
      const auto sprite_pixel = sprite.get(x, y);

      collision = collision || (get(x, y) && sprite_pixel);
      set(x, y, sprite_pixel);
    }
  }
}

void Graphics::AssertPixelPosition(int x, int y) const {
  if (x >= width() || y >= height()) {
    throw std::out_of_range("Out of range pixel pos: (" + std::to_string(x) +
                            ", " + std::to_string(y) + ")");
  }
}

int Graphics::GetIndex(int x, int y) const { return y * width() + x; }

}  // namespace chip8_emu
