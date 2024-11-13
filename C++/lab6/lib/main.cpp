#include "Options.h"
#include "ParseTerminal.h"

int main(int argc, char** argv) {
  HamArc::Options options;
  Parse(argc, argv, options);
  return 0;
}