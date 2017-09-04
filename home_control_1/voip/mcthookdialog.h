#ifndef MCTHOOKDIALOG_H
#define MCTHOOKDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QtGui/QLineEdit>
#include <QModelIndex>

namespace Ui {
	class MctHookDialog;
}

class MctHookDialog : public QDialog
{
    Q_OBJECT

public:
	explicit MctHookDialog(QWidget *parent = 0);
	~MctHookDialog();

	void ResetState(QString text, QString strName, int vchannel, bool bInitVolume = true);
	int setDisplayWindow(bool bShow);

	QString m_strName;
	int m_videoChannel;
	QFrame *m_pFrameFullshow;

protected:
	bool eventFilter(QObject *o, QEvent *e);

private:
	Ui::MctHookDialog *ui;

private slots:
	//void on_tableViewVideoChannel_clicked(QModelIndex index);
	void on_pushButtonHelp_clicked();
 void on_pushButtonEmergency_clicked();
 void on_pushButtonVolumeAdd_clicked();
 void on_pushButtonVolumeSub_clicked();
 void on_tableWidgetVideoChannel_itemSelectionChanged();
 void on_pushButtonCallCenter_clicked();
	void on_horizontalSliderVolume_valueChanged(int value);
	void on_pushButtonHangUp_clicked();
	void on_pushButtonReturn_clicked();
};

#endif // MCTHOOKDIALOG_H
