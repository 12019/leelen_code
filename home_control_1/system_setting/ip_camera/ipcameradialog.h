#ifndef IPCAMERADIALOG_H
#define IPCAMERADIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class IpCameraDialog;
}

class IpCameraDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit IpCameraDialog(QWidget *parent = 0);
    ~IpCameraDialog();

    int loadData();

    QLineEdit *m_pNameLineEdit[8];
    QLineEdit *m_pUrlLineEdit[8];
    QLineEdit *m_pPwdLineEdit[8];
    QLineEdit *m_pUserNameLineEdit[8];

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

    int MessageBox(QString text, QString button_caption);

    void pushButtonSave_pressed();

//private:
    Ui::IpCameraDialog *ui;
};

#endif // IPCAMERADIALOG_H
