#include "stack.h"

#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {

Stack::Stack(int stack_size) : sp_(0) {
  if (stack_size < 12) {
    throw std::out_of_range("Invalid stack size: " +
                            std::to_string(stack_size));
  }
  stack_ = std::vector<int>(stack_size, 0);
}

void Stack::Push(int value) {
  if (sp_ >= stack_.size()) {
    throw std::out_of_range("chip8_emu::system::Stack overflow.");
  }
  stack_[sp_] = value;
  sp_++;
}

int Stack::Pop() {
  if (sp_ == 0) {
    throw std::out_of_range("chip8_emu::system::Stack empty. Cannot pop.");
  }
  sp_--;
  return stack_[sp_];
}

int Stack::Peek() {
  if (sp_ == 0) {
    throw std::out_of_range("chip8_emu::system::Stack empty. Cannot peek");
  }
  return stack_[static_cast<long long>(sp_) - 1];
}

}  // namespace system
}  // namespace chip8_emu
