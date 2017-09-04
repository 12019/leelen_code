#ifndef RINGFILESETDIALOG_H
#define RINGFILESETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QModelIndex>
#include "doorbellringthread.h"

namespace Ui {
    class RingFileSetDialog;
}

class RingFileSetDialog : public QDialog
{
    Q_OBJECT

public:
	    explicit RingFileSetDialog(QWidget *parent = 0);
	    ~RingFileSetDialog();

	void loadRingFile();
	void loadDoorBellRingFile();
	int selectRingFile(QString strRingFile);
	int selectDoorBellFile(QString strRingFile);

	//QLineEdit *m_pLineEdit; //当前行编辑控件
	QPushButton *m_pPushButton; //当前铃声选择按钮
	//QString m_strRingFile;

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	bool m_pBackFromSave;

	Ui::RingFileSetDialog *ui;

	void Remote_Call_Return();				// 屏保调用
	void Switch_Channel_Sound(bool);
private:
private:

	bool doorbell_flag;					// 本对话框选择的是二次门铃的铃音
private slots:
	void playSelectedWavFile(const QModelIndex &, const QModelIndex &);
	void playDoorBellFile(const QModelIndex &, const QModelIndex &);
	void on_pushButtonRingFileSetCancel_pressed();
	void on_pushButtonRingFileSetSave_pressed();
};

#endif // RINGFILESETDIALOG_H
