#ifndef READYPARAMLIST_H
#define READYPARAMLIST_H

#include <QDialog>
#include <createparamwindow.h>
#include <QTreeWidgetItem>

namespace Ui {
class ReadyParamList;
}

class ReadyParamList : public QDialog
{
    Q_OBJECT

    void AddChild(QTreeWidgetItem *parent,QString name,QString submenu);

public:
    explicit ReadyParamList(QWidget *parent = nullptr);
    ~ReadyParamList();

signals:
    void SendData(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir);
    void TransNewData(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int index);
    void SetActive();
    void SendDataForDel(int paramindex,int structindex);
    void LoadOption(int paramindex,int structindex);

public slots:
    void RecData(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir,int index);
    void AddList(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir);
void AddRoot(QString paramname,QString corename,QString insmod,QString setoptions,QString linuxdir,QString initrddir);
    void SetActiveEvent();
private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ReadyParamList *ui;
    CreateParamWindow *ParamWin;
};

#endif // READYPARAMLIST_H
