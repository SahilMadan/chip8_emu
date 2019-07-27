#ifndef CHIP8_EMU_SYSTEM_MEMORY_H_
#define CHIP8_EMU_SYSTEM_MEMORY_H_

#include <cstdio>
#include <vector>

namespace chip8_emu {
namespace system {

class Memory {
 public:
  Memory(std::size_t num_bytes);

  std::uint8_t ReadByte(std::size_t byte_pos) const;
  void WriteByte(std::size_t byte_pos, std::uint8_t byte);

 private:
  std::vector<std::uint8_t> memory_;
};

}  // namespace system
}  // namespace chip8_emu

#endif
