#ifndef REMOTECHECKDIALOG_H
#define REMOTECHECKDIALOG_H

#include <QDialog>

namespace Ui {
    class RemoteCheckDialog;
}

class RemoteCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteCheckDialog(QWidget *parent = 0);
    ~RemoteCheckDialog();
    bool check_character_for_number(QString str_num);//false means it includes non-number character,true means all of it is number character.

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private slots:
    void on_lineEditPassword_textChanged(const QString &arg1);

    void on_lineEditUseName_textChanged(const QString &arg1);

    void on_pushButtonSave_pressed();

public:
    Ui::RemoteCheckDialog *ui;
};

#endif // REMOTECHECKDIALOG_H
