#include "gdb.h"
#include "shellfunctions.h"

GDB::GDB() {}

void GDB::startInstance()	//Default case
{
    gdbInstance = new QProcess();
    gdbInstance->start("gdb"); //Start gdb without parameters
}

void GDB::startInstance(std::string filePath)
{
    gdbInstance = new QProcess();
    //Start gdb with a file as a parameter.
    gdbInstance->start("gdb", QStringList() << QString(filePath.c_str()));
    //Set the architecture for the file.
    setArch(filePath);
}

void GDB::setArch(std::string filePath)
{
    //Get file type
    std::string output = getShellCommandOutput("file \"" + filePath + "\"");

    if      (output.find("64") != std::string::npos)    arch = "x86-64 Executable";
    else if (output.find("32") != std::string::npos)    arch = "x86 Executable";
    else if (output.find("symbolic link") != std::string::npos)
    {
        //If link, follow the link recursively and then call setArch to detect
        //the architecture of the original file that the link is pointing to.
        std::string linkPath = getShellCommandOutput("readlink -en \"" + filePath + "\"");
        setArch(linkPath);
    }
    else arch = "Unknown";
}

QString GDB::getArch()
{
    return arch;
}

bool GDB::isx86()
{
    return (arch == "x86 Executable");
}

bool GDB::isx86_64()
{
    return (arch == "x86-64 Executable");
}

long long GDB::getPID()
{
    return gdbInstance->processId();
}

std::string GDB::getPIDString()
{
    return std::to_string(gdbInstance->processId());
}

QProcess *GDB::getQProcess()
{
    return gdbInstance;
}

void GDB::sendCommand(std::string command)
{
    system(("echo \'" + command + "\'" + "> /proc/" + getPIDString() + "/fd/0").c_str());
    //Redirects the command to STDIN of GDB by writing the command to the file /proc/$PID/fd/0
}

QByteArray GDB::getCommandOutput(std::string command)
{
    sendCommand(command);
    gdbInstance->waitForReadyRead();
    QByteArray output = gdbInstance->readAllStandardOutput();
    return output;
}

QByteArray GDB::getCurrentOutput()
{
    gdbInstance->waitForReadyRead();
    QByteArray output = gdbInstance->readAllStandardOutput();
    return output;
}