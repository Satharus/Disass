#ifndef GDB_H
#define GDB_H

#include <QProcess>
#include <QObject>
#include <cstring>

class GDB
{
private:
    QProcess *gdbInstance;
    QString arch;

public:
    GDB();
    void startInstance();						//Start an instance of gdb.
    void startInstance(std::string filePath);	//Start an instance of gdb,
                                                //with a file to debug.
    void setArch(std::string filePath);
    void forceArch(QString arch);
    QString getArch();
    bool isx86();
    bool isx86_64();

    long long getPID();
    std::string getPIDString();		//Returns PID as a an string.
    QProcess *getQProcess();		//Returns the QProcess gdbInstance.

    void sendCommand(std::string command);		//Sends a command to gdb.
    QByteArray getCommandOutput(std::string command);	//Sends a command and returns its output.
    QByteArray getCurrentOutput();				//Returns the output.
};

#endif // GDBFUNCTIONS_H
