#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>

std::vector<std::string> GetFuzzyCommands();
void Process(const char *message, const char* image);

#endif