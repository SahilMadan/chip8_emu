#include "input.h"
#include <stdexcept>
#include <string>

namespace chip8_emu {

Input::Input() { Reset(); }

void Input::Reset() {
  for (bool& key : input_) {
    key = false;
  }
}

void Input::set(int key, bool state) {
  AssertRange(key);
  input_[key] = state;
}

bool Input::get(int key) const {
  AssertRange(key);
  return input_[key];
}

void Input::AssertRange(int key) const {
  if (key >= static_cast<int>(input_.size())) {
    throw std::out_of_range("Key out of range: " + std::to_string(key));
  }
}

}  // namespace chip8_emu
