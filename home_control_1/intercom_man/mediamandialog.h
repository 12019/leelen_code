#ifndef MEDIAMANDIALOG_H
#define MEDIAMANDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "mediaplayerdialog.h"
#include <QTableView>

//#define MEDIA_LIST_FILE_PATH "/mnt/disk/media/record.xml"

namespace Ui {
    class MediaManDialog;
}

class MediaManDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MediaManDialog(QWidget *parent = 0);
    ~MediaManDialog();

	int ReloadMediaList(char *pathName);
	QTableView *getTableViewMediaList();

	MediaPlayerDialog *m_pMediaPlayerDlg; //对讲录像播放对话框

	int m_newVisitorCnt; //新来对讲的数量

	////void setTableViewMediaListColor(QColor color);

	QTimer *m_pTimer;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
    Ui::MediaManDialog *ui;

public slots:
    void slotMediaListPressed();
    void slotSetMediaManRowHeight(int ID, int row);

private slots:
	void on_tableViewMediaList_pressed(QModelIndex index);
 	void on_pushButtonMediaManDelAll_pressed();
 ////void tvMediaListSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
	void on_tableViewMediaList_doubleClicked(QModelIndex index);
	void on_pushButtonMediaManDel_pressed();
	void on_pushButtonMediaManDetail_pressed();
	void on_pushButtonMediaManNext_pressed();
	void on_pushButtonMediaManPrev_pressed();
	void on_pushButtonMediaManReturn_pressed();
};

#endif // MEDIAMANDIALOG_H
