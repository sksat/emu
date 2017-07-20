#include "modrm.h"

namespace x86 {

ModRM::ModRM(x86::Emulator *e) : emu(e) {}

ModRM::ModRM(x86::Emulator *e, uint8_t modrm) : emu(e), _modrm(modrm) {}

}
