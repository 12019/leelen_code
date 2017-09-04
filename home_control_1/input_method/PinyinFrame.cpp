#include "PinyinFrame.h"
#include <QtGui/QPainter>
#include <QtCore/QList>
#include <QtGui/QBitmap>
#include <QtGui/QFontMetrics>
#include <QtGui/QLayout>
#include <QtGui/QPalette>
#include <QtGui/QToolButton>
#include <QtGui/QMessageBox>
#include <QtGui/QColor>
#include <QtGui/QPen>
//#include <QWSServer>     //跨进程服务
#include <QtGui/QApplication>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>       //鼠标事件
#include <QtGui/QDesktopWidget>

#include "public.h"
#include "Config.h"
//#include "inputmethod.h"
#include "digitpanel.h"
#include "inputmethod.h"
#include "main/maindialog.h"
#include "ui_digitpanel.h"

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

extern QApplication* App;
extern int g_inputMethodMode;

const char table_file[]="/mnt/disk/input_method/pinyin_table.txt";
const char phrase_index_file[]=" ";
const char config_file[]="/mnt/disk/input_method/murphytalk.conf";

/* XPM */
static const char * pix_vertical[] = {
    "2 32 2 1",
    " 	c #F7F3EDF7F3ED",
    "#	c #BAA68EBAA68E",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
    "# ",
};

const unsigned short ZHONG = 0x7b80;   //Hanzi : Chinese
const unsigned short YING  = 0x82f1;   //Hanzi : English
const char ABOUT[]="About";
const char MAKEPHRASE_OPEN [] = "\xe3\x80\x90\xe9\x80\xa0\xe8\xaf\x8d\x20"; //开始查找unicode码
const char MAKEPHRASE_CLOSE[] = "\xe3\x80\x91";  //unicode编码

//the following marks encoded in ucs-2(unicode 16 big endian)  标点符号unicode编码表
const int COMMA_MARK	=0xff0c;
const int PERIOD_MARK	=0x3002;
const int BOOK_MARK1	=0x300a;
const int BOOK_MARK2	=0x300b;
const int QUESTION_MARK	=0xff1f;
const int COLON_MARK	=0xff1a;

QPinyinFrame::QPinyinFrame(QWidget *parent) :
    QFrame(parent), m_engine(table_file,phrase_index_file),
    m_bEnglishMode(false),m_bMakingPhrase(false)    //相对自绘制的键盘代码，减少一些不必要的函数
{
    (new QHBoxLayout(this))->setEnabled(TRUE);

    //创建换页按键
    PY_PushButton_NextPage= new CommonPushButton(this);
    PY_PushButton_PrePage = new CommonPushButton(this);

    if 	( g_pScaleSrc->get_screen_version() == 1 )
    {
	    PY_PushButton_NextPage->setGeometry(721,40,79,50); //按钮的位置及大小
	    PY_PushButton_PrePage->setGeometry(640,40,79,50);
    }
    else if ( g_pScaleSrc->get_screen_version() == 2 )
    {
	    PY_PushButton_NextPage->setGeometry(941,40,79,60); //按钮的位置及大小
	    PY_PushButton_PrePage->setGeometry(860-27,40,79,60);
    }
    else if ( g_pScaleSrc->get_screen_version() == 3 )
    {
	    PY_PushButton_NextPage->setGeometry(1190,70,80,70); //按钮的位置及大小
	    PY_PushButton_PrePage->setGeometry(1070-13,70,80,70);
    }

    PY_PushButton_PrePage->setStyleSheet("QPushButton{background-image: url(:/images/keyboard/left-1.png);}"
                                         "QPushButton:pressed{background-image: url(:/images/keyboard/left-2.png);}"
                                         "QPushButton:disabled{background-image: url(:/images/keyboard/left-3.png);}");
    PY_PushButton_NextPage->setStyleSheet("QPushButton{background-image: url(:/images/keyboard/right-1.png);}"
                                         "QPushButton:pressed{background-image: url(:/images/keyboard/right-2.png);}"
                                         "QPushButton:disabled{background-image: url(:/images/keyboard/right-3.png);}");
    connect(PY_PushButton_PrePage, SIGNAL(clicked()), this, SLOT(prev_page()));
    connect(PY_PushButton_NextPage, SIGNAL(clicked()), this, SLOT(next_page()));

    PY_PushButton_NextPage->setFocusPolicy(Qt::NoFocus);
    PY_PushButton_PrePage->setFocusPolicy(Qt::NoFocus);

#ifdef ARM11
    setFont(QFont("efont",80));
#endif

#ifdef CORTEXA7
    if ( g_pScaleSrc->get_screen_version() == 2 )
	    setFont(QFont("efont",10));
    else if ( g_pScaleSrc->get_screen_version() == 3 )
	    setFont(QFont("efont",12));
#endif

    m_PY_rect=QRect(0,0,-1,-1);
}


