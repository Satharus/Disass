#ifndef GDB_H
#define GDB_H

#include <QFileInfo>
#include <QProcess>
#include <QObject>
#include <QString>
#include <QFile>

//ELF 64 Magic Bytes 0x7F ELF 0x02
#define ELF_64_MAGIC "7f454c4602"
//ELF 32 Magic Bytes 0x7F ELF 01
#define ELF_32_MAGIC "7f454c4601"

#define ELF_64_STR "x86-64 ELF"
#define ELF_32_STR "x86 ELF"
#define UNKNOWN_STR "Unknown"

class GDB
{
private:
    QProcess *gdbInstance;
    QString arch;

public:
    GDB();
    void startInstance();						//Start an instance of gdb.
    void startInstance(QString filePath);	//Start an instance of gdb,
                                                //with a file to debug.
    void setArch(QString filePath);
    void forceArch(QString arch);
    QString getArch();
    bool isx86();
    bool isx86_64();

    quint64 getPID();
    QString getPIDString();		//Returns PID as a an string.
    QProcess *getQProcess();		//Returns the QProcess gdbInstance.

    void sendCommand(QString command);		//Sends a command to gdb.
    QByteArray getCommandOutput(QString command);	//Sends a command and returns its output.
    QByteArray getCurrentOutput();				//Returns the output.
};

#endif // GDBFUNCTIONS_H
