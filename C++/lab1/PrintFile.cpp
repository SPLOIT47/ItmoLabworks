#include <iostream>
#include <fstream>
#include <cstring>

struct Options {
  int64_t lines_count_for_output = -1;
  int64_t lines_count = 0;
  bool is_tail = false;
  bool is_command_correct = false;
  const char* filename;
  const char* taken_delimiter = "\n";
  char delimiter;
};

void TransformationDelimiter(Options& options) {
  int delimiter_size = 0;

  while (options.taken_delimiter[delimiter_size] != '\0') ++delimiter_size;
  if (delimiter_size == 2 && options.taken_delimiter[0] == '\\'){
    if (options.taken_delimiter[1] == 'n') {
      options.delimiter = '\n';
    } else if (options.taken_delimiter[1] == 't') {
      options.delimiter = '\t';
    } else if (options.taken_delimiter[1] == 'a') {
      options.delimiter = '\a';
    } else if (options.taken_delimiter[1] == 'b') {
      options.delimiter = '\b';
    } else if (options.taken_delimiter[1] == 'f') {
      options.delimiter = '\f';
    } else if (options.taken_delimiter[1] == 'r') {
      options.delimiter = '\r';
    } else if (options.taken_delimiter[1] == 'v') {
      options.delimiter = '\v';
    } else if (options.taken_delimiter[1] == '?') {
      options.delimiter = '\?';
    } else if (options.taken_delimiter[1] == '"') {
      options.delimiter = '\"';
    } else if (options.taken_delimiter[1] == '\'') { 
      options.delimiter = '\'';
    }
  } else {
      options.delimiter = options.taken_delimiter[0];
    }
}

void LinesCount(Options& options) {
    std::ifstream file(options.filename);
    int64_t lines = 1;

    while (!file.eof()) {
        if (file.get() == options.delimiter) {
            ++lines;
        }
    }
    if (options.lines_count_for_output != -1) {
      options.lines_count = lines - options.lines_count_for_output;
    } else {
      options.lines_count = 0;
    }
}
    

void PrintFileIfTail(Options& options){
  std::ifstream file(options.filename);
  int64_t current_line = 0;
  char current_simbol;
  if (file.is_open()) {
    LinesCount(options);
  
  while (!file.eof()) {
    bool flag = false;              //переменная flag отвечает за то, чтобы при lines_count == lines.lines_tail не выводился лишний символ в конце строки(начинался вывод с новой строки)
    file.get(current_simbol);
    if (current_simbol == options.delimiter && current_line < options.lines_count) {
      ++current_line;
      if (current_line == options.lines_count) {
        flag = true;
      }
    }
    if (current_line >= options.lines_count && !file.eof()) {
      if (!flag && current_simbol != options.delimiter) {
        if (current_simbol != '\n'){
          std::cout << current_simbol;
        }
      }
      if (!flag && current_simbol == options.delimiter) {
        if (options.delimiter == '\n') {
          std::cout << current_simbol;
        } else {
          std::cout << current_simbol << '\n';
        }
        ++current_line;
      }
      if (!flag) {
        flag = true;
      }
    }
  }
    file.close();   
  } else {
    std::cout << "File opening error";
  }
}

void PrintFileIfNotTail(Options& options) {
  std::ifstream file(options.filename);
  int64_t lines_count = 0;
  char current_simbol;
  
  if (file.is_open()) {
    while (!file.eof()) {
      file.get(current_simbol);
      if (lines_count == options.lines_count_for_output) {
        break;
      }
      if (current_simbol == options.delimiter) {
        if (options.delimiter == '\n') {
          std::cout << current_simbol;
        } else {
          std::cout << current_simbol << '\n';
        }
        ++lines_count;
      }
      else if (current_simbol != '\n' && !file.eof()) {
        std::cout << current_simbol;
      }
    }
    file.close();
  } else {
    std::cout << "File opening error";
  }
}

void ParseArguments(int argc, char* argv[], Options& options) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      if (strlen(argv[i]) >= 3) {
        options.filename = argv[i];
      }  
    } else {
      if (strncmp(argv[i], "--lines", 7) == 0) {
        if (argv[i][8] == '='){
          options.lines_count_for_output = std::atoi(argv[i] + 8);
        } 
      } else if (strcmp(argv[i], "-l") == 0) {
        if (i < (argc - 1)) {
          if (isdigit(argv[i + 1][0])) {
          options.lines_count_for_output = std::atoi(argv[i + 1]);
          }
        }     
      } else if (strncmp(argv[i], "--tail", 6) == 0 || strcmp(argv[i], "-t") == 0) {
        options.is_tail = true;
      } else if (strncmp(argv[i], "--delimiter=", 12) == 0) {
        options.taken_delimiter = argv[i] + 12;
      } else if (strcmp(argv[i], "-d") == 0) {
        if (i < (argc - 1)) {
          options.taken_delimiter = argv[i + 1];
        }
      } else {
        std::cout << "Command"  << " " << argv[i] << " " << "isn't correct" << '\n';
        options.is_command_correct = true;
      }
    }
  }
  TransformationDelimiter(options);
}

int main(int argc, char* argv[]) {
    Options options;
    ParseArguments(argc, argv, options);
     if (!options.is_command_correct) {
       if (options.is_tail) {
         PrintFileIfTail(options);
       } else {
         PrintFileIfNotTail(options);
       }
     }
}
