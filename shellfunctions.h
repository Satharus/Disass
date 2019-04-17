#ifndef SHELLFUNCTIONS_H
#define SHELLFUNCTIONS_H

#include <string>

//Sends a command to the default system shell and returns the output as is
std::string getShellCommandOutput(std::string command);

#endif // SHELLFUNCTIONS_H