bool QPinyinFrame::filter(int id)         //鼠标或者按键事件触发
{
	printf("filter mouse button happen %d\n",id);

	int unicode = id;

	if((Qt::Key)id == Qt::Key_Backspace)        //退格键标识符8,对应Qt::Key
		unicode = 8;
	else if((Qt::Key)id == Qt::Key_Enter)
		unicode = 7;
	else if((Qt::Key)id == Qt::Key_Tab)        //TAB键标识符9
		unicode = 9;
	else if((Qt::Key)id == Qt::Key_Return)
		unicode = 13;
	else if((Qt::Key)id == Qt::Key_Shift || (Qt::Key)id == Qt::Key_CapsLock)  //shift键和Caps Lock键都可以切换大小写
		unicode = 0xffff;
	return GetKey(unicode,id);   //unicode编码与软键盘按键标识对应,返回true or false
}

void QPinyinFrame::languageChange(bool isEn)     //英文切换函数
{
    m_bEnglishMode = isEn;
    update();
}

QPinyinFrame::~QPinyinFrame()
{
        printf("ime killed\n");
}

QSize QPinyinFrame::sizeHint() const
{
        QPinyinFrame *This=const_cast<QPinyinFrame*>(this);
        This->init_gui_dimention();

        return QSize(m_PY_rect.width(), m_PY_rect.height());                       //返回窗体的宽度
}

/*
 *
 */
void QPinyinFrame::init_gui_dimention()           //初始化GUI的尺寸,ui设计的软键盘
{
    if(m_PY_rect.height()>0)
    {
        return;                 //初始化时并没有调用
    }

    m_PY_rect=QRect(0,0,((Digitpanel *)parent())->ui->DigitPYFrame->size().width(),
                    ((Digitpanel *)parent())->ui->DigitPYFrame->size().height());
    qDebug("m_PY_rect %d,%d,%d,%d",m_PY_rect.left(),m_PY_rect.top(),m_PY_rect.width(),m_PY_rect.height());

    m_Chinese_rect=QRect(0,40,((Digitpanel *)parent())->ui->DigitChineseFrame->size().width(),
                         ((Digitpanel *)parent())->ui->DigitChineseFrame->size().height());

    qDebug("m_Chinese_rect %d,%d,%d,%d",m_Chinese_rect.left(),m_Chinese_rect.top(),m_Chinese_rect.width(),m_Chinese_rect.height());

    m_Jian_rect=QRect(0,0,((Digitpanel *)parent())->ui->DigitJianFrame->size().width(),
                      ((Digitpanel *)parent())->ui->DigitJianFrame->size().height());
    qDebug("m_Jian_rect %d,%d,%d,%d",m_Jian_rect.left(),m_Jian_rect.top(),m_Jian_rect.width(),m_Jian_rect.height());
}

bool QPinyinFrame::filter (int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)   //事件触发器
{
    //qDebug("filter!");
    Q_UNUSED(modifiers);
    Q_UNUSED(autoRepeat);
    if (!isHidden()&& isPress) {
        if(m_bEnglishMode){
            if(unicode == 9 && keycode == Qt::Key_Tab){   //TAB键切换中/英文输入
                m_bEnglishMode = false;
                update();
            }
            else if(Qt::Key_F1<=keycode && keycode<=Qt::Key_F12)
            {
                return false;
            }
            else{
                SendKey(unicode,keycode);
            }
            return true;
        }

        if (GetKey (unicode, keycode))  //获取键值
            return true;
        else
            return false;
    }
    else
        return false;
}

