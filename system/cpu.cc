#include "cpu.h"

#include <functional>
#include <limits>
#include <random>
#include <thread>
#include <vector>
#include <iostream>

#include "char_sprite_map.h"
#include "sprite.h"

namespace {

// Functor used to decode opcodes and return a stored value. The start value
// indicates the position (from the right) where the requested value begins. The
// end value indicates the position (from the right) where the requested value
// ends (including that position).
//
// For example, using OpcodeDecoder(2, 1) on the opcode XYZW returns YZ (the
// start value, Y, is 2 from the right; the end value, Z, is 1 from the right).
//
// Note that each 'position' is a hex value (i.e 4 bits).
class OpcodeDecoder {
 public:
  OpcodeDecoder(std::size_t start, std::size_t end) {
    std::uint16_t top_bits = std::numeric_limits<std::uint16_t>::max();
    top_bits <<= end * 4;

    std::uint16_t bottom_bits = std::numeric_limits<std::uint16_t>::max();
    bottom_bits <<= (start + 1) * 4;
    bottom_bits = ~bottom_bits;

    mask_ = top_bits & bottom_bits;
    shift_ = end * 4;
  }

  std::uint16_t operator()(std::uint16_t opcode) {
    return (opcode & mask_) >> shift_;
  }

 private:
  std::uint16_t mask_;
  std::size_t shift_;
};

OpcodeDecoder decode_instruction(3, 3);
OpcodeDecoder decode_x(2, 2);
OpcodeDecoder decode_y(1, 1);
OpcodeDecoder decode_n(0, 0);
OpcodeDecoder decode_nn(1, 0);
OpcodeDecoder decode_nnn(2, 0);

// Random number generator in the range [0, FF].
auto random_number =
    std::bind(std::uniform_int_distribution<std::uint16_t>(0, 0xFF),
              std::default_random_engine());

}  // namespace

namespace chip8_emu {
namespace system {
Cpu::Cpu()
    : i_register_(0),
      v_registers_{0},
      pc_{Memory::kRomStartIndex},
      delay_timer_{0},
      sound_timer_{0} {}

void Cpu::RunSingleIteration(Graphics* graphics, Input* input, Memory* memory,
                             Stack* stack, std::mutex* mutex) {
  // TODO(sahilmadan): Replace 500Hz sleep with individual sleep for each
  // instruction.
  auto now = std::chrono::high_resolution_clock::now();
  if (last_iteration_time_point_ == std::chrono::high_resolution_clock::time_point()) {
    last_iteration_time_point_ = now;
  }

  const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                            now - last_iteration_time_point_)
                            .count();
  if (duration < 2000) {
    std::this_thread::sleep_for(std::chrono::microseconds(2000 - duration));
    now = std::chrono::high_resolution_clock::now();
  }
  {
    std::lock_guard<std::mutex> lock(*mutex);
    const auto opcode = GetCurrentOpcode(memory);
    ExecuteOpcode(opcode, graphics, input, memory, stack);
  }

  if (delay_timer_ > 0) {
    const auto delay_duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            now - last_decrement_delay_time_point_)
            .count();
    if (delay_duration < 16667) {
      delay_timer_--;
      last_decrement_delay_time_point_ += std::chrono::microseconds(16667);
    }
  }

  if (sound_timer_ > 0) {
    const auto sound_duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            now - last_decrement_sound_time_point_)
            .count();
    if (sound_duration < 16667) {
      // TODO(sahilmadan): Implement sound
      sound_timer_--;
      last_decrement_sound_time_point_ += std::chrono::microseconds(16667);
    }
  }

  last_iteration_time_point_ += std::chrono::microseconds(2000);
  pc_ += kInstructionNumBytes;
}

std::uint16_t Cpu::GetCurrentOpcode(Memory* memory) {
  return (static_cast<std::uint16_t>(memory->ReadByte(pc_)) << 8) |
         static_cast<std::uint16_t>(
             memory->ReadByte(static_cast<std::size_t>(pc_) + 1));
}

