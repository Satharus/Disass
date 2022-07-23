#include "gdb.h"

GDB::GDB() {}

void GDB::startInstance()	//Default case
{
    gdbInstance = new QProcess();
    gdbInstance->start("gdb"); //Start gdb without parameters
}

void GDB::startInstance(QString filePath)
{
    gdbInstance = new QProcess();
    //Start gdb with a file as a parameter.
    gdbInstance->start("gdb", QStringList() << filePath);
    //Set the architecture for the file.
    setArch(filePath);
}

void GDB::setArch(QString filePath)
{
    QFileInfo executable(filePath);

    if (executable.isSymbolicLink())
    {
        //If link, follow the link recursively and then call setArch to detect
        //the architecture of the original file that the link is pointing to.
        QString actualPath = executable.symLinkTarget();
        setArch(actualPath);
    }
    else //Attempt to determine the architecture
    {
        QFile _executable(filePath);
        if (_executable.open(QIODevice::ReadOnly))
        {
            //Read the magic bytes of the file
            _executable.seek(0);
            QString magicBytes = _executable.read(5).toHex();

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
    }
}

void GDB::forceArch(QString arch)
{
    this->arch = arch;
}

QString GDB::getArch()
{
    return arch;
}

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
    return gdbInstance->processId();
}

QString GDB::getPIDString()
{
    return QString::number(gdbInstance->processId());
}

QProcess *GDB::getQProcess()
{
    return gdbInstance;
}

void GDB::sendCommand(QString command)
{
    //Redirects the command to STDIN of GDB by writing the command to the file /proc/$PID/fd/0
    system(("echo \'" + command + "\'" + "> /proc/" + getPIDString() + "/fd/0").toStdString().c_str());
}

QByteArray GDB::getCommandOutput(QString command)
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