void QPinyinFrame::paintEvent(QPaintEvent* e)
{
        QFrame::paintEvent(e);
        QPainter painter(this);
        painter.setClipRect (e->rect());

        QFontMetrics metric=painter.fontMetrics();
        QRect rect = metric.boundingRect(QChar(ZHONG));

        //画按键两侧分割线
        int pixmap_height = m_Jian_rect.height() + (m_Chinese_rect.height() - 32)/2;
        int pixmap_left = 0;
        QPixmap prev((const char **)pix_vertical);

	if 	( g_pScaleSrc->get_screen_version() == 1 )
	{
		painter.drawPixmap(719,pixmap_height,prev);
		painter.drawPixmap(638,pixmap_height,prev);
	}
	else if ( g_pScaleSrc->get_screen_version() == 2 )
	{
		painter.drawPixmap(103*9-2,pixmap_height,prev);
		painter.drawPixmap(103*8-2,pixmap_height,prev);
	}
	else if ( g_pScaleSrc->get_screen_version() == 3 )
	{
		painter.drawPixmap(130*9+2-4,pixmap_height,prev);
		painter.drawPixmap(130*8+2-4,pixmap_height,prev);
	}

        QPen penText (QColor(88,47,0));
        QPen penPage (QColor(0,0,255));

        int X=g_pScaleSrc->get_input_method_pinyin_offset();
        int Y=(rect.height()+m_Jian_rect.height())/2-3;

        int vertical_space= (m_Chinese_rect.width() - 7*2)/8;

#if 0
        if(m_bMakingPhrase){
            QRect temp;
            QString s=QString::fromUtf8(MAKEPHRASE_OPEN);     //unicode的utf8编码
            painter.setPen(penText);
            painter.drawText(X,Y,s);
            temp = metric.boundingRect(s);
            X+=temp.width();

            painter.setPen(penPage);
            painter.drawText(X,Y,m_ime_info.phrase);
            temp = metric.boundingRect(m_ime_info.phrase);
            X+=temp.width();

            s=QString::fromUtf8(MAKEPHRASE_CLOSE);
            painter.setPen(penText);
            painter.drawText(X,Y,s);
            temp = metric.boundingRect(s);
            X+=temp.width()+2;
        }
#endif

        //pinyin inputed by user
        painter.setPen(penText);
        painter.drawText(X,Y,QString(m_engine.get_formatted_pinyin().c_str()));

        Y=m_Jian_rect.height()+(m_Chinese_rect.height()+rect.height())/2-3;

        //翻页：上一页
        if(m_ime_info.prev_page_available()) {
            if (PY_PushButton_PrePage->isEnabled() == 0) {
                PY_PushButton_PrePage->setEnabled(true);
            }
        }
        else if (m_ime_info.pinyin.size() == 0)
        {
            if (PY_PushButton_PrePage->isEnabled() == 1) {
                PY_PushButton_PrePage->setEnabled(false);
            }
        }
        else
        {
            if (PY_PushButton_PrePage->isEnabled() == 1) {
                PY_PushButton_PrePage->setEnabled(false);
            }
        }

        //hanzi candidates
        m_ime_info.candidates_on_page=0;
        QString str;
        QRect hanzi;
        painter.setPen(penText);
        unsigned int xx = 0, yy = Y;

        for(unsigned int i=m_ime_info.first_visible, count = 1;m_ime_info.candidates_on_page<10&&i<m_ime_info.candidates_count;i++, count++) 
	{
            str= "";
            if(m_engine.isPhrase()){
                str+=get_phrase(i);
            }
            else{
                str+=get_char(i);
            }

            hanzi=metric.boundingRect(str);
            hanzi = QRect(hanzi.x(),hanzi.y()+10,hanzi.width(),hanzi.height()+40);//+40????

            //画分割线
            pixmap_left = g_pScaleSrc->get_input_method_pinyin_line_offset()*count -2;

            painter.drawPixmap(pixmap_left,pixmap_height,prev);

            if (count == 1)
                xx = (vertical_space - hanzi.width())/2-3;

            if((xx+hanzi.width())>=m_Chinese_rect.width()){//no space for this hanzi and right button
                if(i<m_ime_info.candidates_count-1){ //but we need another page
                    break; //so give up to draw this hanzi
                }
                else{
                    //this is the last page,do not need to show next button
                    //so just check if the left space is enough to show this hanzi
                    if((xx+hanzi.width())>m_Chinese_rect.width()){
                        break;//Ooops
                    }

                }
            }

            QString str1 =str.right(1);
            painter.drawText(xx,yy,str1);
            xx = (vertical_space - hanzi.width())/2 + count*(vertical_space + 2) - 1; //2是分割线的宽度
            m_ime_info.candidates_on_page++;
        }

        //翻页：下一页
        if(m_ime_info.next_page_available()) {
            if (PY_PushButton_NextPage->isEnabled() == 0) {
                PY_PushButton_NextPage->setEnabled(true);
            }
        }
        else if (m_ime_info.pinyin.size() == 0)
        {
            if (PY_PushButton_NextPage->isEnabled() == 1) {
                PY_PushButton_NextPage->setEnabled(false);
            }
        }
        else
        {
            if (PY_PushButton_NextPage->isEnabled() == 1) {
                PY_PushButton_NextPage->setEnabled(false);
            }
        }
}

