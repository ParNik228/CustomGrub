#ifndef EDITPARAM_H
#define EDITPARAM_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class EditParam;
}

class EditParam : public QDialog
{
    Q_OBJECT

public:
    explicit EditParam(QWidget *parent = nullptr);
    ~EditParam();

signals:
    void updata(QString text);

public slots:
    void recieveData(QString text);

private slots:


    void on_pushButton_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::EditParam *ui;
};

#endif // EDITPARAM_H
