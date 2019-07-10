#ifndef CHIP8_EMU_SYSTEM_INPUT_H_
#define CHIP8_EMU_SYSTEM_INPUT_H_

#include <array>

namespace chip8_emu {

class Input {
 public:
  Input();

  void set(int key, bool state);
  bool get(int key);

 private:
  std::array<bool, 16> input_;

  void Reset();
  void AssertRange(int key);
};

}  // namespace chip8_emu

#endif
