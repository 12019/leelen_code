#ifndef PINYINFRAME_H
#define PINYINFRAME_H

#include <QtGui/QFrame>
#include <QtGui/QDesktopWidget>
#include "PinyinEngine.h"
#include "commonpushbutton.h"

class QPinyinFrame : public QFrame
{
        Q_OBJECT
public:
        QPinyinFrame(QWidget *parent= 0);
        ~QPinyinFrame();

        void resetState()
        {
                if(m_bMakingPhrase)
                {
                        m_making_phrase_pinyin+=m_ime_info.pinyin;
                }
                m_ime_info.pinyin="";
                m_ime_info.candidates_count=0;
                m_engine.clear_key();
        }
        QSize sizeHint() const ;

        QRect m_Jian_rect,m_Chinese_rect,m_PY_rect;

public:
        bool GetKey(int,int);
        virtual void show();
        virtual void hide();

        void paintEvent(QPaintEvent* e);
        void SendKey ( int  , int c = 0);

signals:
        void sendchar(QString);
public slots:
        bool filter(int);
        bool prev_page();
        bool next_page();

public:
        PinyinEngine m_engine;
        bool m_bEnglishMode;
        bool m_bMakingPhrase;
        QRect m_about_rect;/*the about text rect*/
        typedef struct ime_info_struc{
                String       pinyin;
                QString      phrase;
                unsigned int candidates_count;
                unsigned int first_visible; //index of first visible candidate hanzi,start from 0
                unsigned int candidates_on_page; //hanzi candidates on current page
                unsigned int candidates_on_prev_page; //hanzi candidates on previous page

                ime_info_struc(){
                        candidates_count = 0;
                }

                bool prev_page_available(){
                        return candidates_count>0&&first_visible>0;
                }

                bool next_page_available(){
                        return candidates_count>0&&(first_visible+candidates_on_page)<candidates_count;
                }
        }ime_info_t;

        ime_info_t m_ime_info;

        String     m_making_phrase_pinyin;
        void languageChange(bool);

        CommonPushButton *PY_PushButton_NextPage;
        CommonPushButton *PY_PushButton_PrePage;
private:
        virtual bool filter ( int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat );
        int get_charunicode(unsigned int nIndexOnPage);
        inline QChar get_char(unsigned int nIndex/*this is global index*/){
                return m_engine.get_char(nIndex);
        }

        inline QString get_phrase(unsigned int nIndex/*this is global index*/){
                return m_engine.get_phrase(nIndex);
        }

        bool commit_selection(int k);
        inline void search(){
                m_ime_info.candidates_count=m_engine.search(m_ime_info.pinyin.c_str());
                m_ime_info.first_visible=0;
//for test
                printf("%s,%d matched\n",m_ime_info.pinyin.c_str(),m_ime_info.candidates_count);

        }
        void init_gui_dimention();
        bool send_hanzi_mark(int ascii_mark);
};

#endif // PINYINFRAME_H
