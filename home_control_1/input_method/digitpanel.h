#ifndef DIGITPANEL_H
#define DIGITPANEL_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtCore/QSignalMapper>
#include <QtGui/QToolButton>
#include "PinyinFrame.h"
#include <qtablewidget.h>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
    class Digitpanel;
}

class Digitpanel : public QWidget
{
    Q_OBJECT

public:
    explicit Digitpanel(QWidget *parent = 0);
    ~Digitpanel();
    void setCharCount(int);
    void setEchodMode(QLineEdit::EchoMode);            //设置QLineEdit显示字符
    void mousePressEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void changeTextCaps(bool isCaps);               //按下CAP键后，button上字符的改变
    void showEvent(QShowEvent *);

signals:
    void updateTime();                                  //更新信号
    void sendOrder(QString);                            //键盘指令发送信号
    void closeDigtal();                                 //关闭软键盘信号
public:
    void on_btn_sym1_clicked();
    void on_btn_sym2_clicked();
    void on_btn_sym3_clicked();
    void on_btn_sym4_clicked();
    void on_btnchangeENGLISH_clicked();
    void on_btnCaps_clicked();
    void on_btnchangeCHINESE_clicked();

private slots:
    void doOperation(int);                              //信号映射器调用的所有按键的统一槽函数

    void on_btn_sym1_pressed();
    void on_btn_sym2_pressed();
    void on_btn_sym3_pressed();
    void on_btn_sym4_pressed();
    void on_btnchangeENGLISH_pressed();
    void on_btnCaps_pressed();
    void on_btnchangeCHINESE_pressed();

public slots:
 void on_toolbiaodian_pressed();
protected:
    void changeEvent(QEvent *e);                    //事件更变

private:
    int charCount;                                  //字符统计
    QSignalMapper *signalMapper;                    //信号映射器，对软键盘来说，不必为每个按键创建一个slot
    QList<QToolButton *> allButtons;
    void changeTextShift(bool isShift);             //按下shift键后，按键button上字符的改变
    bool isEn;                                     //英文输入法切换

public:
	Ui::Digitpanel *ui;
	QPinyinFrame *py;                              //拼音输入,作为display()槽函数的信号
        bool isCaps;
    bool m_isDrag;
    QPoint m_dragPosition;
    int m_inputMethodStatus;                        //0:字母状态 1:数字状态     默认为字母状态
    int m_type;						// 当前显示的状态
    void changeMarkjianti1();
    void changeMarkjianti2();
    void returnenglishjianti();
    void returnzhuyinjianti();
};

#endif // DIGITPANEL_H
