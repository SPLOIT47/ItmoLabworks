#include "lib/Options.h"
#include "lib/Parser.h"
#include "lib/Desk.h"
#include "lib/SaveImage.h"
#include "lib/SandModel.h"

int main(int argc, char* argv[]) {
  Options options;
  ParseArguments(argc, argv, options);
  ParseFile(options);
  options.CorrectValues();
  DeclarationDesk(options);  
  SandModel(options);

  for (int16_t i = 0; i < options.length; ++i) {
    delete[] options.desk[i];
  }
  delete[] options.desk;
}