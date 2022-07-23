#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "help.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(1115, 635));	//Resizing to be added soon :D
    ui->pidBox->hide();			//Comment if you want to see the pid of GDB.
    ui->fileArchBox->setReadOnly(true);
    ui->fileNameBox->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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
        ui->commandLineArgumentsBox->setEnabled(true);

        ui->stepOverButton->setEnabled(true);
        ui->stepIntoButton->setEnabled(true);
        ui->nextCodeLineButton->setEnabled(true);
        ui->continueButton->setEnabled(true);
        ui->actionOpen->setEnabled(false);
        ui->action32_Bit->setEnabled(true);
        ui->action64_Bit->setEnabled(true);
    }
    else				//GDB isn't running.
    {
        ui->atNtButton->setEnabled(false);
        ui->intelButton->setEnabled(false);


        ui->breakButton->setEnabled(false);
        ui->runButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
        ui->quitButton->setEnabled(false);
        ui->commandLineArgumentsBox->setEnabled(false);

        ui->stepOverButton->setEnabled(false);
        ui->stepIntoButton->setEnabled(false);
        ui->nextCodeLineButton->setEnabled(false);
        ui->continueButton->setEnabled(false);
        ui->actionOpen->setEnabled(true);
        ui->action32_Bit->setEnabled(false);
        ui->action64_Bit->setEnabled(false);
    }
}

int MainWindow::checkForArguments(QStringList args)
{
    if (args.size() > 1)  //If an argument is passed
    {
        QFileInfo file(args[1]);
        if (file.exists())
        { //If the argument passed is an existent file
            gdbInstance.startInstance(args[1]);

            ui->pidBox->setText(gdbInstance.getPIDString());
            setUIInteraction(true); //Enable the UI

            //Set the architecture in the ArchBox
            ui->fileArchBox->setText(gdbInstance.getArch());

            //Get filename and set it in the FileNameBox
            ui->fileNameBox->setText(file.fileName());
            ui->fileNameBox->setToolTip(args[1]);

            QString output = gdbInstance.getCurrentOutput();
            ui->gdbOutputBox->append(output);
            showFileOpenedNotification(file.fileName());
            return 1;   //Opened GDB to a file being debugged in a GDB instance
        }
        //If it isn't a file and isn't a help argument
        else if  (args[1].toStdString() != "--help" && args[1].toStdString() != "-h")
        {
            QMessageBox::information(this, "Invalid Argument", "The specified file doesn't exist, starting to defaults.");
            return 2; //Opened Disass but to a default GDB instance
        }
    }
    return 0;   //Opened Disass to defaults (no instance running)
}

void MainWindow::updateRegistersWindow()
{
    ui->registerBox->clear();
    gdbInstance.sendCommand("info reg");
    QString output = gdbInstance.getCurrentOutput();
    ui->gdbOutputBox->append(output); //Append the output to the GDB output box as is.
    //Filtering the output

    QVector <QString> lines; //Vector to hold lines
    QString line;            //A single output line

    if (gdbInstance.isx86())
    {
        for (int i = 0; i < output.size(); i++)
        {
            //Push the character if it isn't a newline
            if (output[i] != '\n') line.push_back(output[i]);
            else
            {
                //Delete the duplicate values of eip
                if (line.contains("eip")) line.remove(35, 10);

                //Delete the extra spaces in all lines
                if (line.contains("eflag")) line.remove(6,7);
                else line.remove(3,7);

                lines.push_back(line);
                line.clear();
            }
        }
    }
    else
    {
        for (int i = 0; i < output.size(); i++)
        {
            //Push the character if it isn't a newline
            if (output[i] != '\n') line.push_back(output[i]);
            else
            {
                //Delete the duplicate values of rip
                if (line.contains("rip")) line.remove(35, 14);

                //Delete the extra spaces in all lines
                if (line.contains("eflag")) line.remove(6,7);
                else line.remove(3,7);

                lines.push_back(line);
                line.clear();
            }
        }
    }

    for (int i = 0; i < lines.size(); i++)
        ui->registerBox->append(lines[i]);

}

void MainWindow::updateStackWindow()
{
    ui->stackBox->clear();
    QString nStackWords = "28";
    //Examine nStackWords words from the top stack down
    QByteArray output;
    if (gdbInstance.isx86())   //if the app being debugged is 32-bit
        output = gdbInstance.getCommandOutput("x/"+nStackWords+"wx $esp");
    else                //if 64-bit
        output = gdbInstance.getCommandOutput("x/"+nStackWords+"wx $rsp");

    gdbInstance.sendCommand("."); //Send a dummy command to get the full output.
    output += gdbInstance.getCurrentOutput(); //Get the rest of the output.
    ui->gdbOutputBox->append(output); //Append the output to the GDB output box as is.

    QString word; //Single word from the stack
    for (int i = 0; i < output.size(); i++)
    {
        //Append the character if it isn't a newline, space, or tab.
        if (output[i] != '\n' && output[i] != ' ' && output[i] != '\t')  word += output[i];
        else
        {
            //Append it if it isn't a stack address and isn't any other type of output,
            //put a few spaces before it.
            if (word.size() != 0 && word != "(gdb)" &&
                word[word.size()-1] != ':' && word[0] == '0' && word[1] == 'x')

                    ui->stackBox->append("     " + word);
            //Append only if it is a stack address.
            else if (word.size() != 0 && word[word.size()-1] == ':')    ui->stackBox->append(word);

            word.clear();
        }
    }

}

