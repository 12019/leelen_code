#ifndef OTHERDIALOG_H
#define OTHERDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class OtherDialog;
}

class OtherDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OtherDialog(QWidget *parent = 0);
    ~OtherDialog();

    int loadData();

    QLineEdit *m_pNameLineEdit[6];
    QLineEdit *m_pUrlLineEdit[6];
    QLineEdit *m_pPwdLineEdit[6];
    QLineEdit *m_pUserNameLineEdit[6];

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

    void pushButtonSave_pressed();

    int MessageBox(QString text, QString button_caption);

    Ui::OtherDialog *ui;

public slots:
    void on_pushButtonDoorBellType_pressed();

//private:
};

#endif // OTHERDIALOG_H
