#ifndef CHIP8_EMU_UTIL_ROM_H_
#define CHIP8_EMU_UTIL_ROM_H_

#include <cstdint>
#include <string>
#include <vector>

namespace chip8_emu {
namespace util {

class Rom {
 public:
  Rom(const std::string& filename);

 private:
  std::vector<std::uint8_t> data_;
};

}
}  // namespace chip8_emu

#endif
