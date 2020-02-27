#ifndef CHIP8_EMU_SYSTEM_STACK_H_
#define CHIP8_EMU_SYSTEM_STACK_H_

#include <cstdint>
#include <vector>

namespace chip8_emu {
namespace system {

class Stack {
 public:
  static constexpr int kStackSize = 12;
  Stack();

  void Push(std::uint16_t value);
  std::uint16_t Pop();
  std::uint16_t Peek();

 private:
  std::vector<std::uint16_t> stack_;
  int sp_;
};

}  // namespace system
}  // namespace chip8_emu

#endif
