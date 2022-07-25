#ifndef GDB_H
#define GDB_H

#include <QFileInfo>
#include <QProcess>
#include <QObject>
#include <QString>
#include <QFile>
#include <QDebug>

//ELF 64 Magic Bytes
#define ELF_64_MAGIC "\x7f" "ELF" "\x02"
//ELF 32 Magic Bytes
#define ELF_32_MAGIC "\x7f" "ELF" "\x02"

#define ELF_64_STR "x86-64 ELF"
#define ELF_32_STR "x86 ELF"
#define UNKNOWN_STR "Unknown"

enum GDBCommands
{
    Start = 0,
    Stop,
    Continue,
    ExamineStack,
    ExamineRegisters,
    ExamineAssembly,
    ExamineCodeLine,
    ExamineCode,
    StepIntoI,
    StepOverI,
};

class GDB  : public QObject
{
    Q_OBJECT
private:
    QProcess *gdbProcess;
    QString arch;
    QByteArray currentGDBOutput;

signals:
    void newOutputReady();

public slots:
    void readGDBOutput();

public:
    GDB();
    ~GDB();
    void start(QString filePath = "");	//Start an instance of gdb, with an optional file to debug.
    void detectandSetArch(QString filePath);
    void setArch(QString arch);
    QString getArch();
    bool isx86();
    bool isx86_64();

    quint64 getPID();
    QString getPIDString();		//Returns PID as a an string.
    QProcess *getQProcess();		//Returns the QProcess gdbInstance.

    void sendCommand(QString command);		//Sends a command to gdb.
    QString getCurrentGDBOutput();
    quint64 lastCommand;

    void examineStack(quint64 nStackWords);
    void examineAssembly(quint64 nInstructions);
    void examineRegisters();
    void examineCurrentCodeLine();
    void examineCodeLines(QString currentLine);

};

#endif // GDBFUNCTIONS_H
