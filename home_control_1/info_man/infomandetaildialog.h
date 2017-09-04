#ifndef INFOMANDETAILDIALOG_H
#define INFOMANDETAILDIALOG_H

#include <QtGui>
#include <QDialog>
#include <pthread.h>
#include "infomanviewattachmentdialog.h"

namespace Ui {
	class InfoManDetailDialog;
}

class InfoManDetailDialog : public QDialog
{
    Q_OBJECT

public:
	explicit InfoManDetailDialog(QWidget *parent = 0);
	~InfoManDetailDialog();

	int ShowCurrentInfoDetail();

	QListView *GetListViewInfoAttachment();

	void done(int);

	char m_InfoTime[80]; //收到该信息时的日期时间字符串
	char m_SenderIPAddr[20]; //信息发送者IP地址
	//int *m_AttachmentIndexArray;

	QString m_senderNetName;

	InfoManViewAttachmentDialog *m_pViewAttachemntDlg; //附件图片查看对话框

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::InfoManDetailDialog *ui;

private slots:
	void on_pushButtonInfoManDetailReply_pressed();
	void on_listViewInfoAttachment_doubleClicked(QModelIndex index);
	void on_pushButtonInfoManDetailAttach_pressed();
	void on_pushButtonInfoManDetailNext_pressed();
	void on_pushButtonInfoManDetailPrev_pressed();
	void on_pushButtonInfoManDetailReturn_pressed();
};

#endif // INFOMANDETAILDIALOG_H
