#ifndef GSROLLLINEEDIT_H
#define GSROLLLINEEDIT_H

#include <QLineEdit>

class ScrollLineEdit : public QLineEdit
{
	Q_OBJECT
	//Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
	//Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
public:
	ScrollLineEdit(QWidget *parent = 0);

	//void setTextColor(const QColor &newColor);
	//QColor getTextColor() const { return textColor;}
	//void setBorderColor(const QColor &newColor);
	//QColor getBorderColor() const { return borderColor;}
protected:
	void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
private:

	//QColor textColor;
	//QColor borderColor;
	int offset;
	int myTimerId;

	QString m_text;
};

#endif
