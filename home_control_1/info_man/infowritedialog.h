#ifndef INFOWRITEDIALOG_H
#define INFOWRITEDIALOG_H

#include <QDialog>
#include "sendinfothread.h"
namespace Ui {
    class InfoWriteDialog;
}

class InfoWriteDialog : public QDialog
{
Q_OBJECT

public:
    	explicit InfoWriteDialog(QWidget *parent = 0);
    	~InfoWriteDialog();

	void ResetState(QString strReceiverName);
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	int sendInfo(char *pIpAddr);
	int m_iModalType;
    SendInfoThread *m_pSendInfoThread; //信息发送线程
	//bool m_bSending;
    void emitSigTcpSendInfo();
    void emitSigUdpSendInfo();
//private:
    	Ui::InfoWriteDialog *ui;
signals:
        void sigTcpSendInfo();
        void sigUdpSendInfo();
private slots:
        void slotTcpSendInfo();
        void slotUdpSendInfo();
	void on_pushButtonInfoWriteAddress_pressed();
	void on_pushButtonInfoWriteSend_pressed();
	void on_pushButtonInfoWriteReturn_pressed();
};

#endif // INFOWRITEDIALOG_H
