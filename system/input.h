#ifndef CHIP8_EMU_SYSTEM_INPUT_H_
#define CHIP8_EMU_SYSTEM_INPUT_H_

#include <array>
#include <cstdint>
#include <optional>

namespace chip8_emu {
namespace system {

class Input {
 public:
  Input();

  void setIsPressed(std::uint8_t key, bool state);
  bool getIsPressed(std::uint8_t key) const;
  std::optional<std::uint8_t> GetPressedKey() const;

 private:
  std::array<bool, 16> input_;

  void Reset();
};

}  // namespace system
}  // namespace chip8_emu

#endif
