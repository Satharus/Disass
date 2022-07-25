#include "gdb.h"

GDB::GDB() : QObject ()
{
    gdbProcess = new QProcess();
    /*  This class automatically reads the output from GDB once it is available.
        We don't need to read any output using readAll() or any QProcess read functions.
        All we need to do is use the getCurrentGDBOutput() function to retrieve the output.
    */
    connect(gdbProcess, SIGNAL(readyRead()), this, SLOT(readGDBOutput()));
}

GDB::~GDB()
{
    delete gdbProcess;
}

//Starts gdb with an optional file to be debugged
void GDB::start(QString filePath)
{
    //Start gdb with an optional file as a parameter.
    gdbProcess->start("gdb", QStringList() << filePath, QIODevice::ReadWrite);
    //Set the architecture for the file.
    detectandSetArch(filePath);
}

//Auto detects the architecture of an ELF file using its header
void GDB::detectandSetArch(QString filePath)
{
    QFileInfo executable(filePath);

    if (executable.isSymbolicLink())
    {
        //If link, follow the link recursively and then call setArch to detect
        //the architecture of the original file that the link is pointing to.
        QString actualPath = executable.symLinkTarget();
        detectandSetArch(actualPath);
    }
    else //Attempt to determine the architecture
    {
        QFile _executable(filePath);
        if (_executable.open(QIODevice::ReadOnly))
        {
            //Read the magic bytes of the file
            _executable.seek(0);
            QString magicBytes = _executable.read(5);

            if (magicBytes == ELF_64_MAGIC)
            {
                arch = ELF_64_STR;
            }
            else if (magicBytes == ELF_32_MAGIC)
            {
                arch = ELF_32_STR;
            }
            else arch = UNKNOWN_STR;
        }
        _executable.close();
    }
}

//Set/Get the architecture string of the binary
void GDB::setArch(QString arch)
{
    this->arch = arch;
}
QString GDB::getArch()
{
    return arch;
}

//Return true if the architecture string matches the ELF 32/64 string defined in gdb.h
bool GDB::isx86()
{
    return (arch == ELF_32_STR);
}
bool GDB::isx86_64()
{
    return (arch == ELF_64_STR);
}

quint64 GDB::getPID()
{
    return gdbProcess->processId();
}

QString GDB::getPIDString()
{
    return QString::number(gdbProcess->processId());
}

QProcess *GDB::getQProcess()
{
    return gdbProcess;
}

//Returns the currently stored gdb output
QString GDB::getCurrentGDBOutput()
{
    return QString(currentGDBOutput);
}

//Sends the examine command (x) to gdb to get the nStrackWords values at top of the stack.
void GDB::examineStack(quint64 nStackWords)
{
    //gdb labels a WORD as 4 bytes, let's agree to disagree
    QString stackWordsStr = QString::number(nStackWords, 10);
    if (isx86())
        sendCommand("x/"+stackWordsStr+"wx $esp");
    else
        sendCommand("x/"+stackWordsStr+"wx $rsp");

    lastCommand = GDBCommands::ExamineStack;
}

//Sends the examine command (x) to gdb to get the next nInstructions instuctions
void GDB::examineAssembly(quint64 nInstructions)
{
    QString instructionsStr = QString::number(nInstructions, 10);
    //Examine nInstructions insructions after the current instruction.
    if (isx86())
        sendCommand("x/"+instructionsStr+"i $eip");
    else
        sendCommand("x/"+instructionsStr+"i $rip");

    lastCommand = GDBCommands::ExamineAssembly;
}

//Sends the "info reg" command to gdb to get the register values
void GDB::examineRegisters()
{
    sendCommand("info reg");
    lastCommand = GDBCommands::ExamineRegisters;
}

//Sends the "frame" command to gdb to get the current code line being executed
void GDB::examineCurrentCodeLine()
{
    sendCommand("frame");
    lastCommand = GDBCommands::ExamineCodeLine;
}

//Sends the "list" command to gdb to get the current code line being executed
void GDB::examineCodeLines(QString currentLine)
{
    sendCommand("list " + currentLine);
    lastCommand = GDBCommands::ExamineCode;
}

//Sends the specified command to gdb, and waits for the output
void GDB::sendCommand(QString command)
{
    //Add the \n character to simulate an enter keypress in gdb.
    command += '\n';
    gdbProcess->write(command.toUtf8(), command.toUtf8().size());
    gdbProcess->waitForBytesWritten();
    gdbProcess->waitForReadyRead();
}

void GDB::readGDBOutput()
{
    currentGDBOutput = gdbProcess->readAll();
    qDebug() << currentGDBOutput;
    emit newOutputReady();
}

