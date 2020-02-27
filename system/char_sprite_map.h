#ifndef CHIP8_EMU_SYSTEM_CHAR_SPRITE_MAP_H_
#define CHIP8_EMU_SYSTEM_CHAR_SPRITE_MAP_H_

#include <cstdint>

#include "memory.h"

namespace chip8_emu {
namespace system {
namespace char_sprite_map {

void InitCharSpriteInMemory(Memory* memory);

std::size_t GetCharSpriteAddr(std::uint8_t character);

}  // namespace char_sprite_map
}  // namespace system
}  // namespace chip8_emu

#endif
