#ifndef BIG5FRAME_H
#define BIG5FRAME_H

#include <QtCore/QtDebug>
#include <QtGui/QFrame>
//#include <QWSServer>
#include <QtGui/QDesktopWidget>
#include "Big5engine.h"

typedef std::basic_string<char> String;                     //或者换用using namespace std,否则String报错

class Big5PinyinPanel;
class Big5InputMethod;
class QBig5Frame :public QFrame
{
    Q_OBJECT
public:
    QBig5Frame(QWidget *parent=0);
    ~QBig5Frame();

    void resetState(){                                  //输入法状态重置
        if(m_bMakingPhrase){
            m_making_phrase_big5pinyin+=m_ime_info.big5pinyin;
        }
		m_ime_info.big5pinyin="";
		m_ime_info.candidates_count=0;                  //候选词统计
        m_enginebig5.clear_key();
    }
    QSize sizeHint() const;                             //保存窗口部件的大小
public:
    bool GetKey(int,int);                               //键值获取
    virtual void show();                                //显示
    virtual void hide();                                //隐藏

    void paintEvent(QPaintEvent *e);                    //英文输入法标识绘制函数
    void SendKey(int , int c=0);                        //键值发送

    void mouseReleaseEvent(QMouseEvent *);              //鼠标释放事件函数
    void keyPressEvent(QKeyEvent *);                    //按键事件处理函数

    Big5PinyinPanel *changekeyboard;

signals:
    void sendchar(QString);                             //字符或字符串传送
public slots:
    bool filter(int);                                   //鼠标或者按键过滤函数

public:
    Big5Engine m_enginebig5;
    bool m_bEnglishMode;                                //中英文输入切换变量
    bool m_bMakingPhrase;                               //词组输入
    QRect m_indicator_rect;                             //英文或中文输入模式标识符大小
    QRect m_about_rect;                                 //汉字候选词显示框(绘制)
    QRect m_leftbtn_rect,m_rightbtn_rect;               //左右选择符号显示框(pixmap绘制)
    bool prev_page();                                       //上一页候选字,用true or false做判断
    bool next_page();                                       //下一页候选字
    bool change;

    typedef struct ime_info_struct{                     //ime即输入法编辑器，包括状态窗口,字母组合窗口,候选字列表窗口
        String      big5pinyin;                         //单个字
        QString     phrase;                             //词组
        unsigned int candidates_count;                  //候选字统计，方便换页
        unsigned int first_visible;                     //从0开始数起，目录中的第一个汉字
        unsigned int candidates_on_page;                //当前的汉字候选字统计标识
        unsigned int candidates_on_prev_page;           //上一页汉字候选字统计标识

        ime_info_struct(){
            candidates_count=0;
        }

        bool prev_page_available(){                 //上一页候选字界面可见
            return candidates_count>0 && first_visible>0;
        }
        bool next_page_available(){
            return candidates_count>0 && (first_visible+candidates_on_page)<candidates_count;
        }

    }ime_info_t;

    ime_info_t   m_ime_info;                      //定义输入法编辑器对象

    String  m_making_phrase_big5pinyin;         //字母组合
    void languageChange(bool);                  //语言切换


private:
    //键盘事件处理虚函数，运行是确定地址
    virtual bool filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat);
    int get_charunicode(unsigned int nIndexOnPage);         //从编码表目录中获取unicode字符
    inline QChar get_char(unsigned int nIndex){
        return m_enginebig5.get_char(nIndex);                   //从Engine类中获取编码目录中的字符
    }
    inline QString get_phrase(unsigned int nIndex){       //从Engine类中通过对象m_engine获取输入法
        return m_enginebig5.get_phrase(nIndex);
    }
    bool commit_selection(int k);                           //软键盘键值选取
    inline void search(){                                 //汉字候选字查找
        m_ime_info.candidates_count=m_enginebig5.search(m_ime_info.big5pinyin.c_str());
        m_ime_info.first_visible=0;

        //控制台信息输出，测试用
        printf("%s,%d matched\n", m_ime_info.big5pinyin.c_str(),m_ime_info.candidates_count);

    }
    int init_gui_dimention();                               //输入法编辑器尺寸初始化
    bool send_hanzi_mark(int ascii_mark);                   //汉字标点符号信号发送，用true or false

};

#endif // BIG5FRAME_H
