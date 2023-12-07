#include "ArgParser.h"
#include <iostream>
#include <utility>

ArgumentParser::ArgParser::ArgParser(std::string name) {
    
}

bool ArgumentParser::ArgParser::Parse(std::vector<std::string> parameters) {
    bool parse = false;
    for (int i = 0; i < parameters.size(); i++) {
        for (auto & j : storage) {
            if (parameters[i][0] == '-') {
                if (std::holds_alternative<string_str>(j)) {
                    string_str *link = &reinterpret_cast<string_str &>(j);
                    if (link->mini_param == parameters[i][1]) {
                        link->value = parameters[i].substr(parameters[i].find('=') + 1, parameters[i].size());
                        if(link-> value_ptr != nullptr){
                            *link->value_ptr = link->value;

                        }
                    }
                }
            } else if(i == parameters.size() - 1 && parameters[i][0] != '-' && reinterpret_cast<string_str &>(j).value.empty()){
                return false;
            }

            if (parameters[i][0] == '-' && parameters[i][1] == '-') {
                if (std::holds_alternative<string_str>(j)) {
                    string_str *link = &reinterpret_cast<string_str &>(j);
                    link->value = parameters[i].substr(parameters[i].find('=') + 1, parameters[i].size());
                    if(link-> value_ptr != nullptr){
                        *link->value_ptr = link->value;
                    }
                } else if(std::holds_alternative<int_v>(j)){
                    int_v *link_int = &reinterpret_cast<int_v&>(j);
                    link_int->value = std::stoi(parameters[i].substr(parameters[i].find('=') + 1, parameters[i].size()));
                    if(link_int-> values_ptr != nullptr){
                        link_int->values_ptr->push_back(link_int->value);
                        if(link_int->act_multivalue){
                            parse = true;
                        }
                        if(parameters[i].find(':') != std::string::npos){
                            std::string exe_path = parameters[i].substr(parameters[i].find(':') + 1, parameters[i].size());
                            std::cout<<exe_path;
                            int exe_value = system(exe_path.c_str());
                            link_int->values_ptr->push_back(exe_value);
                        }
                        if(link_int->act_multivalue ) {
                            if (link_int->values_ptr->size() <= link_int->size_of_args ) {
                                return false;
                            }
                        }

                    }else{
                        link_int->values.push_back(link_int->value);
                    }
                } else if(std::holds_alternative<flag_v>(j)) {
                    flag_v* link_flag= &reinterpret_cast<flag_v &>(j);
                    if(link_flag->act_multivalue){
                        link_flag->flag_value = link_flag ->flag_value_ptr;
                        return link_flag->flag_value;
                    }
                }
            } else if(std::isdigit(parameters[i][0])) {
                if (std::holds_alternative<int_v>(j)) {
                    int_v *link = &reinterpret_cast<int_v &>(j);
                    if (link->act_multivalue && link->act_positional) {
                        if (link->stored_eq) {
                            link->values_ptr->push_back(std::stoi(parameters[i]));
                        } else {
                            link->values.push_back(std::stoi(parameters[i]));
                        }
                    }
                }
            } else if(i == parameters.size() - 1 && parameters[i][0] != '-' && reinterpret_cast<string_str &>(j).value.empty()){
                return false;
            }
        }
    }
    return true;
}

ArgumentParser::ArgParser::string_str &ArgumentParser::ArgParser::AddStringArgument(std::string parameters) {
    string_str str;
    str.parameter = std::move(parameters);
    storage.emplace_back(str);
    return reinterpret_cast<ArgumentParser::ArgParser::string_str &>(storage[storage.size() - 1]);
}

std::string ArgumentParser::ArgParser::GetStringValue(std::string parameter) {
    for (int i = 0; i < storage.size(); i++) {
        if (std::holds_alternative<string_str>(storage[i])) {
            string_str *link = &reinterpret_cast<ArgumentParser::ArgParser::string_str &>(storage[i]);
            if (link->parameter == parameter) {
                return link->value;
            }
        }
    }
}

ArgumentParser::ArgParser::string_str &ArgumentParser::ArgParser::AddStringArgument(char mini_param, std::string long_param) {
    string_str str;
    str.parameter = std::move(long_param);
    str.mini_param = mini_param;
    storage.emplace_back(str);
    return reinterpret_cast<ArgumentParser::ArgParser::string_str &>(storage[storage.size() - 1]);
}

ArgumentParser::ArgParser::string_str&  ArgumentParser::ArgParser::string_str::Default(std::string val) {
    value = val;
}

ArgumentParser::ArgParser::string_str &ArgumentParser::ArgParser::string_str::StoreValue(std::string &val) {
    stored_eq = true;
    if (stored_eq) {
        value_ptr = &val;
        return *this;
    }
}

