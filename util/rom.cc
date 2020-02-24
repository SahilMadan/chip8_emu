#include "rom.h"

#include <fstream>
#include <stdexcept>

namespace chip8_emu {
namespace util {

Rom::Rom(const std::string& filename) {
  std::ifstream rom_stream(filename, std::ios::binary | std::ios::ate);

  if (!rom_stream) {
    throw std::runtime_error("Rom does not exist: " + filename);
  }

  auto end = rom_stream.tellg();
  rom_stream.seekg(0, std::ios::beg);
  auto size = std::size_t(end - rom_stream.tellg());

  if (size == 0) {
    data_ = {};
    return;
  }

  data_ = std::vector<std::uint8_t>(size);

  if (!rom_stream.read(reinterpret_cast<char*>(data_.data()), data_.size())) {
    throw std::runtime_error("Could not read rom: " + filename);
  }
}

}  // namespace util
}  // namespace chip8_emu
