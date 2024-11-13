#include "ArgParser.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace ArgumentParser {

ArgParser::~ArgParser() {
  for (Argument* arg : arguments) {
    delete arg;
  }
}

bool IsValue(const std::string& buff) {
  return buff.size() == 1 || std::isdigit(buff[1]);
}

int32_t ArgParser::FindByLongName(const std::string& long_name_) {
  for (int i = 0; i < arguments.size(); ++i) {
    if (arguments[i]->long_name_ == long_name_) {
      return i;
    }
  }
  return -1;
}

int32_t ArgParser::FindByShortName(char short_name_) {
  for (int i = 0; i < arguments.size(); ++i) {
    if (arguments[i]->short_name_ == short_name_) {
      return i;
    }
  }
  return -1;
}

bool ArgParser::Parse(const std::vector<std::string>& argv) {
  if (arguments.empty()) {
    return true;
  }
  size_t positional_index = 0;
  for (size_t i = 1; i < argv.size(); ++i) {
    if (argv[i][0] != '-') {
      if (positional_index >= arguments.size()) {
        return false;
      }
      if (argv[i][0] != '-') {
        Argument* arg = arguments[positional_index];
        if (arg->is_positional_) {
          switch (arg->type_) {
            case Argument::Type::String: {
              StringArgument* stringArg = static_cast<StringArgument*>(arg);
              stringArg->value.push_back(argv[i]);
              if (stringArg->str_value_ptr != nullptr) {
                *stringArg->str_value_ptr = argv[i];
              }
              break;
            }
            case Argument::Type::Integer: {
              IntArgument* intArg = static_cast<IntArgument*>(arg);
              intArg->value.push_back(std::stoi(argv[i]));
              if (intArg->int_values_ptr != nullptr) {
                intArg->int_values_ptr->push_back(std::stoi(argv[i]));
              }
              break;
            }
            case Argument::Type::Bool:
              break;
          }
          if (!arg->is_multi_value_) {
            ++positional_index;
          }
        } else return false;
      }
    } else {
      std::string argStr = argv[i];
      if (argStr[0] == '-') {
        if (argStr.size() > 1 && argStr[1] == '-') {
          size_t equalPos = argStr.find("=");
          std::string longName = (equalPos != std::string::npos) ? argStr.substr(2, equalPos - 2) : argStr.substr(2);
          std::string value = (equalPos != std::string::npos) ? argStr.substr(equalPos + 1) : argStr.substr(2);
          
          int index = FindByLongName(longName);
          if (index == -1) {
            return false;
          }
          Argument* argument = arguments[index];
          switch (argument->type_) {
            case Argument::Type::String:
              static_cast<StringArgument*>(argument)->value.push_back(value);
              if (static_cast<StringArgument*>(argument)->str_value_ptr != nullptr) {
                *static_cast<StringArgument*>(argument)->str_value_ptr = value;
              }
              break;
            case Argument::Type::Integer:
              static_cast<IntArgument*>(argument)->value.push_back(stoi(value));
              if (static_cast<IntArgument*>(argument)->int_values_ptr != nullptr) {
                static_cast<IntArgument*>(argument)->int_values_ptr->push_back(stoi(value));
              }           
              break;
            case Argument::Type::Bool:
              static_cast<BoolArgument*>(argument)->value = true;
              if (static_cast<BoolArgument*>(argument)->bool_value_ptr != nullptr) {
                *static_cast<BoolArgument*>(argument)->bool_value_ptr = true;
              }
              break;
          }

          if (!argument->is_multi_value_ && i + 1 < argv.size() && argv[i + 1][0] != '-') {
            static_cast<StringArgument*>(argument)->value.push_back(argv[++i]);
          }
        } else {
          size_t len = argStr.size();
          for (size_t j = 1; j < len; ++j) {
            char shortName = argStr[j];
            int index = FindByShortName(shortName);
            if (index == -1) {
              return false;
            }

            Argument* argument = arguments[index];
            switch (argument->type_) {
                case Argument::Type::String:
                  static_cast<StringArgument*>(argument)->value.push_back((j + 1 < len) ? argStr.substr(j + 2) : "");
                  if (static_cast<StringArgument*>(argument)->str_value_ptr != nullptr) {
                    *static_cast<StringArgument*>(argument)->str_value_ptr = (j + 1 < len) ? argStr.substr(j + 2) : "";
                  }
                  break;
                case Argument::Type::Integer:
                  static_cast<IntArgument*>(argument)->value.push_back((j + 1 < len) ? stoi(argStr.substr(j + 2)) : 0);
                  if (static_cast<IntArgument*>(argument)->int_values_ptr != nullptr) {
                    static_cast<IntArgument*>(argument)->int_values_ptr->push_back((j + 1 < len) ? stoi(argStr.substr(j + 2)) : 0);
                  }    
                  break;
                case Argument::Type::Bool:
                  static_cast<BoolArgument*>(argument)->value = true;
                  if (static_cast<BoolArgument*>(argument)->bool_value_ptr != nullptr) {
                    *static_cast<BoolArgument*>(argument)->bool_value_ptr = true;
                  }
                  break;
              }
            if (argument->type_ != Argument::Type::Bool) {
                break;
            }
          }
        }
      }
    }
  }

  for (auto& arg : arguments) {
      size_t minimumCount = arg->min_arguments_count_;
      size_t providedCount = 0;
      
      switch (arg->type_) {
          case Argument::Type::String:
              providedCount = static_cast<StringArgument*>(arg)->value.size();
              break;
          case Argument::Type::Integer:
              providedCount = static_cast<IntArgument*>(arg)->value.size();
              break;
          default:
              continue;
      }
      if (providedCount < minimumCount && !arg->is_default_ && !arg->help_requested_) {
          return false;
      }
  }

  return true;
}