void MainWindow::updateAssemblyOutput()
{
    ui->gdbAsmOutputBox->clear();
    QString nInstructions = "20";
    QString output;
    //Examine nInstructions insructions after the current instruction.
    if (gdbInstance.isx86())   //if the app being debugged is 32-bit
        output = gdbInstance.getCommandOutput("x/"+nInstructions+"i $eip");
    else                //if 64-bit
        output = gdbInstance.getCommandOutput("x/"+nInstructions+"i $rip");

    gdbInstance.sendCommand("."); //Send a dummy command to get the full output.
    output += gdbInstance.getCurrentOutput(); //Get the rest of the output.
    ui->gdbOutputBox->append(output); //Append the output to the GDB output box as is.

    QVector <QString> lines;
    QString line;

    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] != '\n') line.push_back(output[i]);
        else
        {
            lines.push_back(line);
            line.clear();
        }
    }

    for (int i = 0; i < lines.size(); i++)
        ui->gdbAsmOutputBox->append(lines[i]);

}

void MainWindow::updateCodeOutput()
{
    ui->gdbCodeOutputBox->clear();
    //Get the current code line.
    QString output = gdbInstance.getCommandOutput("frame");
    //Splitting the string to get rid of the first part of the output.
    int newLineIndex = output.indexOf('\n', 0);
    output.remove(0, newLineIndex+1);
    //Stripping anything except the code line from the string.
    newLineIndex = output.indexOf('\t', 0);
    output.remove(newLineIndex,output.size()-(newLineIndex));

    QString currentLine = output; //Save the line number

    output = gdbInstance.getCommandOutput("list " + currentLine);
    gdbInstance.sendCommand(".");//Send a dummy command to get the full output.
    output += gdbInstance.getCurrentOutput();
    ui->gdbOutputBox->append(output); //Append the output to the GDB output box as is.

    QVector <QString> lines;
    QString line;

    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] != '\n') line.push_back(output[i]);
        else
        {
            lines.push_back(line);
            line.clear();
        }
    }

    for (int i = 0; i < lines.size(); i++)
    {
        //Print "=>" before the current line being executed.
        if (lines[i].contains(currentLine)) lines[i].insert(0, "=>");
        else lines[i].insert(0, "  "); //Align the output by adding 2 spaces.
        ui->gdbCodeOutputBox->append(lines[i]);
    }
}

void MainWindow::updateOutput()
{
    updateStackWindow();
    updateRegistersWindow();
    updateAssemblyOutput();
    updateCodeOutput();
    ui->gdbOutputBox->append("---------------");
    //To seperate the output of every step.
}

void MainWindow::updateOutput(std::string output)
{
    ui->gdbOutputBox->append(output.c_str());
    updateStackWindow();
    updateRegistersWindow();
    updateAssemblyOutput();
    updateCodeOutput();
    ui->gdbOutputBox->append("---------------");
    //To seperate the output of every step.
}

void MainWindow::showWelcome()
{
    QMessageBox welcome(QMessageBox::Information, "Welcome", "\tWelcome to Disass!\nYou can load an executable from the file menu.");
    welcome.setFont(QFont("Sans Serif",10,0,false));
    welcome.show();
    welcome.exec();
}

void MainWindow::showFileOpenedNotification(QString fileName)
{
    QMessageBox notification(QMessageBox::Information,"Success",
                            "\tFile " + fileName + " loaded successfuly.\n"
                            "You should now:\n"
                            "- Set a breakpoint from the textbox saying Address/Line, and click on the button next to it.\n"
                            "- Run the program using the green button at the bottom right.\n"
                            "- Step through the program using the buttons (Tooltips are available).\n",
                             QMessageBox::Ok);
    notification.setFont(QFont("Sans Serif",10,0,false));
    notification.show();
    notification.exec();
}

void MainWindow::on_sendButton_clicked()
{
    //Sends the typed command to GDB and returns the output.
    QString output  = gdbInstance.getCommandOutput(ui->commandBox->text());
    ui->commandBox->clear();
    ui->gdbOutputBox->append(output);
}

void MainWindow::on_intelButton_clicked()
{
    gdbInstance.sendCommand("set disassembly-flavor intel");
    updateOutput();
}

void MainWindow::on_atNtButton_clicked()
{
    gdbInstance.sendCommand("set disassembly-flavor att");
    updateOutput();
}

void MainWindow::on_breakButton_clicked()
{
    QString address = ui->breakBox->text();
    gdbInstance.sendCommand("break " + address);
    ui->breakBox->clear();
    updateOutput();
}

