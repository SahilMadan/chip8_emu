#ifndef CHIP8_EMU_SYSTEM_MEMORY_H_
#define CHIP8_EMU_SYSTEM_MEMORY_H_

#include <array>
#include <cstdio>

namespace chip8_emu {
namespace system {

class Memory {
 public:
  static constexpr std::size_t kMemorySize = 4096;

  Memory();

  std::uint8_t ReadByte(std::size_t byte_pos) const;
  void WriteByte(std::size_t byte_pos, std::uint8_t byte);

 private:
  std::array<std::uint8_t, kMemorySize> memory_;
};

}  // namespace system
}  // namespace chip8_emu

#endif
