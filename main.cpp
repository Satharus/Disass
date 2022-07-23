#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc > 1 && (strncmp(argv[1], "--help", 6) == 0 || strncmp(argv[1], "-h", 2) == 0))
    {
        std::cout << "Usage: Disass [FILE]\nStarts a GDB session in a GUI to debug the selected file.\n";
        return 0;
    }

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    if (!w.checkForArguments(QCoreApplication::arguments()))
        w.showWelcome();

    return a.exec();
}