void Cpu::ExecuteOpcode(std::uint16_t opcode, Graphics* graphics, Input* input,
                        Memory* memory, Stack* stack) {
  switch (decode_instruction(opcode)) {
    case 0: {
      const auto nnn = decode_nnn(opcode);
      if (nnn == 0x0E0) {
        ClearGraphics(graphics);
      } else if (nnn == 0x0EE) {
        ReturnFromSubroutine(stack);
      }
    } break;
    case 1:
      JumpToNnn(opcode);
      break;
    case 2:
      ExecuteSubroutineAtNnn(opcode, stack);
      break;
    case 3:
      SkipInstructionIfVxEqNn(opcode);
      break;
    case 4:
      SkipInstructionIfVxNeqNn(opcode);
      break;
    case 5:
      SkipInstructionIfVxEqVy(opcode);
      break;
    case 6:
      StoreNnInVx(opcode);
      break;
    case 7:
      AddNnToVx(opcode);
      break;
    case 8: {
      const auto n = decode_n(opcode);
      switch (n) {
        case 0:
          StoreVyInVx(opcode);
          break;
        case 1:
          SetVxToVxOrVy(opcode);
          break;
        case 2:
          SetVxToVxAndVy(opcode);
          break;
        case 3:
          SetVxToVxXorVy(opcode);
          break;
        case 0x4:
          AddVyToVxVfEqCarry(opcode);
          break;
        case 0x5:
          SubVyFromVxVfEqBorrow(opcode);
          break;
        case 0x6:
          StoreVyShiftRightToVxVfEqLeastSignificantBit(opcode);
          break;
        case 0x7:
          SetVxToVySubVxVfEqNotBorrow(opcode);
          break;
        case 0xE:
          StoreVyShiftLeftToVxVfEqMostSignificantBit(opcode);
          break;
      }
    } break;
    case 0x9:
      SkipInstructionIfVxNeqVy(opcode);
      break;
    case 0xA:
      StoreNnnInI(opcode, memory);
      break;
    case 0xB:
      JumpToNnnAddV0(opcode);
      break;
    case 0xC:
      SetVxToRandAndNn(opcode);
      break;
    case 0xD:
      DrawSprite(opcode, graphics, memory);
      break;
    case 0xE: {
      const auto nn = decode_nn(opcode);
      switch (nn) {
        case 0x9E:
          SkipInstructionIfKeyVxIsPressed(opcode, input);
          break;
        case 0xA1:
          SkipInstructionIfKeyVxIsNotPressed(opcode, input);
          break;
      }
    } break;
    case 0xF: {
      const auto nn = decode_nn(opcode);
      switch (nn) {
        case 0x07:
          StoreDelayTimerInVx(opcode);
          break;
        case 0x0A:
          WaitForKeyPressStoreKeyInVx(opcode, input);
          break;
        case 0x15:
          SetDelayTimerToVx(opcode);
          break;
        case 0x18:
          SetSoundTimerToVx(opcode);
          break;
        case 0x1E:
          AddVxToRegisterI(opcode);
          break;
        case 0x29:
          SetRegisterIToMemoryAddressOfCharSpriteVx(opcode, memory);
          break;
        case 0x33:
          StoreBinaryCodedDecimalAtVxInMemory(opcode, memory);
          break;
        case 0x55:
          StoreV0ToVxInMemory(opcode, memory);
          break;
        case 0x65:
          FillV0ToVxWithMemory(opcode, memory);
          break;
      }
    } break;
  }
}

void Cpu::ClearGraphics(Graphics* graphics) { graphics->Clear(); }

void Cpu::ReturnFromSubroutine(Stack* stack) { pc_ = stack->Pop(); }

void Cpu::JumpToNnn(std::uint16_t opcode) {
  const auto nnn = decode_nnn(opcode);
  // Decrement instruction size to offset later increment.
  pc_ = nnn - kInstructionNumBytes;
}

