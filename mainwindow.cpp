#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gdb.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <cstring>
#include <QDesktopServices>
#include <QString>

GDB gdb1;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setFixedSize(QSize(1115, 630));	//Resizing to be added soon :D
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_sendButton_clicked()
{
	gdb1.sendCommand(ui->commandBox->text().toStdString());
	ui->commandBox->clear();
}

void MainWindow::setStandardOutput()
{
	ui->gdbAsmOutputBox->append(gdb1.getStandardOutput());
	ui->gdbCodeOutputBox->setText(ui->gdbAsmOutputBox->toPlainText());
}

void MainWindow::setStandardError()
{
	ui->gdbAsmOutputBox->append(gdb1.getStandardError());
	ui->gdbCodeOutputBox->setText(ui->gdbAsmOutputBox->toPlainText());
}

void MainWindow::on_actionOpen_triggered()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open Binary"), QDir::homePath());
	if (filePath.size() == 0)
	{
		QMessageBox::information(this, tr("Failed"), "Failed to open file, opening GDB to defaults.");
		gdb1.startInstance();
	}
	else gdb1.startInstance(filePath.toStdString());

	//Connect GBD output to textbox
	QObject::connect(gdb1.getQProcess(), &QProcess::readyReadStandardOutput, this, &MainWindow::setStandardOutput);
	QObject::connect(gdb1.getQProcess(), &QProcess::readyReadStandardError, this, &MainWindow::setStandardError);
	ui->pidBox->setText(gdb1.getPIDString().c_str());

	setUIInteraction(true); //Enable the UI
}

void MainWindow::on_intelButton_clicked()
{
	gdb1.sendCommand("set disassembly-flavor intel");
}

void MainWindow::on_atNtButton_clicked()
{
    gdb1.sendCommand("set disassembly-flavor att");
}


void MainWindow::on_breakButton_clicked()
{
	std::string address = ui->breakBox->text().toStdString();
	gdb1.sendCommand("break " + address);
}

void MainWindow::on_stepOverButton_clicked()
{
	if (ui->codeOutputTabs->currentIndex() == 0) // If assembly
		gdb1.sendCommand("nexti");
	else if (ui->codeOutputTabs->currentIndex() == 1) // If code
		gdb1.sendCommand("next");
}

void MainWindow::on_stepIntoButton_clicked()
{
	if (ui->codeOutputTabs->currentIndex() == 0) // If assembly
		gdb1.sendCommand("stepi");
	else if (ui->codeOutputTabs->currentIndex() == 1) // If code
		gdb1.sendCommand("step");
}

void MainWindow::on_nextCodeLineButton_clicked()
{
	gdb1.sendCommand("until");
}

void MainWindow::on_continueButton_clicked()
{
	gdb1.sendCommand("continue");

}

void MainWindow::on_runButton_clicked()
{
	gdb1.sendCommand("run");
}

void MainWindow::on_quitButton_clicked()
{
	//This function kills GDB and clears the UI.
	gdb1.getQProcess()->close();
	gdb1.getQProcess()->kill();	//Kill the GDB process
	setUIInteraction(false);	//Disable UI interaction
	ui->pidBox->clear();
	ui->gdbAsmOutputBox->clear();
	ui->gdbCodeOutputBox->clear();
}

void MainWindow::setUIInteraction(bool state)
{
	//This function disables/enables UI buttons.
	if (state == true) //GDB is running.
	{
		ui->atNtButton->setEnabled(true);
		ui->intelButton->setEnabled(true);


		ui->breakButton->setEnabled(true);
		ui->runButton->setEnabled(true);
		ui->sendButton->setEnabled(true);
		ui->quitButton->setEnabled(true);

		ui->stepOverButton->setEnabled(true);
		ui->stepIntoButton->setEnabled(true);
		ui->nextCodeLineButton->setEnabled(true);
		ui->continueButton->setEnabled(true);
		ui->actionOpen->setEnabled(false);
		ui->registersButton->setEnabled(true);
	}
	else				//GDB isn't running.
	{
		ui->atNtButton->setEnabled(false);
		ui->intelButton->setEnabled(false);


		ui->breakButton->setEnabled(false);
		ui->runButton->setEnabled(false);
		ui->sendButton->setEnabled(false);
		ui->quitButton->setEnabled(false);

		ui->stepOverButton->setEnabled(false);
		ui->stepIntoButton->setEnabled(false);
		ui->nextCodeLineButton->setEnabled(false);
		ui->continueButton->setEnabled(false);
		ui->actionOpen->setEnabled(true);
		ui->registersButton->setEnabled(false);
	}
}

void MainWindow::on_registersButton_clicked()
{
	gdb1.sendCommand("info reg");
}

void MainWindow::on_actionLicense_triggered()
{
	QMessageBox license;
	license.setWindowTitle("License and Credit");
	license.setText("Disass - Copyright (C) 2019 Ahmed Elmayyah (Satharus)\n\n"
					"This program is licensed under GNU GPLv3.\n\n"
					"This program comes with ABSOLUTELY NO WARRANTY\n"
					"This is free software, and you are welcome to redistribute it "
					"under certain conditions.\nFor more info, check the GNU GPLv3 license.");

	license.setStandardButtons(QMessageBox::Ok);
	license.setDefaultButton(QMessageBox::Ok);
	license.setIconPixmap(QPixmap(":/Artwork/Icons/GPLv3LogoSmall.png"));

	license.show();
	license.exec();
}

void MainWindow::on_actionContribute_triggered()
{
	QMessageBox::information(this, tr("Contibute"), "Contribute on https://github.com/Satharus/Disass");
	QDesktopServices::openUrl(QUrl("https://github.com/Satharus/Disass"));
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox about;
	about.setWindowTitle("About");
	about.setText("Disass is a free and open source frontend to GDB that aims to make it easier "
				  "to use for beginners.\n\nDisass uses GDB and the Qt graphics framework.");

	about.setStandardButtons(QMessageBox::Ok);
	about.setDefaultButton(QMessageBox::Ok);

	about.setIconPixmap(QPixmap(":/Artwork/Icons/BannerSmall.png"));

	about.show();
	about.exec();
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}
