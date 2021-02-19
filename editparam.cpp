#include "editparam.h"
#include "ui_editparam.h"
#include <QTreeWidgetItem>


EditParam::EditParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditParam)
{
    ui->setupUi(this);

}

void EditParam::recieveData(QString text){

    ui->lineEdit->setText(text);

}
EditParam::~EditParam()
{
    delete ui;
}


void EditParam::on_pushButton_clicked(bool checked)
{
    emit updata(ui->lineEdit->text());
    this->close();
}

void EditParam::on_pushButton_2_clicked()
{
    this->close();
}
