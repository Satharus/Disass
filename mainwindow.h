#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QWindow>
#include <QLabel>
#include <QMovie>
#include <QTime>
#include <QDir>

#include "gdb.h"

//The output pages in the main window
enum outputTabs
{
    Assembly = 0, Code, GDBOutput
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    int checkForArguments(QStringList args);
    void showWelcome();
    void showFileOpenedNotification(QString fileName);
    QString configDirPath = QDir::homePath() + "/.config/Disass/";
    ~MainWindow();

private slots:
    void updateOutput();

    void on_sendButton_clicked();

    void on_actionOpen_triggered();

    void on_intelButton_clicked();

    void on_atNtButton_clicked();

    void on_breakButton_clicked();

    void on_stepOverButton_clicked();

    void on_stepIntoButton_clicked();

    void on_nextCodeLineButton_clicked();

    void on_continueButton_clicked();

    void on_runButton_clicked();

    void on_actionQuit_triggered();

    void on_actionContribute_triggered();

    void on_actionLicense_triggered();

    void on_actionAbout_triggered();

    void on_action64_Bit_triggered();

    void on_action32_Bit_triggered();

    void on_actionHow_to_Use_triggered();

    void on_codeOutputTabs_currentChanged(int index);

    void on_stopButton_clicked(bool clearGDB = true);

private:
    Ui::MainWindow *ui;
    GDB gdbInstance;
    quint64 nStackWords = 28; //Default number of stack "words" to query from gdb
    quint64 nInstructions = 20; //Default number of assembly instructions to query from gdb
    quint64 nLines = 10; //Default number of code lines to query from gdb
    QString currentLine;

    //Add a config struct later on maybe?
    QString configFilePath = configDirPath + "lastDirectory";
    QString lastDirectoryPath; //The last directory an executable has been chosen from.

    void setUIInteraction(bool state);
    void retrieveGDBContext();

    //Functions to parse the gdb output
    void parseandSetRegistersOutput(QString registersOutput);
    void parseandSetAssemblyOutput(QString assemblyOutput);
    void parseandSetStackOutput(QString stackOutput);
    void parseandSetCodeOutput(QString codeOutput);
    void parseandSetCodeLine(QString codeFrameOutput);
};

#endif // MAINWINDOW_H
