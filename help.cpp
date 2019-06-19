#include "help.h"
#include "ui_help.h"
#include <QMovie>

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(1147, 690));
    QMovie *helpGIF = new QMovie(":/Tutorials/Icons/Tutorials/How_to_Use.gif");
    ui->gifLabel->setMovie(helpGIF);
    helpGIF->start();
}

Help::~Help()
{
    delete ui;
}
