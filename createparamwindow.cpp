#include "createparamwindow.h"
#include "ui_createparamwindow.h"

CreateParamWindow::CreateParamWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateParamWindow)
{
    ui->setupUi(this);
}

CreateParamWindow::~CreateParamWindow()
{
    delete ui;
}

void CreateParamWindow::closeEvent(QCloseEvent *){

    emit SetActive();

}

void CreateParamWindow::recieveData(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir){

    ui->lineEdit_2->setText(NameMenuEntry);
    ui->lineEdit_3->setText(Insmod);
    ui->lineEdit_4->setText(SetOptions);
    ui->lineEdit_5->setText(LinuxDir.remove("\t"));
    ui->lineEdit_6->setText(InitrdDir.remove("\t"));

}


void CreateParamWindow::on_pushButton_clicked()
{
    emit SendData(ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text());
    this->ui->lineEdit->clear();
    emit SetActive();
    this->close();
}

void CreateParamWindow::on_pushButton_2_clicked()
{
    emit SetActive();
    this->close();
}