bool QPinyinFrame::prev_page()                   //选词翻页:上一页
{
        if(m_ime_info.prev_page_available()){
                //qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);
                m_ime_info.first_visible-=m_ime_info.candidates_on_prev_page;
                //qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);
                update();
                return true;
        }
        else if (m_ime_info.pinyin.size() == 0) {
                    SendKey(Qt::Key_Left, 0);
                    return true;
        }
        else{
                return false;
        }
}

bool QPinyinFrame::next_page()                          //选词翻页:下一页
{
        if(m_ime_info.next_page_available()){
                //qDebug("next page,firt visible index %d->",m_ime_info.first_visible);
                m_ime_info.candidates_on_prev_page=m_ime_info.candidates_on_page;
                m_ime_info.first_visible+=m_ime_info.candidates_on_page;
                //qDebug("%d\n",m_ime_info.first_visible);
                update();
                return true;
        }
        else if (m_ime_info.pinyin.size() == 0) {
                    SendKey(Qt::Key_Right, 0);
                    return true;
                }
        else{
                return false;
        }
}

int QPinyinFrame::get_charunicode(unsigned int nIndexOnPage)      //获取unicode编码
{
        unsigned int index = m_ime_info.first_visible + nIndexOnPage;
        qDebug("get unicode:first visible index %d,current index %d,global index %d",
                m_ime_info.first_visible,nIndexOnPage,index);
        return (m_engine.get_char(index)).unicode();
}

/*
  return true if need to update UI
 */
bool QPinyinFrame::commit_selection(int k)            //按键键值的选取
{
    bool bUpdate = false;
    unsigned int index = ((k-'0')+9)%10;
    if(index<m_ime_info.candidates_on_page){
        m_engine.hit(m_ime_info.first_visible+index);
        if(m_engine.isPhrase()){
            QString phrase=get_phrase(m_ime_info.first_visible+index);
            if(m_bMakingPhrase){
                m_ime_info.phrase+=phrase;
            }
            else{
                qDebug("phrase unicode:");
                for(/*unsigned*/ int i=0;i<phrase.length();i++){
                    qDebug("%04X,",phrase[i].unicode());
                    SendKey(phrase[i].unicode());
                }
                //qDebug("\n");
            }
        }
        else{
            if(m_bMakingPhrase){
                m_ime_info.phrase+=get_char(m_ime_info.first_visible+index);
            }
            else{
                SendKey(get_charunicode(index));
            }
        }
        resetState();
        bUpdate=true;
    }
    return bUpdate;
}

bool QPinyinFrame::send_hanzi_mark(int ascii_mark)     //汉字标点符号
{
        int unicode = 0;
		//lg
		/*switch(ascii_mark){                             //ASCII码
                case ',':
                        unicode = COMMA_MARK;
                        break;
                case '.':
                        unicode = PERIOD_MARK;
                        break;
                case '<':
                        unicode = BOOK_MARK1;
                        break;
                case '>':
                        unicode = BOOK_MARK2;
                        break;
                case '?':
                        unicode = QUESTION_MARK;
                        break;
                case ':':
                        unicode = COLON_MARK;
                        break;
		}*/
		unicode = ascii_mark;
		//lg
        if(unicode!=0){
                SendKey(unicode);
                return true;
        }
        else{
                return false;
        }
}

