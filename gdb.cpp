#include "gdb.h"

GDB::GDB() {}

void GDB::startInstance()	//Default case
{
  gdbInstance = new QProcess();
  gdbInstance->start("gdb"); //Start gdb without parameters
}

void GDB::startInstance(std::string filePath)
{
  gdbInstance = new QProcess();
  gdbInstance->start("gdb", QStringList() << QString(filePath.c_str()));
  //Start gdb with a file as a parameter.
}

QByteArray GDB::getStandardOutput()
{
  QByteArray stdOut = gdbInstance->readAllStandardOutput();
  return stdOut;
}

QByteArray GDB::getStandardError()
{
  QByteArray stdErr = gdbInstance->readAllStandardError();
  return stdErr;
}

int GDB::getPID()
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
