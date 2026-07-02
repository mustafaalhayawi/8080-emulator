#include "disassembler.h"
#include <string>

int main() {
  std::string filename = "8080PRE.COM";
  disassemble_file(filename);
}
