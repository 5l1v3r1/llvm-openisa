//=== SBTUtils.cpp - General utilities ------------------*- C++ -*-==//
// 
// Convenience functions to convert register numbers when reading
// an OpenISA binary and converting it to IR.
//
//===------------------------------------------------------------===//

#include "InterpUtils.h"
#include "../lib/Target/Mips/MipsInstrInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Object/ELF.h"

namespace llvm {

bool error(std::error_code ec) {
  if (!ec) return false;

  outs() << "error reading file: " << ec.message() << ".\n";
  outs().flush();
  return true;
}

void DumpBytes(StringRef bytes) {
  static const char hex_rep[] = "0123456789abcdef";
  // FIXME: The real way to do this is to figure out the longest instruction
  //        and align to that size before printing. I'll fix this when I get
  //        around to outputting relocations.
  // 15 is the longest x86 instruction
  // 3 is for the hex rep of a byte + a space.
  // 1 is for the null terminator.
  enum { OutputSize = (15 * 3) + 1 };
  char output[OutputSize];

  assert(bytes.size() <= 15
    && "DumpBytes only supports instructions of up to 15 bytes");
  memset(output, ' ', sizeof(output));
  unsigned index = 0;
  for (StringRef::iterator i = bytes.begin(),
                           e = bytes.end(); i != e; ++i) {
    output[index] = hex_rep[(*i & 0xF0) >> 4];
    output[index + 1] = hex_rep[*i & 0xF];
    index += 3;
  }

  output[sizeof(output) - 1] = 0;
  outs() << output;
}

unsigned conv32(unsigned regnum) {
  switch(regnum) {
  case Mips::AT_64:
    return Mips::AT;
  case Mips::FP_64:
    return Mips::FP;
  case Mips::SP_64:
    return Mips::SP;
  case Mips::RA_64:
    return Mips::RA;
  case Mips::ZERO_64:
    return Mips::ZERO;
  case Mips::GP_64:
    return Mips::GP;
  case Mips::A0_64:
    return Mips::A0;
  case Mips::A1_64:
    return Mips::A1;
  case Mips::A2_64:
    return Mips::A2;
  case Mips::A3_64:
    return Mips::A3;
  case Mips::V0_64:
    return Mips::V0;
  case Mips::V1_64:
    return Mips::V1;
  case Mips::S0_64:
    return Mips::S0;
  case Mips::S1_64:
    return Mips::S1;
  case Mips::S2_64:
    return Mips::S2;
  case Mips::S3_64:
    return Mips::S3;
  case Mips::S4_64:
    return Mips::S4;
  case Mips::S5_64:
    return Mips::S5;
  case Mips::S6_64:
    return Mips::S6;
  case Mips::S7_64:
    return Mips::S7;
  case Mips::K0_64:
    return Mips::K0;
  case Mips::K1_64:
    return Mips::K1;
  case Mips::T0_64:
    return Mips::T0;
  case Mips::T1_64:
    return Mips::T1;
  case Mips::T2_64:
    return Mips::T2;
  case Mips::T3_64:
    return Mips::T3;
  case Mips::T4_64:
    return Mips::T4;
  case Mips::T5_64:
    return Mips::T5;
  case Mips::T6_64:
    return Mips::T6;
  case Mips::T7_64:
    return Mips::T7;
  case Mips::T8_64:
    return Mips::T8;
  case Mips::T9_64:
    return Mips::T9;
  }
  return regnum;
}

unsigned ConvFromDirective(unsigned regnum) {
  switch(regnum) {
  case 0:
    return Mips::ZERO;
  case 1:
    return Mips::AT;
  case 4:
    return Mips::A0;
  case 5:
    return Mips::A1;
  case 6:
    return Mips::A2;
  case 7:
    return Mips::A3;
  case 2:
    return Mips::V0;
  case 3:
    return Mips::V1;
  case 16:
    return Mips::S0;
  case 17:
    return Mips::S1;
  case 18:
    return Mips::S2;
  case 19:
    return Mips::S3;
  case 20:
    return Mips::S4;
  case 21:
    return Mips::S5;
  case 22:
    return Mips::S6;
  case 23:
    return Mips::S7;
  case 26:
    return Mips::K0;
  case 27:
    return Mips::K1;
  case 29:
    return Mips::SP;
  case 30:
    return Mips::FP;
  case 28:
    return Mips::GP;
  case 31:
    return Mips::RA;
  case 8:
    return Mips::T0;
  case 9:
    return Mips::T1;
  case 10:
    return Mips::T2;
  case 11:
    return Mips::T3;
  case 12:
    return Mips::T4;
  case 13:
    return Mips::T5;
  case 14:
    return Mips::T6;
  case 15:
    return Mips::T7;
  case 24:
    return Mips::T8;
  case 25:
    return Mips::T9;
  }
  llvm_unreachable("Invalid register");
  return -1;
}

unsigned ConvToDirective(unsigned regnum) {
  switch(regnum) {
  case Mips::ZERO:
    return 0;
  case Mips::AT:
    return 1;
  case Mips::A0:
    return 4;
  case Mips::A1:
    return 5;
  case Mips::A2:
    return 6;
  case Mips::A3:
    return 7;
  case Mips::V0:
    return 2;
  case Mips::V1:
    return 3;
  case Mips::S0:
    return 16;
  case Mips::S1:
    return 17;
  case Mips::S2:
    return 18;
  case Mips::S3:
    return 19;
  case Mips::S4:
    return 20;
  case Mips::S5:
    return 21;
  case Mips::S6:
    return 22;
  case Mips::S7:
    return 23;
  case Mips::K0:
    return 26;
  case Mips::K1:
    return 27;
  case Mips::SP:
    return 29;
  case Mips::FP:
    return 30;
  case Mips::GP:
    return 28;
  case Mips::RA:
    return 31;
  case Mips::T0:
    return 8;
  case Mips::T1:
    return 9;
  case Mips::T2:
    return 10;
  case Mips::T3:
    return 11;
  case Mips::T4:
    return 12;
  case Mips::T5:
    return 13;
  case Mips::T6:
    return 14;
  case Mips::T7:
    return 15;
  case Mips::T8:
    return 24;
  case Mips::T9:
    return 25;
  case Mips::R32:
    return 32;
  case Mips::R33:
    return 33;
  case Mips::R34:
    return 34;
  case Mips::R35:
    return 35;
  case Mips::R36:
    return 36;
  case Mips::R37:
    return 37;
  case Mips::R38:
    return 38;
  case Mips::R39:
    return 39;
  case Mips::R40:
    return 40;
  case Mips::R41:
    return 41;
  case Mips::R42:
    return 42;
  case Mips::R43:
    return 43;
  case Mips::R44:
    return 44;
  case Mips::R45:
    return 45;
  case Mips::R46:
    return 46;
  case Mips::R47:
    return 47;
  case Mips::R48:
    return 48;
  case Mips::R49:
    return 49;
  case Mips::R50:
    return 50;
  case Mips::R51:
    return 51;
  case Mips::R52:
    return 52;
  case Mips::R53:
    return 53;
  case Mips::R54:
    return 54;
  case Mips::R55:
    return 55;
  case Mips::R56:
    return 56;
  case Mips::R57:
    return 57;
  case Mips::R58:
    return 58;
  case Mips::R59:
    return 59;
  case Mips::R60:
    return 60;
  case Mips::R61:
    return 61;
  case Mips::R62:
    return 62;
  case Mips::R63:
    return 63;


    // Floating point registers
  case Mips::D0:
  case Mips::F0:
    return 34;
  case Mips::F1:
    return 35;
  case Mips::D1:
  case Mips::F2:
    return 36;
  case Mips::F3:
    return 37;
  case Mips::D2:
  case Mips::F4:
    return 38;
  case Mips::F5:
    return 39;
  case Mips::D3:
  case Mips::F6:
    return 40;
  case Mips::F7:
    return 41;
  case Mips::D4:
  case Mips::F8:
    return 42;
  case Mips::F9:
    return 43;
  case Mips::D5:
  case Mips::F10:
    return 44;
  case Mips::F11:
    return 45;
  case Mips::D6:
  case Mips::F12:
    return 46;
  case Mips::F13:
    return 47;
  case Mips::D7:
  case Mips::F14:
    return 48;
  case Mips::F15:
    return 49;
  case Mips::D8:
  case Mips::F16:
    return 50;
  case Mips::F17:
    return 51;
  case Mips::D9:
  case Mips::F18:
    return 52;
  case Mips::F19:
    return 53;
  case Mips::D10:
  case Mips::F20:
    return 54;
  case Mips::F21:
    return 55;
  case Mips::D11:
  case Mips::F22:
    return 56;
  case Mips::F23:
    return 57;
  case Mips::D12:
  case Mips::F24:
    return 58;
  case Mips::F25:
    return 59;
  case Mips::D13:
  case Mips::F26:
    return 60;
  case Mips::F27:
    return 61;
  case Mips::D14:
  case Mips::F28:
    return 62;
  case Mips::F29:
    return 63;
  case Mips::D15:
  case Mips::F30:
    return 64;
  case Mips::F31:
    return 65;
  case Mips::D16:
  case Mips::F32:
    return 66;
  case Mips::F33:
    return 67;
  case Mips::D17:
  case Mips::F34:
    return 68;
  case Mips::F35:
    return 69;
  case Mips::D18:
  case Mips::F36:
    return 70;
  case Mips::F37:
    return 71;
  case Mips::D19:
  case Mips::F38:
    return 72;
  case Mips::F39:
    return 73;
  case Mips::D20:
  case Mips::F40:
    return 74;
  case Mips::F41:
    return 75;
  case Mips::D21:
  case Mips::F42:
    return 76;
  case Mips::F43:
    return 77;
  case Mips::D22:
  case Mips::F44:
    return 78;
  case Mips::F45:
    return 79;
  case Mips::D23:
  case Mips::F46:
    return 80;
  case Mips::F47:
    return 81;
  case Mips::D24:
  case Mips::F48:
    return 82;
  case Mips::F49:
    return 83;
  case Mips::D25:
  case Mips::F50:
    return 84;
  case Mips::F51:
    return 85;
  case Mips::D26:
  case Mips::F52:
    return 86;
  case Mips::F53:
    return 87;
  case Mips::D27:
  case Mips::F54:
    return 88;
  case Mips::F55:
    return 89;
  case Mips::D28:
  case Mips::F56:
    return 90;
  case Mips::F57:
    return 91;
  case Mips::D29:
  case Mips::F58:
    return 92;
  case Mips::F59:
    return 93;
  case Mips::D30:
  case Mips::F60:
    return 94;
  case Mips::F61:
    return 95;
  case Mips::D31:
  case Mips::F62:
    return 96;
  case Mips::F63:
    return 97;

  }
  llvm_unreachable("Invalid register");
  return -1;
}

unsigned ConvToDirectiveDbl(unsigned regnum) {
  return (ConvToDirective(regnum) - 34) >> 1;
}

uint64_t GetELFOffset(const SectionRef &i) {
  DataRefImpl Sec = i.getRawDataRefImpl();
  const object::Elf_Shdr_Impl<object::ELFType<support::little, 2, false> > *sec =
    reinterpret_cast<const object::Elf_Shdr_Impl<object::ELFType<support::little, 2, false> > *>(Sec.p);
  return sec->sh_offset;
}

std::vector<std::pair<uint64_t, StringRef> > GetSymbolsList(const ObjectFile *Obj) {
  std::error_code ec;
  // Make a list of all the symbols in this section.
  std::vector<std::pair<uint64_t, StringRef> > Symbols;
  for (const auto &si : Obj->symbols()) {
    uint64_t Address;
    if (error(si.getAddress(Address))) break;
    if (Address == UnknownAddressOrSize) continue;

    StringRef Name;
    if (error(si.getName(Name))) break;
    Symbols.push_back(std::make_pair(Address, Name));
  }

  // Sort the symbols by address, just in case they didn't come in that way.
  array_pod_sort(Symbols.begin(), Symbols.end());
  return Symbols;
}

Value* GetFirstInstruction(Value *o0, Value *o1) {
  if (o0 && isa<Instruction>(o0))
    return o0;
  return o1;
}

Value* GetFirstInstruction(Value *o0, Value *o1, Value *o2) {
  if (o0 && isa<Instruction>(o0))
    return o0;
  if (o1 && isa<Instruction>(o1))
    return o1;
  return o2;
}

Value* GetFirstInstruction(Value *o0, Value *o1, Value *o2, Value *o3) {
  if (o0 && isa<Instruction>(o0))
    return o0;
  if (o1 && isa<Instruction>(o1))
    return o1;
  if (o2 && isa<Instruction>(o2))
    return o2;
  return o3;
}

Value* GetFirstInstruction(Value *o0, Value *o1, Value *o2, Value *o3, Value *o4) {
  if (o0 && isa<Instruction>(o0))
    return o0;
  if (o1 && isa<Instruction>(o1))
    return o1;
  if (o2 && isa<Instruction>(o2))
    return o2;
  if (o3 && isa<Instruction>(o3))
    return o3;
  return o4;
}

uint32_t GetInstructionSize() {
  return 8;
}

}
