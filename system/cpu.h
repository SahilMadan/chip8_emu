#ifndef CHIP8_EMU_SYSTEM_CPU_H_
#define CHIP8_EMU_SYSTEM_CPU_H_

#include <array>
#include <cstdint>
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include "stack.h"

namespace chip8_emu {
namespace system {

class Cpu {
 public:
  void RunSingleIteration(Graphics* graphics, Input* input, Memory* memory,
                          Stack* stack);

 private:
  // Registers
  std::uint16_t i_register_;
  std::array<std::uint8_t, 16> v_registers_;
  std::uint16_t pc_;
  std::uint8_t delay_timer_;
  std::uint8_t sound_timer_;

  void ExecuteOpcode(std::uint16_t opcode, Graphics* graphics, Input* input,
                     Memory* memory, Stack* stack);

  // Instruction set.
  void ClearGraphics(Graphics* graphics);
  void ReturnFromSubroutine(Stack* stack);
  void JumpToNnn(std::uint16_t opcode);
  void ExecuteSubroutineAtNnn(std::uint16_t opcode, Stack* stack);
  void SkipInstructionIfVxEqNn(std::uint16_t opcode);
  void SkipInstructionIfVxNeqNn(std::uint16_t opcode);
  void SkipInstructionIfVxEqVy(std::uint16_t opcode);
  void StoreNnInVx(std::uint16_t opcode);
  void AddNnToVx(std::uint16_t opcode);
  void StoreVyInVx(std::uint16_t opcode);
  void SetVxToVxOrVy(std::uint16_t opcode);
  void SetVxToVxAndVy(std::uint16_t opcode);
  void SetVxToVxXorVy(std::uint16_t opcode);
  void AddVyToVxVfEqCarry(std::uint16_t opcode);
  void SubVyFromVxVfEqBorrow(std::uint16_t opcode);
  void StoreVyShiftRightToVxVfEqLeastSignificantBit(std::uint16_t opcode);
  void StoreVyShiftLeftToVxVfEqMostSignificantBit(std::uint16_t opcode);
  void SetVxToVySubVxVfEqNotBorrow(std::uint16_t opcode);
  void SkipInstructionIfVxNeqVy(std::uint16_t opcode);
  void StoreMemoryByteNnnInI(std::uint16_t opcode, const Memory* memory);
  void JumpToNnnAddV0(std::uint16_t opcode);
  void SetVxToRandAndNn(std::uint16_t opcode);
  void DrawSprite(std::uint16_t opcode, Graphics* graphics, Memory* memory);
  void SkipInstructionIfKeyVxIsPressed(std::uint16_t opcode, Input* input);
  void SkipInstructionIfKeyVxIsNotPressed(std::uint16_t opcode, Input* input);
  void StoreDelayTimerInVx(std::uint16_t opcode);
  void WaitForKeyPressStoreKeyInVx(std::uint16_t opcode, Input* input);
  void SetDelayTimerToVx(std::uint16_t opcode);
  void SetSoundTimerToVx(std::uint16_t opcode);
  void AddVxToRegisterI(std::uint16_t opcode);
  void SetRegisterIToMemoryAddressOfSpriteCharVx(std::uint16_t opcode,
                                                 Memory* memory);
  void StoreBinaryCodedDecimalAtVxInMemory(std::uint16_t opcode,
                                           Memory* memory);
  void StoreV0ToVxInMemory(std::uint16_t opcode, Memory* memory);
  void FillV0ToVxWithMemory(std::uint16_t opcode, Memory* memory);

  static constexpr std::size_t kInstructionNumBytes = 2;
};

}  // namespace system
}  // namespace chip8_emu

#endif
