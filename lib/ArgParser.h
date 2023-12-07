#pragma once
#include <string>
#include<vector>
#include <variant>

namespace ArgumentParser {

class ArgParser {
 protected:
  struct string_str {
    std::vector<std::string> params;
    std::string parameter;
    std::vector<std::string>* params_ptr = nullptr;
    std::string value;
    std::string* value_ptr = nullptr;
    char mini_param;
    ArgumentParser::ArgParser::string_str& Default(std::string val);
    ArgumentParser::ArgParser::string_str& StoreValue(std::string &value);
    bool stored_eq = false;
  };

  struct int_v{
    std::vector<std::string> params;
    std::string parameter;
    std::vector<std::string>* params_ptr = nullptr;
    int value;
    int *value_ptr;
    char mini_param;
    std::vector<int> values;
    std::vector<int> *values_ptr = nullptr;
    int_v &MultiValue(size_t i);
    int_v &MultiValue();
    ArgumentParser::ArgParser::int_v&  StoreValues(std::vector<int>& vector_1);
    bool stored_eq = false;
    bool act_multivalue = false;
    bool act_default = false;
    int size_of_args = 0;
    ArgumentParser::ArgParser::int_v&  Positional();
    bool act_positional = false;
    ArgumentParser::ArgParser::int_v& StoreValue(std::vector<int>& vector_1);
  };

  struct flag_v{
    std::vector<bool> flag_values;
    std::vector<std::string> name_of_flags;
    std::vector<std::string>* name_of_flags_ptr = nullptr;
    std::string flag;
    std::string* flag_ptr;
    std::string description;
    char flag_mini;
    bool flag_value;
    bool *flag_value_ptr;
    void Default(bool defaul_tvalue);
    ArgumentParser::ArgParser::flag_v& StoreValue(bool &value_storage);
    bool act_multivalue;
    bool act_default;
  };

  struct help{

    void MultiValue(int i);
  };



 public:
  bool Parse(int argc, char** argv);
  ArgParser(std::string name);
  bool Parse(std::vector<std::string> parameters);
  string_str& AddStringArgument(std::string parameters);
  string_str& AddStringArgument(char mini_param, std::string long_param);
  help& AddStringArgument(char mini_parameter,std::string parameter , std::string Help_descript);
  std::string GetStringValue(std::string parameter);
  int_v& AddIntArgument(std::string parameter);
  int_v& AddIntArgument(char mini_parameter, std::string parameter);
  int_v& AddIntArgument(std::string number, std::string nums);
  int GetIntValue(std::string parameter);
  int GetIntValue(std::string parameter, int num_of_param);
  void AddHelp(char i, const char *string, const char *string_1);
  bool Help();
  flag_v& AddFlag(char i, std::string name);
  flag_v& AddFlag(char i, std::string name, std::string description);
  bool GetFlag(std::string string);

 private:
    std::vector<std::variant<string_str, int_v, flag_v>> storage;

};

}