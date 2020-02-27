#include "emulator_main.h"

#include <iostream>

#include "char_sprite_map.h"

void EmulatorMain(const std::string& rom_loc, chip8_emu::system::Cpu* cpu,
                  chip8_emu::system::Graphics* graphics,
                  chip8_emu::system::Input* input,
                  chip8_emu::system::Memory* memory,
                  chip8_emu::system::Stack* stack, std::mutex* emu_mutex,
                  std::atomic_bool* is_running) {
  // Initialize memory.
  chip8_emu::system::char_sprite_map::InitCharSpriteInMemory(memory);
  const auto rom_data = chip8_emu::util::rom_reader::Read(rom_loc);
  for (std::size_t i = 0; i < rom_data.size(); i++) {
    memory->WriteByte(memory->kRomStartIndex + i, rom_data[i]);
  }

  // Start main rom loop.
  while ((*is_running).load()) {
    try {
      cpu->RunSingleIteration(graphics, input, memory, stack, emu_mutex);
    } catch (std::exception e) {
      std::cerr << e.what() << std::endl;
      abort();
    }
  }
}