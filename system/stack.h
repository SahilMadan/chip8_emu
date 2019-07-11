#ifndef CHIP8_EMU_SYSTEM_STACK_H_
#define CHIP8_EMU_SYSTEM_STACK_H_

#include <vector>

namespace chip8_emu {
namespace system {

class Stack {
 public:
  Stack(int stack_size);

  void Push(int value);
  int Pop();
  int Peek();

 private:
  std::vector<int> stack_;
  int sp_;
};

}  // namespace system
}  // namespace chip8_emu

#endif
