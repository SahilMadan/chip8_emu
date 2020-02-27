#ifndef CHIP8_EMU_MAIN_EMULATOR_MAIN_H_
#define CHIP8_EMU_MAIN_EMULATOR_MAIN_H_

#include <atomic>
#include <mutex>
#include <string>

#include "cpu.h"
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include "rom_reader.h"
#include "stack.h"
#include "window.h"

void EmulatorMain(const std::string& rom_loc, chip8_emu::system::Cpu* cpu,
                  chip8_emu::system::Graphics* graphics,
                  chip8_emu::system::Input* input,
                  chip8_emu::system::Memory* memory,
                  chip8_emu::system::Stack* stack, std::mutex* emu_mutex,
                  std::atomic_bool* is_running);

#endif