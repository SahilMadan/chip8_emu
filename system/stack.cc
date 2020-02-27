#include "stack.h"

#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {

Stack::Stack() : sp_(0) { stack_ = std::vector<std::uint16_t>(kStackSize, 0); }

void Stack::Push(std::uint16_t value) {
  if (sp_ >= stack_.size()) {
    throw std::out_of_range("chip8_emu::system::Stack overflow.");
  }
  stack_[sp_] = value;
  sp_++;
}

std::uint16_t Stack::Pop() {
  if (sp_ == 0) {
    throw std::out_of_range("chip8_emu::system::Stack empty. Cannot pop.");
  }
  sp_--;
  return stack_[sp_];
}

std::uint16_t Stack::Peek() {
  if (sp_ == 0) {
    throw std::out_of_range("chip8_emu::system::Stack empty. Cannot peek");
  }
  return stack_[static_cast<long long>(sp_) - 1];
}

}  // namespace system
}  // namespace chip8_emu