ArgumentParser::ArgParser::int_v & ArgumentParser::ArgParser::int_v::StoreValues(std::vector<int>& vector_1) {
    stored_eq = true;
    if(stored_eq){
        values_ptr = &vector_1;
    }
    return *this;
}

ArgumentParser::ArgParser::int_v &ArgumentParser::ArgParser::int_v::MultiValue() {
    act_multivalue = true;
}

ArgumentParser::ArgParser::int_v &ArgumentParser::ArgParser::int_v::MultiValue(size_t i) {
    act_multivalue = true;
    size_of_args = i;
}

ArgumentParser::ArgParser::int_v&  ArgumentParser::ArgParser::int_v::Positional() {
    act_positional = true;
 }

int ArgumentParser::ArgParser::GetIntValue(std::string parameter, int num_of_param) {
    for (int i = 0; i < storage.size(); i++) {
        if (std::holds_alternative<int_v>(storage[i])) {
            int_v *link = &reinterpret_cast<ArgumentParser::ArgParser::int_v &>(storage[i]);
            if (link->parameter == parameter) {
                if(link->values_ptr != nullptr){
                    link->values = *link->values_ptr;
                }
                return link->values[num_of_param];
            }
        }
    }
}

ArgumentParser::ArgParser::int_v &ArgumentParser::ArgParser::AddIntArgument(char mini_parameter, std::string parameter) {
    int_v is_int;
    is_int.parameter = std::move(parameter);
    is_int.mini_param = mini_parameter;
    storage.emplace_back(is_int);
    return reinterpret_cast<ArgumentParser::ArgParser::int_v &>(storage[storage.size() - 1]);;
}

ArgumentParser::ArgParser::int_v &ArgumentParser::ArgParser::AddIntArgument(std::string parameter) {
    int_v is_int;
    is_int.parameter = std::move(parameter);
    storage.emplace_back(is_int);
    return reinterpret_cast<ArgumentParser::ArgParser::int_v &>(storage[storage.size() - 1]);
}

int ArgumentParser::ArgParser::GetIntValue(std::string parameter) {
    for (auto & i : storage) {
        if (std::holds_alternative<int_v>(i)) {
            int_v *link = &reinterpret_cast<ArgumentParser::ArgParser::int_v &>(i);
            if (link->parameter == parameter) {
                return link->value;
            }
        }
    }
}

void ArgumentParser::ArgParser::AddHelp(char i, const char *string, const char *string_1) {

}

bool ArgumentParser::ArgParser::Help() {
    return true;
}

ArgumentParser::ArgParser::flag_v &ArgumentParser::ArgParser::AddFlag(char i, std::string name) {
    flag_v is_flag;
    is_flag.flag = std::move(name);
    is_flag.flag_mini = i;
    storage.emplace_back(is_flag);
    return reinterpret_cast<ArgumentParser::ArgParser::flag_v&>(storage[storage.size() - 1]);
}

ArgumentParser::ArgParser::int_v &ArgumentParser::ArgParser::AddIntArgument(std::string number, std::string nums){

}

bool ArgumentParser::ArgParser::GetFlag(std::string parameter) {
    for(auto i: storage){
        if (std::holds_alternative<flag_v>(i)){
            flag_v *link = &reinterpret_cast<ArgumentParser::ArgParser::flag_v &>(i);
            if(link->flag == parameter){
                return link->flag_value;
            }
        }
    }
}

ArgumentParser::ArgParser::help &ArgumentParser::ArgParser::AddStringArgument(char mini_parameter, std::string parameter, std::string Help_descript) {

}

ArgumentParser::ArgParser::flag_v &ArgumentParser::ArgParser::AddFlag(char i, std::string name, std::string description) {
    flag_v is_flag;
    is_flag.flag = std::move(name);
    is_flag.flag_mini = i;
    is_flag.description = description;
    storage.emplace_back(is_flag);
    return reinterpret_cast<ArgumentParser::ArgParser::flag_v&>(storage[storage.size() - 1]);
}

bool ArgumentParser::ArgParser::Parse(int argc, char **argv) {
    std::vector<std::string> argument;
    for(int i = 0; i < argc; i++){
        argument.emplace_back(argv[i]);
    }
    return Parse(argument);
}

void ArgumentParser::ArgParser::flag_v::Default(bool defaul_value) {
}

ArgumentParser::ArgParser::flag_v& ArgumentParser::ArgParser::flag_v::StoreValue(bool &value_storage) {
    act_multivalue = true;
    if(act_multivalue) {
        value_storage = act_multivalue;
        flag_value_ptr = &value_storage;
    }
    return *this;
}

void ArgumentParser::ArgParser::help::MultiValue(int i) {

}