void Cpu::ExecuteSubroutineAtNnn(std::uint16_t opcode, Stack* stack) {
  stack->Push(pc_);
  pc_ = decode_nnn(opcode) - kInstructionNumBytes;
}

void Cpu::SkipInstructionIfVxEqNn(std::uint16_t opcode) {
  const auto vx = v_registers_[decode_x(opcode)];
  const auto nn = decode_nn(opcode);
  if (vx == nn) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::SkipInstructionIfVxNeqNn(std::uint16_t opcode) {
  const auto vx = v_registers_[decode_x(opcode)];
  const auto nn = decode_nn(opcode);
  if (vx != nn) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::SkipInstructionIfVxEqVy(std::uint16_t opcode) {
  const auto vx = v_registers_[decode_x(opcode)];
  const auto vy = v_registers_[decode_y(opcode)];
  if (vx == vy) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::StoreNnInVx(std::uint16_t opcode) {
  const auto nn = decode_nn(opcode);
  v_registers_[decode_x(opcode)] = nn;
}

void Cpu::AddNnToVx(std::uint16_t opcode) {
  const auto nn = decode_nn(opcode);
  v_registers_[decode_x(opcode)] += nn;
}

void Cpu::StoreVyInVx(std::uint16_t opcode) {
  const auto vy = v_registers_[decode_y(opcode)];
  v_registers_[decode_x(opcode)] = vy;
}

void Cpu::SetVxToVxOrVy(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx = v_registers_[x];
  const auto vy = v_registers_[decode_y(opcode)];
  v_registers_[x] = vx | vy;
}

void Cpu::SetVxToVxAndVy(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx = v_registers_[x];
  const auto vy = v_registers_[decode_y(opcode)];
  v_registers_[x] = vx & vy;
}

void Cpu::SetVxToVxXorVy(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx = v_registers_[x];
  const auto vy = v_registers_[decode_y(opcode)];
  v_registers_[x] = vx ^ vy;
}

void Cpu::AddVyToVxVfEqCarry(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx_prev = v_registers_[x];
  v_registers_[x] = vx_prev + v_registers_[decode_y(opcode)];
  v_registers_[0xF] = (v_registers_[x] < vx_prev) ? 1 : 0;
}

void Cpu::SubVyFromVxVfEqBorrow(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx = v_registers_[x];
  const auto vy = v_registers_[decode_y(opcode)];
  v_registers_[x] = vx - vy;
  v_registers_[0xF] = vy > vx ? 0 : 1;
}

void Cpu::StoreVyShiftRightToVxVfEqLeastSignificantBit(std::uint16_t opcode) {
  const auto vy = v_registers_[decode_y(opcode)];
  const auto lsb = vy & 1;
  v_registers_[decode_x(opcode)] = (vy >> 1);
  v_registers_[0xF] = lsb;
}

void Cpu::SetVxToVySubVxVfEqNotBorrow(std::uint16_t opcode) {
  const auto x = decode_x(opcode);
  const auto vx_prev = v_registers_[x];
  v_registers_[x] = v_registers_[decode_y(opcode)] - vx_prev;
  v_registers_[0xF] = (v_registers_[x] > vx_prev) ? 0 : 1;
}

void Cpu::StoreVyShiftLeftToVxVfEqMostSignificantBit(std::uint16_t opcode) {
  const auto vy = v_registers_[decode_y(opcode)];
  const auto flag = 1 << (8 - 1);
  const auto msb = vy & flag;
  v_registers_[decode_x(opcode)] = (vy << 1);
  v_registers_[0xF] = msb;
}

void Cpu::SkipInstructionIfVxNeqVy(std::uint16_t opcode) {
  if (v_registers_[decode_x(opcode)] != v_registers_[decode_y(opcode)]) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::StoreNnnInI(std::uint16_t opcode, const Memory* memory) {
  i_register_ = decode_nnn(opcode);
}

void Cpu::JumpToNnnAddV0(std::uint16_t opcode) {
  pc_ = decode_nnn(opcode) + v_registers_[0];
  // Decrement instruction size to offset later increment.
  pc_ -= kInstructionNumBytes;
}

void Cpu::SetVxToRandAndNn(std::uint16_t opcode) {
  v_registers_[decode_x(opcode)] = random_number() & decode_nn(opcode);
}

void Cpu::DrawSprite(std::uint16_t opcode, Graphics* graphics, Memory* memory) {
  const auto height = static_cast<std::size_t>(decode_n(opcode));
  std::vector<std::uint8_t> bytes = std::vector<std::uint8_t>(height, 0);
  for (std::size_t i = 0; i < bytes.size(); i++) {
    bytes[i] = memory->ReadByte(i + i_register_);
  }
  Sprite sprite = Sprite(bytes);
  v_registers_[0xF] = graphics->Draw(v_registers_[decode_x(opcode)],
                                     v_registers_[decode_y(opcode)], sprite);
}

void Cpu::SkipInstructionIfKeyVxIsPressed(std::uint16_t opcode, Input* input) {
  const auto vx = v_registers_[decode_x(opcode)];
  const auto isPressed = input->getIsPressed(vx);
  if (isPressed) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::SkipInstructionIfKeyVxIsNotPressed(std::uint16_t opcode,
                                             Input* input) {
  const auto vx = v_registers_[decode_x(opcode)];
  const auto isPressed = input->getIsPressed(vx);
  if (!isPressed) {
    pc_ += kInstructionNumBytes;
  }
}

void Cpu::StoreDelayTimerInVx(std::uint16_t opcode) {
  v_registers_[decode_x(opcode)] = delay_timer_;
}

void Cpu::WaitForKeyPressStoreKeyInVx(std::uint16_t opcode, Input* input) {
  if (auto key = input->GetPressedKey()) {
    v_registers_[decode_x(opcode)] = key.value();
  } else {
    pc_ -= kInstructionNumBytes;
  }
}

void Cpu::SetDelayTimerToVx(std::uint16_t opcode) {
  delay_timer_ = v_registers_[decode_x(opcode)];
  last_decrement_delay_time_point_ = std::chrono::high_resolution_clock::now();
}

void Cpu::SetSoundTimerToVx(std::uint16_t opcode) {
  sound_timer_ = v_registers_[decode_x(opcode)];
}

void Cpu::AddVxToRegisterI(std::uint16_t opcode) {
  const auto vx = v_registers_[decode_x(opcode)];
  i_register_ += vx;
}

void Cpu::SetRegisterIToMemoryAddressOfCharSpriteVx(std::uint16_t opcode,
                                                    Memory* memory) {
  i_register_ =
      char_sprite_map::GetCharSpriteAddr(v_registers_[decode_x(opcode)]);
}

void Cpu::StoreBinaryCodedDecimalAtVxInMemory(std::uint16_t opcode,
                                              Memory* memory) {
  auto binary_coded_decimal =
      static_cast<std::size_t>(v_registers_[decode_x(opcode)]);

  for (int i = 2; i >= 0; i--) {
    memory->WriteByte(static_cast<std::size_t>(i_register_) + i,
                      binary_coded_decimal % 10);
    binary_coded_decimal = binary_coded_decimal / 10;
  }
}

void Cpu::StoreV0ToVxInMemory(std::uint16_t opcode, Memory* memory) {
  auto const x = static_cast<std::size_t>(decode_x(opcode));
  for (std::size_t i = 0; i <= x; i++) {
    memory->WriteByte(i_register_ + i, v_registers_[i]);
  }
  i_register_ = i_register_ + x;
}

void Cpu::FillV0ToVxWithMemory(std::uint16_t opcode, Memory* memory) {
  auto const x = static_cast<std::size_t>(decode_x(opcode));
  for (std::size_t i = 0; i <= x; i++) {
    v_registers_[i] = memory->ReadByte(i_register_ + i);
  }
  i_register_ = i_register_ + x;
}

}  // namespace system
}  // namespace chip8_emu