ArgParser& ArgParser::AddStringArgument(std::string long_name_) {
    StringArgument* arg = new StringArgument();
    arg->long_name_ = std::move(long_name_);
    arguments.push_back(arg);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char short_name_) {
  StringArgument* arg = new StringArgument();
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddStringArgument(char short_name_, std::string long_name_) {
  StringArgument* arg = new StringArgument();
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::StoreValue(std::string& value) {
    if (arguments.back()->type_ == Argument::Type::String) {
      StringArgument* stringArg = static_cast<StringArgument*>(arguments.back());
      stringArg->str_value_ptr = &value;
    }
    return *this;
}

std::string ArgParser::GetStringValue(std::string long_name_) {
  int32_t index = FindByLongName(long_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::String) {
    StringArgument* stringArg = static_cast<StringArgument*>(arguments[index]);
    if (!stringArg->value.empty()) return stringArg->value.front();
  }
  return "";
}

std::string ArgParser::GetStringValue(char short_name_) {
  int32_t index = FindByShortName(short_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::String) {
    StringArgument* stringArg = static_cast<StringArgument*>(arguments[index]);
    if (!stringArg->value.empty()) return stringArg->value.front();
  }
  return "";
}

ArgParser& ArgParser::Default(const char* default_value) {
  if (arguments.back()->type_ == Argument::Type::String) {
    StringArgument* stringArg = static_cast<StringArgument*>(arguments.back());
    if (stringArg->value.empty()) {
      stringArg->value.push_back(default_value);
    }
    stringArg->is_default_ = true;
  } else {
    std::cerr << "Parameter isn't correct";
    exit(EXIT_FAILURE);
  }
  return *this;
}
ArgParser& ArgParser::AddIntArgument(std::string long_name_) {
  IntArgument* arg = new IntArgument();
  arg->long_name_ = std::move(long_name_);
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddIntArgument(char short_name_) {
  IntArgument* arg = new IntArgument();
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddIntArgument(char short_name_, std::string long_name_) {
  IntArgument* arg = new IntArgument();
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

int32_t ArgParser::GetIntValue(std::string long_name_) {
  int32_t index = FindByLongName(long_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::Integer) {
    IntArgument* intArg = static_cast<IntArgument*>(arguments[index]);
    if (!intArg->value.empty()) return intArg->value.front();
  }
  return 0;
}

int32_t ArgParser::GetIntValue(char short_name_) {
  int32_t index = FindByShortName(short_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::Integer) {
    IntArgument* intArg = static_cast<IntArgument*>(arguments[index]);
    if (!intArg->value.empty()) return intArg->value.front();
  }
  return 0;
}

int32_t ArgParser::GetIntValue(std::string long_name_, int32_t index) {
  int32_t index_of_argument = FindByLongName(long_name_);
  IntArgument* intArg = static_cast<IntArgument*>(arguments[index_of_argument]);
  return intArg->value[index];
}

int32_t ArgParser::GetIntValue(char short_name_, int32_t index) {
  int32_t index_of_argument = FindByShortName(short_name_);
  IntArgument* intArg = static_cast<IntArgument*>(arguments[index_of_argument]);
  return intArg->value[index];
}

ArgParser& ArgParser::StoreValues(std::vector<int32_t>& values) {
  if (arguments.back()->type_ == Argument::Type::Integer) {
    IntArgument* intArg = static_cast<IntArgument*>(arguments.back());
    intArg->int_values_ptr = &values;
  }
  return *this;
}

ArgParser& ArgParser::MultiValue() {
  arguments.back()->is_multi_value_ = true;
  return *this;
}

ArgParser& ArgParser::MultiValue(size_t min_arguments_count_) {
  arguments.back()->is_multi_value_ = true;
  arguments.back()->min_arguments_count_ = min_arguments_count_;

  return *this;
}

ArgParser& ArgParser::AddFlag(char short_name_, std::string long_name_) {
  BoolArgument* arg = new BoolArgument;
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddFlag(char short_name_) {
  BoolArgument* arg = new BoolArgument;
  arg->short_name_ = short_name_;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddFlag(std::string long_name_) {
  BoolArgument* arg = new BoolArgument;
  arg->long_name_ = std::move(long_name_);
  arguments.push_back(arg);
  return *this;
}

bool ArgParser::GetFlag(char short_name_) {
  int32_t index = FindByShortName(short_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::Bool) {
    BoolArgument* boolArg = static_cast<BoolArgument*>(arguments[index]);
    return boolArg->value;
  }
  return false; 
}

bool ArgParser::GetFlag(std::string long_name_) {
  int32_t index = FindByLongName(long_name_);
  if (index != -1 && arguments[index]->type_ == Argument::Type::Bool) {
    BoolArgument* boolArg = static_cast<BoolArgument*>(arguments[index]);
    return boolArg->value;
  }
  return false; 
}

ArgParser& ArgParser::Default(bool value) {
  if (arguments.back()->type_ == Argument::Type::Bool) {
    BoolArgument* boolArg = static_cast<BoolArgument*>(arguments.back());
    boolArg->value = value;
    boolArg->is_default_ = true;
  } else {
    std::cerr << "Parameter isn't correct";
    exit(EXIT_FAILURE);
  }
  return *this;
}

ArgParser& ArgParser::StoreValue(bool& value) {
  if (arguments.back()->type_ == Argument::Type::Bool) {
    BoolArgument* boolArg = static_cast<BoolArgument*>(arguments.back());
    boolArg->bool_value_ptr = &value;
  }
  return *this;
}

ArgParser& ArgParser::Positional() {
  arguments.back()->is_positional_ = true;
  return *this;
}

ArgParser& ArgParser::AddHelp(char short_name_, std::string long_name_, std::string description) {
  BoolArgument* arg = new BoolArgument();
  arg->short_name_ = short_name_;
  arg->long_name_ = std::move(long_name_);
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

bool ArgParser::Help() {
  for(int i = 0; i < arguments.size(); ++i) {
    if (arguments[i]->help_requested_) {
      return true;
    }
  }
  return false;
}

std::string ArgParser::HelpDescription() {
    std::stringstream help_string;
    help_string << name_ << "\n";
    
    for (const auto &arg : arguments) {
      if (arg->help_requested_) {
        help_string << arg->help_message_ << "\n\n";
        continue;
      }
      help_string << (arg->short_name_ != '\0' ? "-" : "") 
                  << (arg->short_name_ != '\0' ? std::string(1, arg->short_name_) + ", " : "")
                  << (!arg->long_name_.empty() ? "-" : "")
                  << arg->long_name_;
      switch (arg->type_) {
        case Argument::Type::String: help_string << ", <string>"; break;
        case Argument::Type::Integer: help_string << ", <integer>"; break;
        case Argument::Type::Bool: help_string << ", <bool>"; break;
      }
      help_string << ", " << arg->help_message_;


    if (arg->is_multi_value_) {
      help_string << " [repeated";
      if (arg->min_arguments_count_ > 0) {
        help_string << ", min args = " << arg->min_arguments_count_;
      }
      help_string << "]";
    }
    
    if (arg->is_default_) {
      if(arg->type_ == Argument::Type::String){
        if (arg->is_default_) {
          help_string << " [default = " << static_cast<StringArgument*>(arg)->value[0] << "]";
        }
      }else if(arg->type_ == Argument::Type::Integer && arg->is_default_ && !static_cast<IntArgument*>(arg)->value.empty()){
        help_string << " [default = " << static_cast<IntArgument*>(arg)->value[0] << "]";
      }else if(arg->type_ == Argument::Type::Bool){
        if (arg->is_default_) {
          help_string << " [default = " << (static_cast<BoolArgument*>(arg)->value ? "true" : "false") << "]";
        }
      }
    }
    help_string << "\n\n";
  }
  help_string << "\n\n" << (arguments[0]->short_name_ != 0 ? "-" : "") 
            << (arguments[0]->short_name_ != 0 ? std::string(1, arguments[0]->short_name_) + ", " : "")
            << "--help Display this help and exit\n";

  return help_string.str();
}



ArgParser& ArgParser::AddStringArgumentWithDescription(std::string long_name_, std::string description) {
  StringArgument* arg = new StringArgument();
  arg->long_name_ = std::move(long_name_);
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddStringArgumentWithDescription(char short_name_, std::string description) {
  StringArgument* arg = new StringArgument();
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddStringArgumentWithDescription(char short_name_, std::string long_name_, std::string description) {
  StringArgument* arg = new StringArgument();
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddIntArgumentWithDescription(std::string long_name_, std::string description) {
  IntArgument* arg = new IntArgument();
  arg->long_name_ = std::move(long_name_);
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddIntArgumentWithDescription(char short_name_, std::string description) {
  IntArgument* arg = new IntArgument();
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddIntArgumentWithDescription(char short_name_, std::string long_name_, std::string description) {
  IntArgument* arg = new IntArgument();
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddFlagWithDescription(std::string long_name_, std::string description) {
  BoolArgument* arg = new BoolArgument();
  arg->long_name_ = std::move(long_name_);
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddFlagWithDescription(char short_name_, std::string description) {
  BoolArgument* arg = new BoolArgument();
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}

ArgParser& ArgParser::AddFlagWithDescription(char short_name_, std::string long_name_, std::string description) {
  BoolArgument* arg = new BoolArgument();
  arg->long_name_ = std::move(long_name_);
  arg->short_name_ = short_name_;
  arg->help_message_ = std::move(description);
  arg->help_requested_ = true;
  arguments.push_back(arg);
  return *this;
}
} // namespace ArgumentParser
