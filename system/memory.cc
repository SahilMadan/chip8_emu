#include "memory.h"
#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {

Memory::Memory(std::size_t num_bytes) {
  memory_ = std::vector<std::uint8_t>(num_bytes, 0);
}

std::uint8_t Memory::ReadByte(std::size_t byte_pos) const {
  if (byte_pos >= memory_.size()) {
    throw std::out_of_range("Out of range byte_pos for Memory read: " +
                            std::to_string(byte_pos));
  }

  return memory_[byte_pos];
}

void Memory::WriteByte(std::size_t byte_pos, std::uint8_t byte) {
  if (byte_pos >= memory_.size()) {
    throw std::out_of_range("Out of range byte_pos for Memory write: " +
                            std::to_string(byte_pos));
  }
}

}  // namespace system
}  // namespace chip8_emu
