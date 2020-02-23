#include "memory.h"
#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {

Memory::Memory() : memory_{} {}

std::uint8_t Memory::ReadByte(std::size_t byte_pos) const {
  if (byte_pos >= kMemorySize) {
    throw std::out_of_range("Out of range byte_pos for Memory read: " +
                            std::to_string(byte_pos));
  }

  return memory_[byte_pos];
}

void Memory::WriteByte(std::size_t byte_pos, std::uint8_t byte) {
  if (byte_pos >= kMemorySize) {
    throw std::out_of_range("Out of range byte_pos for Memory write: " +
                            std::to_string(byte_pos));
  }

  memory_[byte_pos] = byte;
}

}  // namespace system
}  // namespace chip8_emu
