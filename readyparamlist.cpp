#include "readyparamlist.h"
#include "ui_readyparamlist.h"
#include <QTreeWidgetItem>
#include <QMessageBox>


QString CoreName;
QString InsmodP;
QString SetOptionsP;
QString LinuxDirP;
QString InitrdDirP;
int ind;

ReadyParamList::ReadyParamList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadyParamList)
{
    ParamWin=new CreateParamWindow();
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),ParamWin,SLOT(open()));
    connect(this,SIGNAL(SendData(QString,QString,QString,QString,QString)),ParamWin,SLOT(recieveData(QString,QString,QString,QString,QString)));
    connect(ParamWin,SIGNAL(SendData(QString,QString,QString,QString,QString,QString)),this,SLOT(AddList(QString,QString,QString,QString,QString,QString)));
    connect(ParamWin,SIGNAL(SetActive()),this,SLOT(SetActiveEvent()));
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(0,300);

}

void ReadyParamList::closeEvent(QCloseEvent *){

    emit  SetActive();
    ui->treeWidget->clear();
    this->close();

}

void ReadyParamList::RecData(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int index){

    CoreName = NameMenuEntry;
    InsmodP = Insmod;
    SetOptionsP = SetOptions;
    LinuxDirP = LinuxDir;
    InitrdDirP = InitrdDir;
    ind=index;

}
void ReadyParamList::AddRoot(QString paramname,QString corename,QString insmod,QString setoptions,QString linuxdir,QString initrddir){

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,paramname);
    ui->treeWidget->addTopLevelItem(itm);
    AddChild(itm,"CoreName",corename);
    AddChild(itm,"Insmod",insmod);
    AddChild(itm,"SetOptions",setoptions);
    AddChild(itm,"LinuxDir",linuxdir);
    AddChild(itm,"InitrdDir",initrddir);

}
void ReadyParamList::AddChild(QTreeWidgetItem *parent,QString name,QString submenu){

    QTreeWidgetItem *itm = new QTreeWidgetItem();

    if(name=="CoreName"){
        itm->setText(0,name);
        itm->setText(1,submenu);

    }
    if(name=="Insmod"){
        itm->setText(0,name);
        itm->setText(1,submenu);
    }
    if(name=="SetOptions"){
        itm->setText(0,name);
        itm->setText(1,submenu);
    }
    if(name=="LinuxDir"){
        itm->setText(0,name);
        submenu.remove("\t");
        itm->setText(1,submenu);
    }
    if(name=="InitrdDir"){
        itm->setText(0,name);
        submenu.remove("\t");
        itm->setText(1,submenu);
    }

    parent->addChild(itm);

}

void ReadyParamList::AddList(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir){

    AddRoot(PName,NameMenuEntry,Insmod,SetOptions,LinuxDir,InitrdDir);
    emit TransNewData(PName,NameMenuEntry,Insmod,SetOptions,LinuxDir,InitrdDir,ind);
}

ReadyParamList::~ReadyParamList()
{
    delete ui;
}

void ReadyParamList::on_pushButton_clicked()
{
    /*QString EditParamName;
    QString EditCoreName;
    QString EditInsmodP;
    QString EditSetOptionsP;
    QString EditLinuxDirP;
    QString EditInitrdDirP;*/
    emit SendData(CoreName,InsmodP,SetOptionsP,LinuxDirP,InitrdDirP);
    this->setEnabled(false);

}

void ReadyParamList::on_pushButton_2_clicked()
{
    emit SetActive();
    ui->treeWidget->clear();
    this->close();
}
void ReadyParamList::SetActiveEvent(){

    this->setEnabled(true);

}

void ReadyParamList::on_pushButton_3_clicked()
{

        if(ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem())==-1){



        }else{

            int index=0;
            if(ui->treeWidget->currentItem()->parent()){

            }else{

                index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
                ui->treeWidget->clear();
                emit SendDataForDel(index,ind);
            }
            }

        }






void ReadyParamList::on_pushButton_4_clicked()
{
    if(ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem())==-1){



    }else{

        int index=0;
        if(ui->treeWidget->currentItem()->parent()){

        }else{

            index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
            LoadOption(index,ind);
            //ui->treeWidget->clear();
            //emit SendDataForDel(index,ind);
        }
    }


}
