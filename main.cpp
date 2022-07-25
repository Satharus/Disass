#include <QApplication>
#include <iostream>

#include "shellfunctions.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && (strncmp(argv[1], "--help", 6) == 0 || strncmp(argv[1], "-h", 2) == 0))
    {
        std::cout << "Usage: Disass [FILE]\nStarts a GDB session in a GUI to debug the selected file.\n";
        return 0;
    }


    QApplication a(argc, argv);
    MainWindow w;


    QString gdbFound = getShellCommandOutput("which gdb");
    if (gdbFound.isEmpty())
    {
        QMessageBox::critical(nullptr, "GDB not found", "Please install GDB using your package manager.\n\nExiting.");
        return 1;
    }
    else
    {
        w.show();
    }



    QDir *configDirectory = new QDir(w.configDirPath);
    if (!configDirectory->exists())
    {
        configDirectory->mkpath(w.configDirPath);
    }
    else
    {
        return a.exec();
    }

    if (!w.checkForArguments(QCoreApplication::arguments()))
        w.showWelcome();

    return a.exec();
}
