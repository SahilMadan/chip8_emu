#include <atomic>
#include <exception>
#include <mutex>
#include <string>
#include <thread>

#include "char_sprite_map.h"
#include "cpu.h"
#include "emulator_main.h"
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include "renderer.h"
#include "rom_reader.h"
#include "stack.h"
#include "window.h"

int main(int argc, char** argv) {
  std::string window_title = "chip8_emu";

  const int kPixelSize = 30;
  const int kWindowWidth = chip8_emu::system::Graphics::kWidth * kPixelSize;
  const int kWindowHeight = chip8_emu::system::Graphics::kHeight * kPixelSize;

  std::mutex emu_mutex;
  chip8_emu::util::Window window(kWindowWidth, kWindowHeight, window_title);
  chip8_emu::util::Renderer renderer;
  chip8_emu::system::Cpu cpu;
  chip8_emu::system::Graphics graphics;
  chip8_emu::system::Input input;
  chip8_emu::system::Memory memory;
  chip8_emu::system::Stack stack;

  std::atomic_bool is_running;
  std::atomic_init(&is_running, true);

  const std::string rom_title = "Tank.ch8";

  std::thread emu_thread(EmulatorMain, rom_title, &cpu, &graphics, &input,
                         &memory, &stack, &emu_mutex, &is_running);

  const float kRenderPixelWidth =
      1.0f / static_cast<float>(chip8_emu::system::Graphics::kWidth);
  const float kRenderPixelHeight =
      1.0f / static_cast<float>(chip8_emu::system::Graphics::kHeight);

  window.MainLoop([&renderer, kRenderPixelWidth, kRenderPixelHeight]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.BatchSquare(0.0f, 0.0f, kRenderPixelWidth, kRenderPixelHeight);
    renderer.BatchSquare(
        kRenderPixelWidth * (chip8_emu::system::Graphics::kWidth - 1), 0.0f,
        kRenderPixelWidth, kRenderPixelHeight);
    renderer.BatchSquare(
        0.0f, kRenderPixelHeight * (chip8_emu::system::Graphics::kHeight - 1),
        kRenderPixelWidth, kRenderPixelHeight);
    renderer.BatchSquare(
        kRenderPixelWidth * (chip8_emu::system::Graphics::kWidth - 1),
        kRenderPixelHeight * (chip8_emu::system::Graphics::kHeight - 1),
        kRenderPixelWidth, kRenderPixelHeight);
    renderer.Draw();
  });

  is_running.store(false);
  emu_thread.join();

  return 0;
}