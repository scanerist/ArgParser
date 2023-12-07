#include <functional>
#include <lib/ArgParser.h>
#include <iostream>

struct Options {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char** argv) {
    Options opt;
    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("--param1");
    parser.Parse(argc, argv);
    int val = parser.GetIntValue("--param1");
    std::cout <<val;
    return 0;

}
