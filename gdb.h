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

public:
	GDB();
	void startInstance();						//Start an instance of gdb.
	void startInstance(std::string filePath);	//Start an instance of gdb.
												//with a file to debug.
	QByteArray getStandardOutput();	//Returns the STDOUT of the GDB instance.
	QByteArray getStandardError();	//Returns the STDERR of the GDB instance.

	int getPID();
	std::string getPIDString();		//Returns PID as a an string.
	QProcess *getQProcess();		//Returns the QProcess gdbInstance.

	void sendCommand(std::string command);		//Sends a command to gdb.
};

#endif // GDBFUNCTIONS_H
