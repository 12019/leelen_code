#ifndef RINGANDVOLUMEDIALOG_H
#define RINGANDVOLUMEDIALOG_H

#include <QDialog>
#include "ringfilesetdialog.h"

namespace Ui {
    class RingAndVolumeDialog;
}

class RingAndVolumeDialog : public QDialog
{
    Q_OBJECT

public:
	explicit RingAndVolumeDialog(QWidget *parent = 0);
	~RingAndVolumeDialog();

	void loadData();

        RingFileSetDialog *m_pRingFileSetDlg; //铃声文件选择对话框
        RingFileSetDialog *m_pDoorBellRingFileSetDlg; //二次门铃声文件选择对话框

	    void showEvent(QShowEvent *);
	    void hideEvent(QHideEvent *);

	    //private:
	    Ui::RingAndVolumeDialog *ui;

	    unsigned  int m_nRingVolume;
	    unsigned  int m_nTalkVolume;
    public slots:
        void on_pushButtonRingVolumeSave_pressed();

    private slots:
	    void on_horizontalSliderVolume_valueChanged(int value);
	    void on_horizontalSliderRingVolume_valueChanged(int value);
        void on_pushButtonRingVolumeCenter2_pressed();
        void on_pushButtonRingVolumeCenter1_pressed();
        void on_pushButtonRingVolumeDoor_pressed();
        void on_pushButtonRingVolumeDail_pressed();
        void on_pushButtonRingVolumeCallIn_pressed();
        void on_pushButtonRingVolumeReturn_pressed();
};

#endif // RINGANDVOLUMEDIALOG_H
