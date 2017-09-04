#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "mediaplayerdialog.h"
#include <QTableView>

//#define MEDIA_LIST_FILE_PATH "/mnt/disk/media/record.xml"

namespace Ui {
	class TestDialog;
}

class TestDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TestDialog(QWidget *parent = 0);
	~TestDialog();

	int ReloadMediaList(char *pathName);
	QTableView *getTableViewMediaList();

	MediaPlayerDialog *m_pMediaPlayerDlg; //对讲录像播放对话框

	int m_newVisitorCnt; //新来对讲的数量

	////void setTableViewMediaListColor(QColor color);

	QTimer *m_pTimer;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::TestDialog *ui;

public slots:
	void slotMediaListPressed();

private slots:
	void on_tableViewMediaList_pressed(QModelIndex index);
 void on_pushButtonDelMediaAll_pressed();
 ////void tvMediaListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
	void on_tableViewMediaList_doubleClicked(QModelIndex index);
  void on_pushButtonDelMedia_pressed();
 void on_pushButtonMediaDetail_pressed();
 void on_pushButtonNextMedia_pressed();
 void on_pushButtonPrevMedia_pressed();
 void on_pushButtonReturn_pressed();
};

#endif // MEDIAMANDIALOG_H
