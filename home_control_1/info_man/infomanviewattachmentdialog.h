#ifndef INFOMANVIEWATTACHMENTDIALOG_H
#define INFOMANVIEWATTACHMENTDIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
	class InfoManViewAttachmentDialog;
}

class InfoManViewAttachmentDialog : public QDialog
{
    Q_OBJECT

public:
	explicit InfoManViewAttachmentDialog(QWidget *parent = 0);
	~InfoManViewAttachmentDialog();

	int ShowCurrentAttachment();

	QMovie *m_Movie; //播放GIF动画图像的QMovie对象
	float m_MaxScale; //图片最大放大比例
	float m_MinScale; //图片最小缩小比例

	float *m_CurScaleArray; //存放信息的当前图片的缩放比例

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

//private:
	Ui::InfoManViewAttachmentDialog *ui;

private slots:
	void on_pushButtonInfoManViewZoomOut_pressed();
	void on_pushButtonInfoManViewZoomIn_pressed();
	void on_pushButtonInfoManViewNext_pressed();
	void on_pushButtonInfoManViewPrev_pressed();
	void on_pushButtonInfoManViewReturn_pressed();
};

#endif // INFOMAINVIEWATTACHMENTDIALOG_H
