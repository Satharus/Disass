#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QWindow>
#include <QLabel>
#include <QMovie>
#include <QDir>

#include "gdb.h"

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
    ~MainWindow();

private slots:
    void setUIInteraction(bool state);

    void updateRegistersWindow();
    void updateStackWindow();
    void updateAssemblyOutput();
    void updateCodeOutput();
    void updateOutput();
    void updateOutput(std::string output);


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

    void on_quitButton_clicked();

    void on_actionQuit_triggered();

    void on_actionContribute_triggered();

    void on_actionLicense_triggered();

    void on_actionAbout_triggered();

    void on_action64_Bit_triggered();

    void on_action32_Bit_triggered();

    void on_actionHow_to_Use_triggered();

    void on_codeOutputTabs_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    GDB gdbInstance;
};

#endif // MAINWINDOW_H
