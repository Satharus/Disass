#include "shellfunctions.h"

std::string getShellCommandOutput(std::string command)
{
    FILE *file;
    std::string output;
    char buff[1024];

    file = popen(command.c_str(), "r");

    while (fgets(buff, sizeof(buff)-1, file) != nullptr)
        output.append(buff);

    pclose(file);

    return output;
}