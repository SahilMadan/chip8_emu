#include "rom_reader.h"

#include <fstream>
#include <stdexcept>

namespace chip8_emu {
namespace util {
namespace rom_reader {

std::vector<uint8_t> Read(const std::string& loc) {
  std::ifstream rom_stream(loc, std::ios::binary | std::ios::ate);

  if (!rom_stream) {
    throw std::runtime_error("Rom does not exist: " + loc);
  }

  auto end = rom_stream.tellg();
  rom_stream.seekg(0, std::ios::beg);
  auto size = std::size_t(end - rom_stream.tellg());

  if (size == 0) {
    return {};
  }

  auto data = std::vector<std::uint8_t>(size);

  if (!rom_stream.read(reinterpret_cast<char*>(data.data()), data.size())) {
    throw std::runtime_error("Could not read rom: " + loc);
  }

  return data;
}

}  // namespace rom_reader
}  // namespace util
}  // namespace chip8_emu
