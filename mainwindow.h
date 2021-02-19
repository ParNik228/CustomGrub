#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <editparam.h>
#include <readyparamlist.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void AddRoot(QString name,QString insmod,QString setoptions,QString linuxdir,QString initrddir);
    void AddChild(QTreeWidgetItem *parent,QString name,QString submenu);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InsertItem (QTreeWidgetItem *, QString);

signals:
    void SendText(QString text);
    void TransDataToList(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int i);
    void FillDataForTreeThirdWindow(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir);

public slots:
    void SetActiveEvent();
    void UpdateDataOfTree(QString text);
    void RecParamInStruct(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int index);
    void RecieveDataForDel(int paramindex,int structindex);
    void UpdateTreeLoadOpt(int paramindex,int structindex);

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void structUpdate(QTreeWidgetItem *curitem,int index);
    void treeUpdateFuncDown(int index,QString name);
    void treeUpdateFuncUp(int index,QString name);
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    EditParam *sWindow;
    ReadyParamList *thirdWindow;
};
#endif // MAINWINDOW_H
