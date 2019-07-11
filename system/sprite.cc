#include "sprite.h"

#include <stdexcept>
#include <string>

namespace {
const int kMaxHeight = 15;
}

namespace chip8_emu {
namespace system {

Sprite::Sprite(std::vector<int> bytes) {
  const int height = static_cast<int>(bytes.size());
  if (height > kMaxHeight) {
    throw std::out_of_range("Invalid sprite height: " +
                            std::to_string(bytes.size()));
  }

  for (const int& byte : bytes) {
    std::array<PixelState, 8> row;
    for (int i = 0; i < 8; i++) {
      const PixelState pixel_state =
          static_cast<PixelState>((byte >> (7 - i)) & 0x1);
      row[i] = pixel_state;
    }
    pixels_.push_back(std::move(row));
  }
}

PixelState Sprite::get(int x, int y) const {
  AssertPixelPosition(x, y);
  return pixels_[y][x];
}

int Sprite::height() const { return static_cast<int>(pixels_.size()); }
int Sprite::width() const { return 8; }

void Sprite::AssertPixelPosition(int x, int y) const {
  if (x < 0 || x >= 8 || y <= 0 || y >= height()) {
    throw std::out_of_range(
        "Invalid Sprite Position (height = " + std::to_string(pixels_.size()) +
        "): (" + std::to_string(x) + ", " + std::to_string(y) + ")");
  }
}
}  // namespace system
}  // namespace chip8_emu
