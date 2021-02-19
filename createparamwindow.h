#ifndef CREATEPARAMWINDOW_H
#define CREATEPARAMWINDOW_H

#include <QDialog>

namespace Ui {
class CreateParamWindow;
}

class CreateParamWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateParamWindow(QWidget *parent = nullptr);
    ~CreateParamWindow();

signals:
    void SendData(QString PName,QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir);
    void SetActive();

public slots:
    void recieveData(QString NameMenuEntry,QString Insmod,QString SetOptions,QString LinuxDir,QString InitrdDir);


private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *);
    void on_pushButton_2_clicked();

private:
    Ui::CreateParamWindow *ui;
};

#endif // CREATEPARAMWINDOW_H
