#ifndef BIG5PINYINPANEL_H
#define BIG5PINYINPANEL_H

#include <QtGui/QWidget>
#include <QtGui/QToolButton>
#include <QtCore/QSignalMapper>
#include <QtGui/QLineEdit>
//#include "Big5frame.h"
class QBig5Frame;
namespace Ui {
    class Big5PinyinPanel;
}

class Big5PinyinPanel : public QWidget
{
    Q_OBJECT

public:
    explicit Big5PinyinPanel(QWidget *parent = 0);
    ~Big5PinyinPanel();
    void setCharCount(int);
    void setEchodMode(QLineEdit::EchoMode);            //设置QLineEdit显示字符
    QString getCurval() {return curVal;}              //获取当前数值
    void setCurval(QString curVal);                   //设置当前数值
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void btn_return_default_state(int ignore_id);
    Ui::Big5PinyinPanel *ui;
    QBig5Frame *py;                                //拼音输入,作为display()槽函数的信号

signals:
    void updateTime();                                  //更新信号
    void sendOrder(QString);                            //键盘指令发送信号
    void closeDigtal();                                 //关闭软键盘信号
public:
    void on_btn_hide_clicked();                         //退出输入法
    void on_btn_delete_clicked();                       //退格键处理
    void on_btn_enter_clicked();
    void on_btn_sym1_clicked();
    void on_btn_sym2_clicked();
    void on_btn_sym3_clicked();
    void on_btn_sym4_clicked();
    void on_btnchangeEnglish_clicked();
    void on_toolButtonPrevPage_clicked();
    void on_toolButtonNextPage_clicked();

    void on_btn_caps_clicked();
    void on_btnchangeChinese_clicked();

private slots:
    void doOperation(int);                              //信号映射器调用的所有按键的统一槽函数
    void display(QString);                              //槽函数:显示字符串

    void on_btn_hide_pressed();                         //退出输入法
    void on_btn_delete_pressed();                       //退格键处理
    void on_btn_enter_pressed();
    void on_btn_sym1_pressed();
    void on_btn_sym2_pressed();
    void on_btn_sym3_pressed();
    void on_btn_sym4_pressed();
    void on_btnchangeEnglish_pressed();
    void on_toolButtonPrevPage_pressed();
    void on_toolButtonNextPage_pressed();

    void on_btn_caps_pressed();
    void on_btnchangeChinese_pressed();
    void on_toolbiaodian_pressed();
public slots:
protected:
    void changeEvent(QEvent *e);                        //事件更变

private:
    QString curVal;                                 //当前数值(或者状态标识符)
    int charCount;                                  //字符统计
    QSignalMapper *signalMapper;                    //信号映射器，对软键盘来说，不必为每个按键创建一个slot
    QList<QToolButton *> allButtons;
    bool isEn;                                     //英文输入法切换




public:
    bool m_isDrag;
    bool m_btnchange;
    QPoint m_dragPosition;
    bool isCaps;
    QBig5Frame *btnchange;
    void changeMark1();
    void changeMark2();
	void returnzhuyinenglish();
	//void returnzhuyin();
    void changeTextShift(bool isShift);             //按下shift键后，按键button上字符的改变
    void changeTextCaps(bool isCaps);               //按下CAP键后，button上字符的改变
    void showzhuyinkeyboard();
    void showEngishkeyboard();
};

#endif // BIG5PINYINPANEL_H
