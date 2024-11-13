#include "Desk.h"
#include "Options.h"
#include <cstdlib>

void DeclarationDesk(Options& options) {
  options.desk = new uint64_t*[options.length];
  for (uint64_t i = 0; i < options.length; ++i) {
    options.desk[i] = new uint64_t[options.width];
     std::memset(options.desk[i], 0, options.width * sizeof(uint64_t));
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    for (uint64_t j = 0; j < options.width; ++j) {
      options.desk[i][j] = 0;
    }
  } 

  for (uint64_t i = 0; i < options.real_cords_count; ++i) {
    options.desk[options.y_cords[i] + options.shift_zero_cord_y][options.x_cords[i] + options.shift_zero_cord_x] = options.height_values[i];
  }
}

void ExtensionUp(Options& options) {
  uint64_t** temp = new uint64_t*[options.length + 1];
  for (uint64_t i = 0; i < options.length + 1; ++i) {
    temp[i] = new uint64_t[options.width];
     std::memset(temp[i], 0, options.width * sizeof(uint64_t));
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    for (uint64_t j = 0; j < options.width; ++j) {
      temp[i + 1][j] = options.desk[i][j];
    }
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    delete[] options.desk[i];
  }
  delete[] options.desk;

  options.desk = temp;
  ++options.length;
}


void ExtensionDown(Options& options) {
  uint64_t** temp = new uint64_t*[options.length + 1];
  for (uint64_t i = 0; i < options.length + 1; ++i) {
    temp[i] = new uint64_t[options.width];
     std::memset(temp[i], 0, options.width * sizeof(uint64_t));
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    for (uint64_t j = 0; j < options.width; ++j) {
      temp[i][j] = options.desk[i][j];
    }
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    delete[] options.desk[i];
  }
  delete[] options.desk;

  options.desk = temp;
  ++options.length;
  
}

void ExtensionRight(Options& options) {
  uint64_t** temp = new uint64_t*[options.length];
  for (uint64_t i = 0; i < options.length; ++i) {
    temp[i] = new uint64_t[options.width + 1];
     std::memset(temp[i], 0, (options.width + 1) * sizeof(uint64_t));
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    for (uint64_t j = 0; j < options.width; ++j) {
      temp[i][j] = options.desk[i][j];
    }
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    delete[] options.desk[i];
  }
  delete[] options.desk;

  options.desk = temp;
  ++options.width;
}

void ExtensionLeft(Options& options) {
  uint64_t** temp = new uint64_t*[options.length];
  for (uint64_t i = 0; i < options.length; ++i) {
    temp[i] = new uint64_t[options.width + 1];
     std::memset(temp[i], 0, (options.width + 1) * sizeof(uint64_t));
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    for (uint64_t j = 0; j < options.width; ++j) {
      temp[i][j + 1] = options.desk[i][j];
    }
  }

  for (uint64_t i = 0; i < options.length; ++i) {
    delete[] options.desk[i];
  }
  delete[] options.desk;

  options.desk = temp;
  ++options.width;
}