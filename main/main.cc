#include <atomic>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

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

constexpr float kRenderPixelWidth =
    1.0f / static_cast<float>(chip8_emu::system::Graphics::kWidth);
constexpr float kRenderPixelHeight =
    1.0f / static_cast<float>(chip8_emu::system::Graphics::kHeight);

std::vector<chip8_emu::util::Keyboard> key_map;
void BuildKeyMap();

int main(int argc, char** argv) {
  std::string window_title = "chip8_emu";

  const int kPixelSize = 30;
  const int kWindowWidth = chip8_emu::system::Graphics::kWidth * kPixelSize;
  const int kWindowHeight = chip8_emu::system::Graphics::kHeight * kPixelSize;

  BuildKeyMap();

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

  const std::string rom_title = "Pong.ch8";

  std::thread emu_thread(EmulatorMain, rom_title, &cpu, &graphics, &input,
                         &memory, &stack, &emu_mutex, &is_running);

  window.Run([&renderer, &graphics, &emu_mutex, &window, &input]() {
    {
      std::unique_lock<std::mutex> lock(emu_mutex);
      for (std::uint8_t key = 0; key <= 0xF; key++) {
        input.SetIsPressed(key, window.IsPressed(key_map[static_cast<std::size_t>(key)]));
      }
      for (auto i = 0; i < chip8_emu::system::Graphics::kWidth; i++) {
        for (auto j = 0; j < chip8_emu::system::Graphics::kHeight; j++) {
          if (graphics.get(i, j)) {
            renderer.BatchSquare(i * kRenderPixelWidth, j * kRenderPixelHeight,
                                 kRenderPixelWidth, kRenderPixelHeight);
          }
        }
      }
    }
    renderer.Draw();
  });

  is_running.store(false);
  emu_thread.join();

  return 0;
}

void BuildKeyMap() {
  key_map.clear();
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_X);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_1);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_2);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_3);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_Q);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_W);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_E);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_A);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_S);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_D);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_Z);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_C);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_4);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_R);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_F);
  key_map.push_back(chip8_emu::util::Keyboard::KEYBOARD_KEY_V);
}