bool QPinyinFrame::GetKey(int u, int k/*,int m*/)/*int unicode, int keycode, int modifiers*/
{
        qDebug("Pinyin:key code is %d ,0x%02X\n",u,k);
        if(k == 0)return true; //zengjia

        if(m_bEnglishMode)
        {
            if(u == 9 && k == Qt::Key_Tab)
            {
                    m_bEnglishMode = false;
                    update();
            }
            else if(u == 8 && k == Qt::Key_Backspace)
            {
                QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
                QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);
                return false;
            }
            else if(u == 7 && k == Qt::Key_Enter)
            {
                QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
                QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);
                return false;
            }
            else
            {
                    SendKey(u,k);
            }
            return true;
        }

        switch(k)
        {
            //case Qt::Key_F1:
            //return true;
            case Qt::Key_F2:
                return true;
            case Qt::Key_F3:
                return true;
            case Qt::Key_F4:
                return true;
            case Qt::Key_F5:
                return true;
            case Qt::Key_F6:
                return true;
            case Qt::Key_F7:
                return true;
            case Qt::Key_F8:
                return true;
            case Qt::Key_F9:
                return true;
            case Qt::Key_F10:
                return true;
            case Qt::Key_F11:
                return true;
            case Qt::Key_F12:
                return true;
            case Qt::Key_Control:
                return true;
            case Qt::Key_NumLock:
                return true;
            case Qt::Key_CapsLock:
                return true;
            case Qt::Key_Alt:
                return true;
        }

        bool bUpdate         = false;
        bool bKeyProcessed   = true;

        if( k >= '0' && k <= '9'){
commit:
            bUpdate=commit_selection(k);
            if(!bUpdate){
                bKeyProcessed=false;
            }
        }
        else if((k == '\'') || (k >= 'a' && k <= 'z')){     //小写字母
            //input:
            m_ime_info.pinyin+=(char)k;
            search();
            bUpdate=true;

        }
        else if(k >= 'A' && k <= 'Z'){    //大写字母
            //		k = k - 'A'+ 'a';
            //		goto input;
            SendKey(u,k);
        }
        else if(k == Qt::Key_Space){
            if(m_ime_info.candidates_on_page>0){
                k='1';
                goto commit;
            }
            else{
                bKeyProcessed=false;
            }
        }
        else if(k == Qt::Key_Right||k == Qt::Key_Down){
            if(!next_page()){
                bKeyProcessed=false;
            }
        }
        else if(k == Qt::Key_Left||k == Qt::Key_Up){
            if(!prev_page()){
                bKeyProcessed=false;
            }
        }
        else if( u == 8 && k == Qt::Key_Backspace){
            if(m_ime_info.pinyin.size()>0){
                String::iterator pos=m_ime_info.pinyin.end()-1;
                m_ime_info.pinyin.erase(pos);
                search();
                if (m_ime_info.pinyin.size() == 0)
                {
                    resetState();
                }

                bUpdate=true;
            }
            else{
                QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
                QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);

                return false;
                //SendKey(u,k);
            }

        }
        else if( (u == 7 && k == Qt::Key_Enter) && (m_ime_info.pinyin.size()<=0)){
            QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);
            return false;
        }
        else if(k == Qt::Key_F1){
            if(m_bMakingPhrase){
                //commit the new phrase
                m_bMakingPhrase=false;
                m_engine.append_phrase(m_ime_info.phrase,m_making_phrase_pinyin.c_str());

                //save table (just test)
                m_engine.save_table();
                //**********************************

                m_ime_info.phrase="";
                resetState();
                bUpdate=true;
            }
            else if(m_ime_info.pinyin.size()==0){
                qDebug("entering making phrase mode...");
                m_making_phrase_pinyin="";
                m_bMakingPhrase=true;
                bUpdate=true;
            }
        }
        else if(u == 9 && k == Qt::Key_Tab){
            m_bEnglishMode=true;
            bUpdate=true;
        }
        else if(  k == Qt::Key_Escape){
            if(m_bMakingPhrase){
                m_ime_info.phrase="";
                m_bMakingPhrase=false;
            }
            resetState();
            bUpdate=true;
        }
        else if(!send_hanzi_mark(k)){
            bKeyProcessed=false;
        }


        if(bUpdate){
            update();
        }

        if(!bKeyProcessed){
            SendKey(u,k);
        }


        return true;
}

void QPinyinFrame::show()
{
        QFrame::show ();
}

void QPinyinFrame::hide()
{
        QFrame::hide ();
        resetState();
        m_engine.save_table();
}

bool hit_test_helper(int x,int y,QRect& rect)
{
        if(x>=rect.left()&&x<=rect.right()&&
           y>=rect.top() &&y<=rect.bottom()){
                return true;
        }
        else{
                return false;
        }
}

void QPinyinFrame::SendKey(int u , int c)
{
    Q_UNUSED(c);
	QChar mychar(u);
	QString str(mychar);

//	InputMethod *pInputMethod = ((MainDialog *)(parent()->parent()))->m_pInputMethod;
//	pInputMethod->sendString(str);

	//emit sendchar(str);
//	QKeyEvent *me=new QKeyEvent(QEvent::KeyPress,c,0,str,false);
		//qDebug("lg %s %d ",str.toAscii().data(),str.length());
//	QApplication::postEvent(App->focusWidget(),me);

    if ((u != Qt::Key_Left) && (u != Qt::Key_Right))
    {
        InputMethod *pInputMethod = ((MainDialog  *)(parent()->parent()))->m_pInputMethod;
        pInputMethod->sendString(str);
    }
    else
    {
        QKeyEvent *me=new QKeyEvent(QEvent::KeyPress,u,0,str,false);
        QApplication::postEvent(qApp->focusWidget(),me);

        me=new QKeyEvent(QEvent::KeyRelease,u,0,str,false);
        QApplication::postEvent(qApp->focusWidget(),me);
    }
}
