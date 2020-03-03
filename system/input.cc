#include "input.h"

#include <string>

namespace chip8_emu {
namespace system {

Input::Input() { Reset(); }

void Input::Reset() {
  for (bool& key : input_) {
    key = false;
  }
}

void Input::SetIsPressed(std::uint8_t key, bool state) { input_[key] = state; }

bool Input::getIsPressed(std::uint8_t key) const { return input_[key]; }

std::optional<std::uint8_t> Input::GetPressedKey() const {
  for (std::uint8_t key = 0; key < 0xF; key++) {
    if (input_[key]) {
      return std::optional<std::uint8_t>(key);
    }
  }
  return std::nullopt;
}

}  // namespace system
}  // namespace chip8_emu
