#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string.h>
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

    return a.exec();
}