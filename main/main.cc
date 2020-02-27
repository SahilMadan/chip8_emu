#include <atomic>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "char_sprite_map.h"
#include "cpu.h"
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include "rom.h"
#include "stack.h"
#include "window.h"

void EmulatorLoop(const chip8_emu::util::Rom* rom, chip8_emu::system::Cpu* cpu,
                  chip8_emu::system::Graphics* graphics,
                  chip8_emu::system::Input* input,
                  chip8_emu::system::Memory* memory,
                  chip8_emu::system::Stack* stack, std::mutex* emu_mutex,
                  std::atomic_bool* is_running) {
  // Initialize memory.
  chip8_emu::system::char_sprite_map::InitCharSpriteInMemory(memory);
  const auto rom_data = rom->get();
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

int main(int argc, char** argv) {
  std::string window_title = "chip8_emu";
  int width = 64 * 30;
  int height = 32 * 30;

  std::mutex emu_mutex;

  chip8_emu::util::Rom rom("Tank.ch8");
  chip8_emu::util::Window window(width, height, window_title);
  chip8_emu::system::Cpu cpu;
  chip8_emu::system::Graphics graphics;
  chip8_emu::system::Input input;
  chip8_emu::system::Memory memory;
  chip8_emu::system::Stack stack;

  std::atomic_bool is_running;
  std::atomic_init(&is_running, true);

  std::thread emu_thread(EmulatorLoop, &rom, &cpu, &graphics, &input, &memory,
                         &stack, &emu_mutex, &is_running);

  window.MainLoop([]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  });

  is_running.store(false);
  emu_thread.join();

  return 0;
}