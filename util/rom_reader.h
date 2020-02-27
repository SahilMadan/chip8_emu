#ifndef CHIP8_EMU_UTIL_ROM_H_
#define CHIP8_EMU_UTIL_ROM_H_

#include <cstdint>
#include <string>
#include <vector>

namespace chip8_emu {
namespace util {
namespace rom_reader {

std::vector<std::uint8_t> Read(const std::string& loc);

}
}  // namespace util
}  // namespace chip8_emu

#endif
