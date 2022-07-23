#include "shellfunctions.h"

QString getShellCommandOutput(QString command)
{
    FILE *file;
    QString output;
    char buff[1024];

    file = popen(command.toStdString().c_str(), "r");

    while (fgets(buff, sizeof(buff)-1, file) != nullptr)
        output.append(buff);

    pclose(file);

    return output;
}