void MainWindow::on_stepOverButton_clicked()
{
    std::string output;

    if (ui->codeOutputTabs->currentIndex() == 0) // If assembly
    {
        gdbInstance.sendCommand("nexti");
        output = gdbInstance.getCurrentOutput().toStdString();
        updateOutput(output);
    }
    else if (ui->codeOutputTabs->currentIndex() == 1) // If code
    {
        gdbInstance.sendCommand("next");
        output = gdbInstance.getCurrentOutput().toStdString();
        updateOutput(output);
    }
}

void MainWindow::on_stepIntoButton_clicked()
{
    std::string output;

    if (ui->codeOutputTabs->currentIndex() == 0) // If assembly
    {
        gdbInstance.sendCommand("stepi");
        output = gdbInstance.getCurrentOutput().toStdString();
        updateOutput(output);
    }
    else if (ui->codeOutputTabs->currentIndex() == 1) // If code
    {
        gdbInstance.sendCommand("step");
        output = gdbInstance.getCurrentOutput().toStdString();
        updateOutput(output);
    }
}

void MainWindow::on_nextCodeLineButton_clicked()
{
    gdbInstance.sendCommand("until");
    std::string output = gdbInstance.getCurrentOutput().toStdString();
    updateOutput(output);
}

void MainWindow::on_continueButton_clicked()
{
    gdbInstance.sendCommand("continue");
    std::string output = gdbInstance.getCurrentOutput().toStdString();
    updateOutput(output);

}

void MainWindow::on_runButton_clicked()
{
    QString args = ui->commandLineArgumentsBox->text();
    gdbInstance.sendCommand("run " + args);
    ui->commandLineArgumentsBox->clear();
    ui->runButton->setIcon(QPixmap(":/Controls/Icons/Controls/Restart_Button.png"));
    std::string output = gdbInstance.getCurrentOutput().toStdString();
    updateOutput(output);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Binary"), QDir::currentPath());
    if (filePath.size() == 0)
    {
        QMessageBox::information(this, tr("Failed"), "Failed to open file, opening GDB to defaults.");
        gdbInstance.startInstance();
    }
    else
    {
        QFileInfo file(filePath);
        showFileOpenedNotification(file.fileName());
        gdbInstance.startInstance(filePath);
        ui->fileArchBox->setText(gdbInstance.getArch());
        ui->fileNameBox->setText(file.fileName());
        ui->fileNameBox->setToolTip(filePath);
    }
    /*
    Connect GBD output to textbox //Not needed, kept for debugging purposes.
    QObject::connect(gdbInstance.getQProcess(), &QProcess::readyReadStandardOutput, this, &MainWindow::setStandardOutput);
    QObject::connect(gdbInstance.getQProcess(), &QProcess::readyReadStandardError, this, &MainWindow::setStandardError);
    */

    ui->pidBox->setText(gdbInstance.getPIDString());

    setUIInteraction(true); //Enable the UI

    QString output = gdbInstance.getCurrentOutput();
    ui->gdbOutputBox->append(output);

}

void MainWindow::on_quitButton_clicked()
{
    //This function kills GDB and clears the UI.
    gdbInstance.getQProcess()->close();
    gdbInstance.getQProcess()->kill();	//Kill the GDB process
    setUIInteraction(false);	//Disable UI interaction
    ui->pidBox->clear();
    ui->breakBox->clear();
    ui->commandLineArgumentsBox->clear();
    ui->gdbAsmOutputBox->clear();
    ui->gdbCodeOutputBox->clear();
    ui->gdbOutputBox->clear();
    ui->registerBox->clear();
    ui->stackBox->clear();
    ui->fileArchBox->clear();
    ui->fileNameBox->clear();
    ui->fileNameBox->setToolTip("");
    ui->runButton->setIcon(QPixmap(":/Controls/Icons/Controls/Start_Button.png"));
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
    about.setText("\t   Disass v0.07\nDisass is a free and open source frontend to GDB that aims to make it easier "
                  "to use for beginners.\n\nDisass uses GDB and the Qt application framework.");

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

void MainWindow::on_action64_Bit_triggered()
{
    gdbInstance.forceArch("x86-64 Executable");
    ui->fileArchBox->setText(gdbInstance.getArch());
    updateOutput();
}

void MainWindow::on_action32_Bit_triggered()
{
    gdbInstance.forceArch("x86 Executable");
    ui->fileArchBox->setText(gdbInstance.getArch());
    updateOutput();
}

void MainWindow::on_actionHow_to_Use_triggered()
{
    Help help;
    help.setModal(true);
    help.exec();
}

void MainWindow::on_codeOutputTabs_currentChanged(int index)
{
    //If GDB Output is selected, and a program is being debugged.
    if (index == 2 && ui->sendButton->isEnabled())
    {
        ui->stepIntoButton->setEnabled(false);
        ui->stepOverButton->setEnabled(false);
        ui->nextCodeLineButton->setEnabled(false);
    }
    //If a program is being debugged.
    else if (ui->sendButton->isEnabled())
    {
        ui->stepIntoButton->setEnabled(true);
        ui->stepOverButton->setEnabled(true);
        ui->nextCodeLineButton->setEnabled(true);
    }
}
