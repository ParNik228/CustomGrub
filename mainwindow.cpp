#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include "editparam.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include "SimpleOperations.cpp"
#include <QCommonStyle>

struct ReadyParam{
    QString ParamName;
    QString CoreName;
    QString Insmod;
    QString SetOptions;
    QString LinuxDir;
    QString InitrdDir;
};

struct MDS {
    //int strictId;
   QString TextModule;
   QString NameMenuEntry;
   QString Insmod;
   QString SetOptions;
   QString LinuxDir;
   QString InitrdDir;
   ReadyParam ReadyParameterList [100];
   int paramcount;
};

MDS listelem[20];
int NumberOfStruct;
QString OtherStrings;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    CheckDirAndFile();

    //QString Other;
    sWindow = new EditParam();
    thirdWindow = new ReadyParamList();

    QFile file("/boot/grub/grub.cfg");
    if (!file.open(QIODevice::ReadOnly)){

        qWarning("Cannot open file for reading");

    }
    file.copy("../CustomGrub/cpgrub");
    file.close();

    QFile fl("../CustomGrub/cpgrub");
    if (!fl.open(QIODevice::ReadOnly)){

        qWarning("Cannot open file for reading");

    }
    NumberOfStruct = StructCounter(fl);
    QList<QString> paramlist;
    for(int i=0;i<NumberOfStruct;i++){
      //listelem[i].strictId = i;
        listelem[i].TextModule = TextModuleFill(i+1);
        QString str = listelem[i].TextModule;
        OtherStrings = OtherFill(str);
        listelem[i].NameMenuEntry = NameMenuEntryFill(str);

        listelem[i].Insmod = OptionFill(str,"insmod","\tinsmod","\t\tinsmod"," \tinsmod"," \t\tinsmod");
        listelem[i].SetOptions = OptionFill(str,"set","\tset","\t\tset"," \tset"," \t\tset");
        listelem[i].LinuxDir = DirSearch(str,"linux","\tlinux","\t\tlinux"," \tlinux"," \t\tlinux");
        listelem[i].InitrdDir = DirSearch(str,"initrd","\tinitrd","\t\tinitrd"," \tinitrd"," \t\tinitrd");
        listelem[i].paramcount = ParamCounter(listelem[i].NameMenuEntry);
        paramlist=Optionfill(listelem[i].NameMenuEntry);
        int l =0;
        for(int j=0;j<listelem[i].paramcount;j++){

            listelem[i].ReadyParameterList[j].ParamName=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].CoreName=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].Insmod=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].SetOptions=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].LinuxDir=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].InitrdDir=paramlist[l];
            l++;

        }
    } 
    FileRemove();
    ui->setupUi(this);
    QCommonStyle style;
    ui->pushButton->setIcon(style.standardIcon(QStyle::SP_ArrowDown));
    ui->pushButton_2->setIcon(style.standardIcon(QStyle::SP_ArrowUp));
    connect(this->ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),sWindow,SLOT(open()));
    connect(this->ui->pushButton_3,SIGNAL(clicked()),thirdWindow,SLOT(open()));
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(0,300);

    for(int i=0;i<NumberOfStruct;i++){
        AddRoot(listelem[i].NameMenuEntry,listelem[i].Insmod,listelem[i].SetOptions,listelem[i].LinuxDir,listelem[i].InitrdDir);
    }
    connect(this,SIGNAL(TransDataToList(QString,QString,QString,QString,QString,int)),thirdWindow,SLOT(RecData(QString,QString,QString,QString,QString,int)));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem *,int)));
    connect(this,SIGNAL(SendText(QString)),sWindow,SLOT(recieveData(QString)));
    connect(sWindow,SIGNAL(updata(QString)),this,SLOT(UpdateDataOfTree(QString)));
    connect(thirdWindow,SIGNAL(TransNewData(QString,QString,QString,QString,QString,QString,int)),this,SLOT(RecParamInStruct(QString,QString,QString,QString,QString,QString,int)));
    connect(this,SIGNAL(FillDataForTreeThirdWindow(QString,QString,QString,QString,QString,QString)),thirdWindow,SLOT(AddRoot(QString,QString,QString,QString,QString,QString)));
    connect(thirdWindow,SIGNAL(SetActive()),this,SLOT(SetActiveEvent()));
    connect(thirdWindow,SIGNAL(SendDataForDel(int,int)),this,SLOT(RecieveDataForDel(int,int)));
    connect(thirdWindow,SIGNAL(LoadOption(int,int)),this,SLOT(UpdateTreeLoadOpt(int,int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddRoot(QString name,QString insmod,QString setoptions,QString linuxdir,QString initrddir){

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    ui->treeWidget->addTopLevelItem(itm);
    AddChild(itm,"Insmod",insmod);
    AddChild(itm,"SetOptions",setoptions);
    AddChild(itm,"LinuxDir",linuxdir);
    AddChild(itm,"InitrdDir",initrddir);

}
void MainWindow::AddChild(QTreeWidgetItem *parent,QString name,QString submenu){

    QTreeWidgetItem *itm = new QTreeWidgetItem();

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
void MainWindow::structUpdate(QTreeWidgetItem *curitem,int index){


    if(curitem->parent()){

        if(curitem->text(0)=="Insmod"){

            listelem[index].TextModule=UpdateTextModule(listelem[index].TextModule,curitem->text(1),listelem[index].Insmod,1,"insmod");
            listelem[index].Insmod = OptionFill(listelem[index].TextModule,"insmod","\tinsmod","\t\tinsmod"," \tinsmod"," \t\tinsmod");

        }
        if(curitem->text(0)=="SetOptions"){

            listelem[index].TextModule=UpdateTextModule(listelem[index].TextModule,curitem->text(1),listelem[index].SetOptions,1,"set");
            listelem[index].SetOptions = OptionFill(listelem[index].TextModule,"set","\tset","\t\tset"," \tset"," \t\tset");

        }
        if(curitem->text(0)=="LinuxDir"){

            listelem[index].TextModule=UpdateTextModule(listelem[index].TextModule,curitem->text(1),listelem[index].LinuxDir,2,"linux");
            listelem[index].LinuxDir = DirSearch(listelem[index].TextModule,"linux","\tlinux","\t\tlinux"," \tlinux"," \t\tlinux");

        }
        if(curitem->text(0)=="InitrdDir"){

            listelem[index].TextModule=UpdateTextModule(listelem[index].TextModule,curitem->text(1),listelem[index].InitrdDir,2,"initrd");
            listelem[index].InitrdDir = DirSearch(listelem[index].TextModule,"initrd","\tinitrd","\t\tinitrd"," \tinitrd"," \t\tinitrd");

        }

    }else{
        listelem[index].TextModule=UpdateTextModule(listelem[index].TextModule,curitem->text(0),listelem[index].NameMenuEntry,3,"");
        listelem[index].NameMenuEntry = NameMenuEntryFill(listelem[index].TextModule);
    }

}
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

    if(item->parent()){
        emit SendText(item->text(1));
    }else{
        emit SendText(item->text(column));
    }

}
void MainWindow::UpdateTreeLoadOpt(int paramindex,int structindex){

    listelem[structindex].TextModule=UpdateTextModule(listelem[structindex].TextModule,listelem[structindex].ReadyParameterList[paramindex].CoreName,listelem[structindex].NameMenuEntry,3,"");
    listelem[structindex].NameMenuEntry = NameMenuEntryFill(listelem[structindex].TextModule);
    listelem[structindex].TextModule=UpdateTextModule(listelem[structindex].TextModule,listelem[structindex].ReadyParameterList[paramindex].Insmod,listelem[structindex].Insmod,1,"insmod");
    listelem[structindex].Insmod = OptionFill(listelem[structindex].TextModule,"insmod","\tinsmod","\t\tinsmod"," \tinsmod"," \t\tinsmod");
    listelem[structindex].TextModule=UpdateTextModule(listelem[structindex].TextModule,listelem[structindex].ReadyParameterList[paramindex].SetOptions,listelem[structindex].SetOptions,1,"set");
    listelem[structindex].SetOptions = OptionFill(listelem[structindex].TextModule,"set","\tset","\t\tset"," \tset"," \t\tset");
    listelem[structindex].TextModule=UpdateTextModule(listelem[structindex].TextModule,listelem[structindex].ReadyParameterList[paramindex].LinuxDir,listelem[structindex].LinuxDir,2,"linux");
    listelem[structindex].LinuxDir = DirSearch(listelem[structindex].TextModule,"linux","\tlinux","\t\tlinux"," \tlinux"," \t\tlinux");
    listelem[structindex].TextModule=UpdateTextModule(listelem[structindex].TextModule,listelem[structindex].ReadyParameterList[paramindex].InitrdDir,listelem[structindex].InitrdDir,2,"initrd");
    listelem[structindex].InitrdDir = DirSearch(listelem[structindex].TextModule,"initrd","\tinitrd","\t\tinitrd"," \tinitrd"," \t\tinitrd");
    ui->treeWidget->clear();
    for(int i=0;i<NumberOfStruct;i++){
        AddRoot(listelem[i].NameMenuEntry,listelem[i].Insmod,listelem[i].SetOptions,listelem[i].LinuxDir,listelem[i].InitrdDir);
    }

}
void MainWindow::UpdateDataOfTree(QString text){

    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    if(item->parent()){
        item->setText(1,text);
        structUpdate(item,ui->treeWidget->currentIndex().parent().row());

    }else
    {
        item->setText(0,text);
        structUpdate(item,ui->treeWidget->indexOfTopLevelItem(item));
    }


}

void MainWindow::treeUpdateFuncDown(int index,QString name){

    if(index==NumberOfStruct-1){

    }else{

        MDS newlist;
        newlist=listelem[index];
        listelem[index]=listelem[index+1];
        listelem[index+1]=newlist;
        ui->treeWidget->clear();
        for(int i=0;i<NumberOfStruct;i++){
            AddRoot(listelem[i].NameMenuEntry,listelem[i].Insmod,listelem[i].SetOptions,listelem[i].LinuxDir,listelem[i].InitrdDir);
        }
        QList<QTreeWidgetItem *> items = ui->treeWidget->findItems("", Qt::MatchContains);
        for(int i=0;i<items.size();i++){

            if(items[i]->text(0)==name){
                ui->treeWidget->setCurrentItem(items[i]);
                break;
            }
            else{}

        }
    }

}
void MainWindow::treeUpdateFuncUp(int index,QString name){

    if(index==0){

    }else{

        MDS newlist;
        newlist=listelem[index];
        listelem[index]=listelem[index-1];
        listelem[index-1]=newlist;
        ui->treeWidget->clear();
        for(int i=0;i<NumberOfStruct;i++){
            AddRoot(listelem[i].NameMenuEntry,listelem[i].Insmod,listelem[i].SetOptions,listelem[i].LinuxDir,listelem[i].InitrdDir);
        }
        QList<QTreeWidgetItem *> items = ui->treeWidget->findItems("", Qt::MatchContains);
        for(int i=0;i<items.size();i++){

            if(items[i]->text(0)==name){
                ui->treeWidget->setCurrentItem(items[i]);
                break;
            }
            else{}

        }
    }

}

void MainWindow::RecParamInStruct(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int index){

    listelem[index].ReadyParameterList[listelem[index].paramcount].ParamName = PName;
    listelem[index].ReadyParameterList[listelem[index].paramcount].CoreName = NameMenuEntry;
    listelem[index].ReadyParameterList[listelem[index].paramcount].Insmod = Insmod;
    listelem[index].ReadyParameterList[listelem[index].paramcount].SetOptions = SetOptions;
    listelem[index].ReadyParameterList[listelem[index].paramcount].LinuxDir = LinuxDir;
    listelem[index].ReadyParameterList[listelem[index].paramcount].InitrdDir = InitrdDir;
    listelem[index].paramcount++;

}

void MainWindow::on_pushButton_clicked()
{
    int index;
    QString name;
    QTreeWidgetItem *tritem = ui->treeWidget->currentItem();

    if(tritem->parent()){

    }else
    {
        index=ui->treeWidget->indexOfTopLevelItem(tritem);
        name=tritem->text(0);
        treeUpdateFuncDown(index,name);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    int index;
    QString name;
    QTreeWidgetItem *tritem = ui->treeWidget->currentItem();

    if(tritem->parent()){

    }else
    {
        index=ui->treeWidget->indexOfTopLevelItem(tritem);
        name=tritem->text(0);
        treeUpdateFuncUp(index,name);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    this->setEnabled(false);
    if(ui->treeWidget->currentItem()->parent()){

    }else{

        int index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
        if(listelem[index].paramcount>0){

            for(int i=0;i<listelem[index].paramcount;i++){

                emit FillDataForTreeThirdWindow(listelem[index].ReadyParameterList[i].ParamName,
                                           listelem[index].ReadyParameterList[i].CoreName,
                                           listelem[index].ReadyParameterList[i].Insmod,
                                           listelem[index].ReadyParameterList[i].SetOptions,
                                           listelem[index].ReadyParameterList[i].LinuxDir,
                                           listelem[index].ReadyParameterList[i].InitrdDir);

            }

        }
        emit TransDataToList(listelem[index].NameMenuEntry,listelem[index].Insmod,listelem[index].SetOptions,listelem[index].LinuxDir,listelem[index].InitrdDir,index);

    }

}
void MainWindow::SetActiveEvent(){

    this->setEnabled(true);


}
void MainWindow::RecieveDataForDel(int paramindex,int structindex){

    if(paramindex+1<=listelem[structindex].paramcount){

        ReadyParam newlist;
        for(int i=paramindex;i<listelem[structindex].paramcount;i++){
            if(i==listelem[structindex].paramcount-1){

                listelem[structindex].ReadyParameterList[i]={};

            }else{

                newlist=listelem[structindex].ReadyParameterList[i];
                listelem[structindex].ReadyParameterList[i]=listelem[structindex].ReadyParameterList[i+1];
                listelem[structindex].ReadyParameterList[i+1]=newlist;
            }

        }

        listelem[structindex].paramcount--;
        if(listelem[structindex].paramcount>0){

            for(int i=0;i<listelem[structindex].paramcount;i++){

                emit FillDataForTreeThirdWindow(listelem[structindex].ReadyParameterList[i].ParamName,
                                           listelem[structindex].ReadyParameterList[i].CoreName,
                                           listelem[structindex].ReadyParameterList[i].Insmod,
                                           listelem[structindex].ReadyParameterList[i].SetOptions,
                                           listelem[structindex].ReadyParameterList[i].LinuxDir,
                                           listelem[structindex].ReadyParameterList[i].InitrdDir);

            }


    }}

}
void MainWindow::closeEvent(QCloseEvent *){

    for(int i=0;i<NumberOfStruct;i++){
        for(int j=0;j<listelem[i].paramcount;j++){

            SaveToFileOptionList(listelem[i].NameMenuEntry,listelem[i].ReadyParameterList[j].ParamName,
                                 listelem[i].ReadyParameterList[j].CoreName,listelem[i].ReadyParameterList[j].Insmod,
                                 listelem[i].ReadyParameterList[j].SetOptions,listelem[i].ReadyParameterList[j].LinuxDir,
                                 listelem[i].ReadyParameterList[j].InitrdDir);


        }


    }


}

void MainWindow::on_pushButton_4_clicked()
{
    for(int i=0;i<NumberOfStruct;i++){
        SaveGrubParam(OtherStrings,listelem[i].TextModule);

    }

}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox msb;
    msb.setText("Backup is complete!");
    MakeGrubBackup();
    msb.exec();
}

void MainWindow::on_pushButton_6_clicked()
{
    LoadGrubBackup();
    ui->treeWidget->clear();
    QList<QString> paramlist;
    for(int i=0;i<NumberOfStruct;i++){
      //listelem[i].strictId = i;
        listelem[i].TextModule = TextModuleFill(i+1);
        QString str = listelem[i].TextModule;
        OtherStrings = OtherFill(str);
        listelem[i].NameMenuEntry = NameMenuEntryFill(str);

        listelem[i].Insmod = OptionFill(str,"insmod","\tinsmod","\t\tinsmod"," \tinsmod"," \t\tinsmod");
        listelem[i].SetOptions = OptionFill(str,"set","\tset","\t\tset"," \tset"," \t\tset");
        listelem[i].LinuxDir = DirSearch(str,"linux","\tlinux","\t\tlinux"," \tlinux"," \t\tlinux");
        listelem[i].InitrdDir = DirSearch(str,"initrd","\tinitrd","\t\tinitrd"," \tinitrd"," \t\tinitrd");
        listelem[i].paramcount = ParamCounter(listelem[i].NameMenuEntry);
        paramlist=Optionfill(listelem[i].NameMenuEntry);
        int l =0;
        for(int j=0;j<listelem[i].paramcount;j++){

            listelem[i].ReadyParameterList[j].ParamName=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].CoreName=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].Insmod=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].SetOptions=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].LinuxDir=paramlist[l];
            l++;
            listelem[i].ReadyParameterList[j].InitrdDir=paramlist[l];
            l++;

        }
}
    for(int i=0;i<NumberOfStruct;i++){
        AddRoot(listelem[i].NameMenuEntry,listelem[i].Insmod,listelem[i].SetOptions,listelem[i].LinuxDir,listelem[i].InitrdDir);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    QMessageBox msb;
    msb.setText(listelem[0].TextModule);
    msb.exec();
}
