#pragma once

#include <cstdlib>
#include <string>
#include <vector>

namespace ArgumentParser {

class ArgParser {
public:
  ArgParser(const std::string& name_) : name_(name_) {}

  ~ArgParser();
  
  bool Parse(const std::vector<std::string>& argv);

  ArgParser& AddStringArgument(std::string long_name_);

  ArgParser& AddStringArgument(char short_name_);

  ArgParser& AddStringArgument(char short_name_, std::string argument); 

  std::string GetStringValue(std::string long_name_);

  std::string GetStringValue(char short_name_);

  ArgParser& StoreValue(std::string& value);

  ArgParser& Default(const char* default_value);

  ArgParser& AddIntArgument(std::string argument);

  ArgParser& AddIntArgument(char short_name_, std::string argument);

  ArgParser& AddIntArgument(char short_name_);

  int32_t GetIntValue(std::string long_name_);

  int32_t GetIntValue(char short_name_);

  int32_t GetIntValue(std::string long_name_, int32_t index);

  int32_t GetIntValue(char short_name_, int32_t index);

  ArgParser& StoreValues(std::vector<int32_t>& values);

  ArgParser& MultiValue();

  ArgParser& MultiValue(size_t min_arguments_count_);

  ArgParser& AddFlag(char short_name_, std::string long_name_);

  ArgParser& AddFlag(char short_name_);

  ArgParser& AddFlag(std::string long_name_);

  bool GetFlag(char short_name_);

  bool GetFlag(std::string long_name_);

  ArgParser& Default(bool value);

  ArgParser& StoreValue(bool& value);

  ArgParser& Positional();

  ArgParser& AddHelp(char short_name_, std::string long_name_, std::string description);

  bool Help();

  std::string HelpDescription();

  ArgParser& AddStringArgumentWithDescription(std::string long_name_, std::string description);

  ArgParser& AddStringArgumentWithDescription(char short_name_, std::string description);

  ArgParser& AddStringArgumentWithDescription(char short_name_, std::string long_name_, std::string description);

  ArgParser& AddIntArgumentWithDescription(std::string long_name_, std::string description);

  ArgParser& AddIntArgumentWithDescription(char short_name_, std::string description);

  ArgParser& AddIntArgumentWithDescription(char short_name_, std::string long_name_, std::string description);

  ArgParser& AddFlagWithDescription(std::string long_name_, std::string description);

  ArgParser& AddFlagWithDescription(char short_name_, std::string description);

  ArgParser& AddFlagWithDescription(char short_name_, std::string long_name_, std::string description);

private:
  std::string name_;
  class Argument {
  public:
    enum class Type { String, Integer, Bool };
    std::string long_name_;
    char short_name_;
    Type type_;
    size_t min_arguments_count_ = 1;
    bool is_multi_value_ = false;
    bool is_positional_ = false;
    bool help_requested_ = false;
    std::string help_message_;
    bool is_default_ = false;
    bool is_help_variable_ = false;

    Argument(Type t) : type_(t) {}
  };

  class StringArgument : public Argument {
  public:
    std::vector<std::string> value;
    std::string* str_value_ptr = nullptr;

    StringArgument() : Argument(Type::String) {}
  };

  class IntArgument : public Argument {
  public:
    std::vector<int32_t> value;
    std::vector<int32_t>* int_values_ptr = nullptr;

    IntArgument() : Argument(Type::Integer) {}
  };

  class BoolArgument : public Argument {
  public:
    bool value = false;
    bool* bool_value_ptr = nullptr;

    BoolArgument() : Argument(Type::Bool) {}
  };

  std::vector<Argument*> arguments;

  int32_t FindByLongName(const std::string& long_name_);

  int32_t FindByShortName(char short_name_);
};

}  // namespace ArgumentParser