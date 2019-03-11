#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
	void on_sendButton_clicked();

	void setStandardOutput();
	void setStandardError();

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

	void setUIInteraction(bool state);

	void on_registersButton_clicked();

	void on_actionQuit_triggered();

	void on_actionContribute_triggered();

	void on_actionLicense_triggered();

	void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
