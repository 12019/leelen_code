#ifndef INFOMANMAINDIALOG_H
#define INFOMANMAINDIALOG_H

#include <QtGui>
#include <QDialog>
#include "infomandetaildialog.h"
#include <stdio.h>
#include "netrecvsocket.h"
#include "infowritedialog.h"

namespace Ui {
	class InfoManMainDialog;
}

class InfoManMainDialog : public QDialog
{
    Q_OBJECT

public:
	explicit InfoManMainDialog(QWidget *parent = 0);
	~InfoManMainDialog();

	int InfoReceive(NetRecvClientSocket *p_sock, char *buf, int len);
	int InfoListAddOne(char *info_file_name);
	int ReloadInfoList();
	void delInfoRecord(int row);
    void ReceiveUdpInfo(char *str_file_name);
	//void setTableViewInfoListColor(QColor color);
    int read_data_from_file(FILE *fp,const char *type,char *type_name);
	QTableView *GetTableViewInfoList();

	InfoManDetailDialog *m_pInfoManDetailDlg; //信息详细内容查看对话框
	InfoWriteDialog *m_pInfoWriteDlg;
	int m_newInfoCnt; //新来的信息的条数

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::InfoManMainDialog *ui;
signals:
    void sigHandlUdpInfoFile(char *info_file_name);
private slots:
    void slotHandlUdpInfoFile(char *info_file_name);
	void on_pushButtonInfoManMainWrite_pressed();
void on_pushButtonInfoManMainDelAll_pressed();
 ////void tvInfoListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
	void on_tableViewInfoList_doubleClicked(QModelIndex index);
	void on_pushButtonInfoManMainDel_pressed();
	void on_pushButtonInfoManMainDetail_pressed();
	void on_pushButtonInfoManMainNext_pressed();
	void on_pushButtonInfoManMainPrev_pressed();
	void on_pushButtonInfoManMainReturn_pressed();
};

#endif // INFOMANMAINDIALOG_H
