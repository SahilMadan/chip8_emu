#include "char_sprite_map.h"

#include <stdexcept>
#include <string>

namespace chip8_emu {
namespace system {
namespace char_sprite_map {

void InitCharSpriteInMemory(Memory* memory) {
  int memory_addr = 0;

  // Sprite 0
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 1
  memory->WriteByte(memory_addr++, 0x20);
  memory->WriteByte(memory_addr++, 0x60);
  memory->WriteByte(memory_addr++, 0x20);
  memory->WriteByte(memory_addr++, 0x20);
  memory->WriteByte(memory_addr++, 0x70);

  // Sprite 2
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 3
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 4
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0x10);

  // Sprite 5
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 6
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 7
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0x20);
  memory->WriteByte(memory_addr++, 0x40);
  memory->WriteByte(memory_addr++, 0x40);

  // Sprite 8
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite 9
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x10);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite A
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);

  // Sprite B
  memory->WriteByte(memory_addr++, 0xE0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xE0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xE0);

  // Sprite C
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite D
  memory->WriteByte(memory_addr++, 0xE0);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0x90);
  memory->WriteByte(memory_addr++, 0xE0);

  // Sprite E
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);

  // Sprite F
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0xF0);
  memory->WriteByte(memory_addr++, 0x80);
  memory->WriteByte(memory_addr++, 0x80);
}

std::size_t GetCharSpriteAddr(std::uint8_t character) {
  if (character > 0xF) {
    throw std::out_of_range("Invalid sprite character: " +
                            std::to_string(character));
  }

  return character * 5;
}

}  // namespace char_sprite_map
}  // namespace system
}  // namespace chip8_emu
