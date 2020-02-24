#include <string>
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include "pixel_state.h"
#include "sprite.h"
#include "stack.h"
#include "window.h"
#include "rom.h"

int main(int argc, char** argv) {
  std::string window_title = "Test Window";
  int width = 640;
  int height = 480;
  std::string title{"Test Window"};
  chip8_emu::util::Window window(width, height, title);

  chip8_emu::util::Rom rom("Tank.ch8");

  window.MainLoop([]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  });

	return 0;
}
