#ifndef GDB_H
#define GDB_H
#include "mainwindow.h"

#include <QProcess>
#include <QObject>
#include <cstring>

class GDB
{
private:
    QProcess *gdbInstance;
    std::string outputString;

public:
    GDB();
    void startInstance();						//Start an instance of gdb.
    void startInstance(std::string filePath);	//Start an instance of gdb,
                                                //with a file to debug.
    QByteArray getStandardOutput();	//Returns the STDOUT of the GDB instance.
    QByteArray getStandardError();	//Returns the STDERR of the GDB instance.

    long long getPID();
    std::string getPIDString();		//Returns PID as a an string.
    QProcess *getQProcess();		//Returns the QProcess gdbInstance.

    void sendCommand(std::string command);		//Sends a command to gdb.
    QByteArray getCommandOutput(std::string command);	//Sends a command and returns its output.
    QByteArray getCurrentOutput();				//Returns the output.

    std::string getOutputString();
};

#endif // GDBFUNCTIONS_H
