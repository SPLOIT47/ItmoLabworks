#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

struct Options {
    int64_t lines = -1;
    int64_t lines_tail = 0;
    bool tail = false;
    std::string filename;
    std::string taken_delimiter = "\n";
    char delimiter;
};

void ParseArguments(int argc, char* argv[], Options& options) {
  for (int i = 1; i < argc; ++i) {
    if (argv[1][0] != '-') {
      options.filename = argv[1];
    } else {
      options.filename = argv[argc - 1];
      if (strncmp(argv[i], "--lines=", 8) == 0) {
        options.lines = std::atoi(argv[i] + 8);
      } else if (strcmp(argv[i], "-l") == 0) {
        options.lines = std::atoi(argv[i + 1]);
      } else if (strncmp(argv[i], "--tail", 6) == 0|| strcmp(argv[i], "-t") == 0) {
        options.tail = true;
      } else if (strncmp(argv[i], "--delimiter=", 12) == 0) {
        std::string input_command = argv[i];
        options.taken_delimiter = input_command.substr(12);
      } else if (strcmp(argv[i], "-d") == 0) {
        options.taken_delimiter = argv[i + 1];
      }
    }
  }
}

void EscapeSequence(Options& options) {
    if (options.taken_delimiter == "\\n") {
        options.delimiter = '\n';
    } else if (options.taken_delimiter == "\\t") {
        options.delimiter = '\t';
    } else if (options.taken_delimiter == "\\a") {
        options.delimiter = '\a';
    } else if (options.taken_delimiter == "\\b") {
        options.delimiter = '\b';
    } else if (options.taken_delimiter == "\\f") {
        options.delimiter = '\f';
    } else if (options.taken_delimiter == "\\t") {
        options.delimiter = '\t';
    } else if (options.taken_delimiter == "\\r") {
        options.delimiter = '\r';
    } else if (options.taken_delimiter == "\\v") {
        options.delimiter = '\v';
    } else if (options.taken_delimiter == "\\?") {
        options.delimiter = '\?';
    } else if (options.taken_delimiter == "\\'") {
        options.delimiter = '\'';
    } else if (options.taken_delimiter.length() > 1 &&
               options.taken_delimiter[options.taken_delimiter.length() - 1] == '\"') {
        options.delimiter = '\"';
    } else {
        options.delimiter = options.taken_delimiter[0];
    }
}

void TailFunc(Options& options) {
    EscapeSequence(options);
    std::ifstream file(options.filename);
    int64_t lines_count = 1;
    char simb;

    while (!file.eof()) {
        file.get(simb);
        if (simb == options.delimiter) {
            lines_count++;
        }
    }
    options.lines_tail = lines_count - options.lines;
}

void FilePrint(Options& options) {
  EscapeSequence(options);
  std::ifstream file(options.filename);
  int64_t lines_count = 0;
  char simb;
  
  if (!options.tail) {
    while (!file.eof()) {
      file.get(simb);

      if (lines_count == options.lines) {
        break;
      }

      if (simb == options.delimiter) {
        if (options.delimiter == '\n'){
          std::cout << simb;
        } else{
          std::cout << simb << '\n';
        }
        ++lines_count;
      }
      else if (simb != '\n'){
        std::cout << simb;
      }

    }
  } else {
    TailFunc(options);
  
    while (!file.eof()) {
      bool flag = false;
      file.get(simb);
      if (simb == options.delimiter && lines_count < options.lines_tail) {
        ++lines_count;
        if (lines_count == options.lines_tail){
            flag = true;
        }
      }

      if (lines_count >= options.lines_tail) {
        if (!flag && simb != options.delimiter) {
          if (simb != '\n'){
            std::cout << simb;
          }
        }
      
        if (!flag && simb == options.delimiter) {
          std::cout << simb << '\n';
          ++lines_count;
        }
      
        flag = false;
      }
    }   
  }
}

int main(int argc, char* argv[]) {
    Options options;
    ParseArguments(argc, argv, options);
    FilePrint(options);
}
