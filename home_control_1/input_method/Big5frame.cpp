#include "Big5frame.h"

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
//#include <QWSServer>
#include <QtGui/QApplication>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QDesktopWidget>
#include <QtCore/QDebug>

#include "public.h"              //公共宏定义函数头文件，用于删除空白字符
#include "Config.h"              //字体和字符设置文件
//#include "Big5inputmethod.h"
#include "scim/Big5pinyin_char.h"
#include "Big5pinyinpanel.h"
#include "inputmethod.h"
#include "main/maindialog.h"
#include "ui_big5pinyinpanel.h"

const char table_file[]="/mnt/disk/input_method/fanti_pinyin_table.txt";
const char phrase_index_file[]="";
const char config_file[]="/mnt/disk/input_method/murphytalk.conf";        //字体配置

/*XPM图形绘制:主要是用于ime输入法编辑器的绘制*/
static const char * pix_prev[]={
"17 21 3 1",
"       c #80FFFF80FFFF",
"#      c #000000000000",
".      c #000000000000",
"              # ",
"             ## ",
"            #.# ",
"           #..# ",
"          #...# ",
"         #....# ",
"        #.....# ",
"       #......# ",
"      #.......# ",
"     #........# ",
"    #.........# ",
"     #........# ",
"      #.......# ",
"       #......# ",
"        #.....# ",
"         #....# ",
"          #...# ",
"           #..# ",
"            #.# ",
"             ## ",
"              # "};

static const char * pix_next[]={
"17 21 3 1",
"       c #80FFFF80FFFF",
"#      c #000000000000",
".      c #000000000000",
"  #             ",
"  ##            ",
"  #.#           ",
"  #..#          ",
"  #...#         ",
"  #....#        ",
"  #.....#       ",
"  #......#      ",
"  #.......#     ",
"  #........#    ",
"  #.........#   ",
"  #........#    ",
"  #.......#     ",
"  #......#      ",
"  #.....#       ",
"  #....#        ",
"  #...#         ",
"  #..#          ",
"  #.#           ",
"  ##            ",
"  #             "};

const int BIG5PINYIN_X=2;                           //汉字是2字节,光标移动位置是2
const int BIG5PINYIN_Y=2;

const unsigned short FAN=0x7e41;                    //unicode汉字编码表:0x7e41=繁
const unsigned short YING=0x82f1;                   //unicode汉字编码表:0x82f1=英
const char ABOUT[]="About";
const char MAKEPHRASE_OPEN []="\xe3\x80\x90\xe9\x80\xa0\xe8\xaf\x8d\x20";
const char MAKEPHRASE_CLOSE[]="\xe3\x80\x91";

//常用标点符号unicode编码表
const int COMMA_MARK =0xff0c;                       // ,
const int PERIOD_MARK=0x3002;                       // 。
const int BOOK_MARK1 =0x300a;                       //《  半角为ff1c  <
const int BOOK_MARK2 =0x300b;                       // 》 半角为ff1e  >
const int QUESTION_MARK=0xff1f;                     // ?
const int COLON_MARK =0xff1a;                       // :

char str_zhuyin[50] = "";
static char zhuyin_covert_table[37][4] = {"ㄅ", "ㄆ", "ㄇ", "ㄈ", "ㄉ", "ㄊ", "ㄋ", "ㄌ",
                            "ㄍ", "ㄎ", "ㄏ", "ㄐ", "ㄑ", "ㄒ", "ㄓ", "ㄔ",
                            "ㄕ", "ㄖ", "ㄗ", "ㄘ", "ㄙ", "ㄚ", "ㄛ", "ㄜ",
                            "ㄝ", "ㄞ", "ㄟ", "ㄠ", "ㄡ", "ㄢ", "ㄣ", "ㄤ",
                            "ㄥ", "ㄦ", "ㄧ", "ㄨ", "ㄩ"};

QBig5Frame::QBig5Frame(QWidget *parent):
    QFrame(parent),m_enginebig5(table_file,phrase_index_file),
    m_bEnglishMode(false),m_bMakingPhrase(false)                  ///*m_engine(table_file,phrase_index_file),*/两种输入法初始化都是未选定
{
    (new QHBoxLayout(this))->setEnabled(TRUE);
    /*Config config(config_file);                               //配置字体,字型大小,暂时未匹配config_file文件
    String font_name=config.get_as_string("font");              //字型:efont
    int font_size=config.get_as_int("font-size");               //字体大小:16
    qDebug("from config -> font=%s\n", font_name.c_str());
    qDebug("from config -> font-size=%d\n",font_size);
    if(font_name.size()>0){
        if(font_size<=0){
            font_size=font().pointSize();
        }

    setFont(QFont(font_name.c_str(),font_size));               //字体配置,c_str返回c字符串指针
    }*/
    setFont(QFont("efont",55));

    change=false;

    changekeyboard = (Big5PinyinPanel *)parent;
}

//bool g_bFuyinInputed = false;

bool QBig5Frame::filter(int id)                             //按键处理函数,unicode编码
{
    int ret = 0;
    int unicode= 0;//id;
	printf("filter:%08x\n", id);

    if((Qt::Key)id == Qt::Key_Backspace)        //自定义标识符id=8,对应Qt::Key_Backspace
        unicode = 8;
    else if((Qt::Key)id == Qt::Key_Tab)        //自动义TAB键标识符id=9
        unicode = 9;
    else if((Qt::Key)id == Qt::Key_Return)
        unicode = 13;
    else if((Qt::Key)id == Qt::Key_Shift || (Qt::Key)id == Qt::Key_CapsLock)  //shift键和Caps Lock键都可以切换大小写
        unicode = 0xffff;                               //只传送键值，不打印编码

    if (unicode != 0)
    {
       ret = GetKey(unicode, id);
       goto jump0;
    }

    if (id >= '0' && id <= '9')
    {
        ret=GetKey(id,id);
        return ret;
    }

    /*if ((strlen(str_zhuyin) > 0) && (str_zhuyin[strlen(str_zhuyin) - 1] == ' '))
    {
        return false;
    }*/

    if (id >= 'A' && id <= 'Z')
    {
        ret=GetKey(id,id);
        return ret;
    }

    int b_zh;
    int b_er;
    b_zh=0x3113;  //处理几个特殊的注音
    b_er=0x3126;
    if(id<'0' || (id>'9'&& id <'A') || (id>'Z' && id <'a') || (id>'z' && id<b_zh) || id>b_er)
    {
        ret=GetKey(id,id);
        return ret;
    }

    char str_pinyin[50];
    int len;

    if (id >= 'a' && id <= 'z')
    {
        if (id == 'u')
        {
            if (strlen(str_zhuyin) == 0)
            {
                id = 'w';
            }
        }
        else if (id == 'i')
        {
            if (strlen(str_zhuyin) == 0)
            {
                id = 'y';
            }
        }

        if ((id != 'a') && (id != 'e') && (id != 'i')
                && (id != 'o') && (id != 'u'))
        {
            ret = GetKey(id, id);
            //g_bFuyinInputed = true;
        }
        else
        {
            /*if (!g_bFuyinInputed)
            {
                ret = GetKey('\'', '\'');
            }*/
            ret = GetKey(id, id);
            //g_bFuyinInputed = false;
        }
        goto jump0;
    }

    int i, j;

    if (1)//strlen(str_zhuyin) == 0)
    {
        for (i = 1; i < 24; i++)                //搜索辅音字母表
        {
            if (scim_pinyin_initials[i].wstr[0] == id)
            {
                for (j = 0; j < scim_pinyin_initials[i].len; j++)
                {
                    unicode = scim_pinyin_initials[i].str[j];      //unicode等于搜索到的拼音
                    ret = GetKey(unicode, unicode);
                    //g_bFuyinInputed = true;
                }
                break;
            }
        }
    }

    if (unicode == 0)//i >= 24)
    {
        for (i = 1; i < 42; i++)            //搜索元音字母表,只搜索单个注音
        {
            if ((scim_pinyin_finals[i].wlen == 1) && (scim_pinyin_finals[i].wstr[0] == id))
            {
                /*if (!g_bFuyinInputed)
                {
                    ret = GetKey('\'', '\'');
                }*/

                for (j = 0; j < scim_pinyin_finals[i].len; j++)
                {
                    unicode = scim_pinyin_finals[i].str[j];
                    ret = GetKey(unicode, unicode);
                }
                //g_bFuyinInputed = false;
                break;
            }
        }
    }

jump0:
    strcpy(str_pinyin, m_enginebig5.get_formatted_pinyin().c_str());
    strcpy(str_zhuyin, "");
    //for (i = 0; i < strlen(str_pinyin); i++)
    //{
    i = 0;
    while (i < (int)strlen(str_pinyin))           //str_pinyin是用户输入的拼音
    {
        if (str_pinyin[i] == ' ')
        {
            strcat(str_zhuyin, " ");        //将空格\0添加到注音字符串的结尾
            i++;
            continue;
        }

        unicode = 0;
        len = 0;
        for (j = 1; j < 24; j++)  //比较辅音表最大长度
        {
            if (strncmp(str_pinyin + i, scim_pinyin_initials[j].str, scim_pinyin_initials[j].len) == 0)
            {
                if (scim_pinyin_initials[j].len > len)
                {
                    unicode = scim_pinyin_initials[j].wstr[0];
                    len = scim_pinyin_initials[j].len;
                }
            }
        }

        if (unicode == 0)
        {
            len = 0;
            for (j = 1; j < 42; j++)        //找不到辅音，找元音表
            {
                if (scim_pinyin_finals[j].wlen == 1)
                {
                    if (strncmp(str_pinyin + i, scim_pinyin_finals[j].str, scim_pinyin_finals[j].len) == 0)
                    {
                        if (scim_pinyin_finals[j].len > len)
                        {
                            unicode = scim_pinyin_finals[j].wstr[0];
                            len = scim_pinyin_finals[j].len;
                        }
                    }
                }
            }
        }

        if (unicode == 0)
        {
            break;
        }

        strcat(str_zhuyin, zhuyin_covert_table[unicode - 0x3105]);   //关联注音表

        i += len;
    }

    return ret;//GetKey(unicode,id);                 //unicode编码与软键盘按键标识对应
}

void QBig5Frame::languageChange(bool isEn)                  //输入法切换
{
    m_bEnglishMode=isEn;
    update();                                              //状态更新
}

QBig5Frame::~QBig5Frame()
{
    printf("ime killed\n");                                 //要加入public.h头文件，不然printf会报错
}

QSize QBig5Frame::sizeHint() const                          //开启IME窗口,与show()联合使用
{
    QBig5Frame *This=const_cast<QBig5Frame*>(this);
    int height=This->init_gui_dimention();
    return QSize(640,height);                               //height具体值是sHeight=y+rect.height()+5;
}

int QBig5Frame::init_gui_dimention()                        //窗体GUI初始化,无参函数返回值是height
{
    static int sHeight=-1;                                  //静态全局变量，sHeight在全局都分配变量
    if(sHeight>0)                                           //初始化为-1
    {
        return  sHeight;                                   // >0 则直接返回
    }
    const int pix_width=17,pix_height=21;                   //const规定pix_width,pix_height是常量
    QFontMetrics metric=this->fontMetrics();                //QFontMetrics获取字符串的size
    QRect rect=metric.boundingRect(QChar(FAN));             //绘制输入法标识符"繁"

    int width=640;                                          //widget宽640
    qDebug("text metric is %d %d %d,%d ",rect.x(),rect.y(),rect.width(),rect.height());

    int x=BIG5PINYIN_X;
    int y=BIG5PINYIN_Y;

    m_indicator_rect=QRect(x,y,rect.width(),rect.height());
    qDebug("indicator rect (%d,%d,%d,%d)",m_indicator_rect.left(),m_indicator_rect.top(),m_indicator_rect.width(),m_indicator_rect.height());

    //显示拼音字母
    y+=rect.height()+2;

    //第二行,显示汉字
    y+=2;

    m_leftbtn_rect=QRect(x,y+2,pix_width,pix_height);               //字体宽高16*12
    qDebug("left btn rect(%d %d %d %d)",m_leftbtn_rect.left(),m_leftbtn_rect.top(),m_leftbtn_rect.width(),m_leftbtn_rect.height());

    m_rightbtn_rect=QRect(width-230-pix_width,y+2,pix_width,pix_height);
    qDebug("right btn rect(%d %d %d %d)",m_rightbtn_rect.left(),m_rightbtn_rect.top(),m_rightbtn_rect.width(),m_rightbtn_rect.height());

    sHeight=y+rect.height()+5;

    return sHeight;
}

//编码处理函数
bool QBig5Frame::filter(int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)
{
    Q_UNUSED(modifiers);
    Q_UNUSED(autoRepeat);

    if(!isHidden() && isPress){                                 //isPress是按键处理
        if(m_bEnglishMode){                                     //当前是English模式
            if(unicode==9 && keycode==Qt::Key_Tab){             //从English模式切换至中文模式
                m_bEnglishMode=false;
                update();
            }
            else if(Qt::Key_F1<=keycode && keycode<=Qt::Key_F12)        //F1-F12之间的键值不作处理
            {
                return false;
            }
            else{                                               //除F1-F12外，其余键值都处理
                SendKey(unicode,keycode);
            }
            return true;
        }

        if (GetKey (unicode, keycode))                      //GetKey()返回true,表示获取键值
            return true;
        else
            return false;
    }

    else                                                    //无按键的情况:false
        return false;
}

//pix图形绘制函数
void QBig5Frame::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);

    QPainter painter(this);
    painter.setClipRect(e->rect());

    unsigned int width=640;

    QFontMetrics metric=painter.fontMetrics();
    QRect rect=metric.boundingRect(QChar(FAN));                 //显示"繁"的size=12
        QPen penText (QColor(0,0,0));

    int x=BIG5PINYIN_X;                              //汉字为2字节，所以BIG5PINYIN_X＝2
    int y=BIG5PINYIN_Y;                              //BIG5PINYIN_Y=2
	painter.setPen(penText);
	painter.drawText(x,rect.height(),QChar(m_bEnglishMode?YING:FAN));           //"英"，"繁"



    QPen penGray (QColor(128,128,128));
    QPen penWhite(QColor(255,255,255));
    QPen penPage (QColor(0,0,255));

    //拼音字母与"简",“繁"分隔区域
    painter.setPen(penGray);
    painter.drawLine(x+rect.width()+2,y,x+rect.width()+2,y+rect.height()); //xiugai20131106     //灰竖线 |
    painter.setPen(penWhite);
    painter.drawLine(x+rect.width()+3,y,x+rect.width()+3,y+rect.height());      //白竖线 |

    //中文输入法,用画笔绘制候选词
    int X=x+rect.width()+7;                                         //rect是"繁"的宽高,x=2
    int Y=rect.height();
//    if(m_bMakingPhrase){                                            //中文词组输入法
//        QRect temp;

//        QString s=tr(MAKEPHRASE_OPEN);               //unicode编码,取出汉字
//        painter.setPen(penText);                                    //绘制候选汉字,penText为黑色(0,0,0)
//        painter.drawText(X,Y,s);
//        temp=metric.boundingRect(s);
//        X+=temp.width();                                            //下一个汉字显示的坐标

//        painter.setPen(penPage);                                //绘制候选汉字前的编号1,2,...
//        painter.drawText(X,Y,m_ime_info.phrase);
//        temp=metric.boundingRect(m_ime_info.phrase);
//        X+= temp.width();

//        s=tr(MAKEPHRASE_CLOSE);
//        painter.setPen(penText);
//        painter.drawText(X,Y,s);
//        temp=metric.boundingRect(s);
//        X+=temp.width()+2;
//    }

    //绘制拼音字母栏的内容，显示注音字符串
    painter.setPen(penText);
    painter.drawText(X,Y, tr(str_zhuyin));

    //分隔拼音和汉字
    y+=rect.height()+2;
    painter.setPen(penGray);
    painter.drawLine(x,y,x+width - 100-160 -20 +30,y);   //xiugai
    painter.setPen(penWhite);
    painter.drawLine(x,y+1,x+width - 100-160 - 20 +30,y+1);

    //第2行，汉字候选词区
    y+=2;

    //绘制汉字候选词区域 ,最底下的横线
    Y=y+rect.height();
    painter.setPen(penWhite);
    painter.drawLine(x,Y+2,x+width - 100-160 -20+30,Y+2);

    if(m_ime_info.prev_page_available()){         //翻页按钮：上一页,x,y是鼠标点击有效坐标
        QPixmap prev((const char **)pix_prev);
        painter.drawPixmap(x,y,prev);
        ((Big5PinyinPanel *)parent())->ui->toolButtonPrevPage->setEnabled(true);
        x+=prev.width()+2;
    }
    else
    {
            ((Big5PinyinPanel *)parent())->ui->toolButtonPrevPage->setEnabled(false);
    }

    //汉字候选词
    m_ime_info.candidates_on_page=0;
    QString str;
    QRect hanzi;
    painter.setPen(penText);
    for(unsigned int i=m_ime_info.first_visible,xx=X,yy=Y-3;m_ime_info.candidates_on_page<10&&i<m_ime_info.candidates_count;i++){
        str.sprintf("%d.",(i-m_ime_info.first_visible+1)%10);//1. 2. ... 9. 0.
        if(m_enginebig5.isPhrase()){
            str+=get_phrase(i);
        }
        else{
            str+=get_char(i);
        }
        hanzi=metric.boundingRect(str);

        if(xx+hanzi.width()>=(unsigned int)m_rightbtn_rect.left()){//no space for this hanzi and right button
            if(i<m_ime_info.candidates_count-1){ //but we need another page
                break; //so give up to draw this hanzi
            }
            else{
                //this is the last page,do not need to show next button
                //so just check if the left space is enough to show this hanzi
                if(xx+hanzi.width()>width){
                    break;//Ooops
                }
            }
        }
        QString str1 = str.right(1);
        painter.drawText(xx,yy+2,str1);  //绘制汉字
        xx+=hanzi.width()+9;//10
        m_ime_info.candidates_on_page++;
      }

    if(m_ime_info.next_page_available()){            //翻页按钮:下一页
        QPixmap next((const char **)pix_next);
        painter.drawPixmap(width-230-next.width() - 10,y,next);//10
        ((Big5PinyinPanel *)parent())->ui->toolButtonNextPage->setEnabled(true);
    }
    else
    {
            ((Big5PinyinPanel *)parent())->ui->toolButtonNextPage->setEnabled(false);
    }

}

bool QBig5Frame::prev_page()                   //选词翻页:上一页
{
        if(m_ime_info.prev_page_available()){
                //qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);
                m_ime_info.first_visible-=m_ime_info.candidates_on_prev_page;
                //qDebug("prev page,firt visible index %d->",m_ime_info.first_visible);
                update();
                return true;
        }
        else{
                return false;
        }
}

bool QBig5Frame::next_page()                          //选词翻页:下一页
{
        if(m_ime_info.next_page_available()){
                //qDebug("next page,firt visible index %d->",m_ime_info.first_visible);
                m_ime_info.candidates_on_prev_page=m_ime_info.candidates_on_page;
                m_ime_info.first_visible+=m_ime_info.candidates_on_page;
                //qDebug("%d\n",m_ime_info.first_visible);
                update();
                return true;
        }
        else{
                return false;
        }
}

void QBig5Frame::show()                                     //显示IME窗口
{
    QFrame::show();
}

void QBig5Frame::hide()
{
    QFrame::hide();
    resetState();
    m_enginebig5.save_table();                          //词频保存
}


int QBig5Frame::get_charunicode(unsigned int nIndexOnPage)      //获取unicode编码
{
        unsigned int index = m_ime_info.first_visible + nIndexOnPage;
        qDebug("get unicode:first visible index %d,current index %d,global index %d",
                m_ime_info.first_visible,nIndexOnPage,index);
        return (m_enginebig5.get_char(index)).unicode();        //get_char返回QChar c(m_chars[index])
}


/*
  return true if need to update UI
 */
bool QBig5Frame::commit_selection(int k)            //按键键值的选取
{
        bool bUpdate = false;
        unsigned int index = ((k-'0')+9)%10;

        if(index<m_ime_info.candidates_on_page){
                m_enginebig5.hit(m_ime_info.first_visible+index);
                if(m_enginebig5.isPhrase()){
                        QString phrase=get_phrase(m_ime_info.first_visible+index);          //返回值:str+=QChar(m_phrases[index][i])
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
                strcpy(str_zhuyin, "");                 //选择汉字后清空注音显示
                bUpdate=true;
        }
        return bUpdate;
}

bool QBig5Frame::send_hanzi_mark(int ascii_mark)     //汉字标点符号
{
        int unicode = 0;

		//lg
		/*switch(ascii_mark){                             //ASCII码
                case ',':
                        unicode = COMMA_MARK;           //0xff0c
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



bool big5_hit_test_helper(int x,int y,QRect& rect)
{
        if(x>=rect.left()&&x<=rect.right()&&
           y>=rect.top() &&y<=rect.bottom()){
                return true;
        }
        else{
                return false;
        }
}

void QBig5Frame::mouseReleaseEvent(QMouseEvent* m)
{
#if 0
    bool bUpdate=false;
    int x=m->x();
    int y=m->y();

    if(big5_hit_test_helper(x,y,m_about_rect)){

    }
    else if(big5_hit_test_helper(x,y,m_leftbtn_rect)){
                    prev_page();
    }
    else if(big5_hit_test_helper(x,y,m_rightbtn_rect)){
                    next_page();
    }
    else if(big5_hit_test_helper(x,y,m_indicator_rect)){
       //在英/繁之间切换
       m_bEnglishMode=!m_bEnglishMode;

		if (m_bEnglishMode == false)
		{
			changekeyboard->on_btn_caps_clicked();
		}

       bUpdate=true;
       change=m_bEnglishMode;

	   if(change){    //change=m_bEnglishMode==true,显示注音
		   resetState();
		   str_zhuyin[0] = '\0';
		   changekeyboard->showEngishkeyboard();
	   }
	   else if(!change){     //change=m_bEnglishMode==false,显示拼音键盘
		   changekeyboard->showzhuyinkeyboard();
	   }
     }

    if(bUpdate){
        update();
    }
#endif
}

void QBig5Frame::keyPressEvent(QKeyEvent*)
{
}

void QBig5Frame::SendKey(int u , int c)                             //键值发送
{
    Q_UNUSED(c);

	QChar mychar(u);
	QString str(mychar);
	//emit sendchar(str);

	InputMethod *pInputMethod = ((MainDialog *)(parent()->parent()))->m_pInputMethod;
	pInputMethod->sendString(str);
}

//主要处理中文模式的按键情况
bool QBig5Frame::GetKey(int u, int k)     /*int unicode, int keycode, int modifiers*/
{
    if(k == 0)return true;
        qDebug("Big5:key code is 0x%02X\n",k);
#if 1                                                       //1: 表示英文模式m_bEnglishMode
        if(m_bEnglishMode){
                if(u == 9 && k == Qt::Key_Tab){
                        m_bEnglishMode = false;             //Tab键进行中英文切换
                        update();                           //QWidget::update()更新窗口部件，处理绘制事件
                }
                else if(u == 8 && k == Qt::Key_Backspace){
					QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
					QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);

                    return false;                           //GetKey返回false
                }
                else{
                        SendKey(u,k);                       //键盘的其他按键(除了Tab和Backspace)
                }
                return true;
        }
#endif
        switch(k)                                           //有键按下，则返回true
        {
                //case Qt::Key_F1:
                        //return true;
                case Qt::Key_F2:                            //GetKey(u,Qt::Key_F2),返回true，则已经获取键值
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
            bUpdate=commit_selection(k);                //commit_selection（）返回ture
                if(!bUpdate){
                        bKeyProcessed=false;
                }
        }
        else if(k >= 'a' && k <= 'z'){
//input:
                m_ime_info.big5pinyin+=(char)k;
                search();                               //字符搜索函数
                bUpdate=true;

        }

        //因为大写字母不需要搜索汉字表，所以不需要调用函数search()
        else if(k >= 'A' && k <= 'Z'){                              //大写字母
            SendKey(u,k);                                           //SendKey()主要处理英文模式
        }
        else if(k == Qt::Key_Space){                                //空格键处理,Qt编码为:0x20
                if(m_ime_info.candidates_on_page>0){
                        k='1';
                        goto commit;                                //返回数字键选择
                }
                else{
                        bKeyProcessed=false;
                }
        }
        else if(k == Qt::Key_Right||k == Qt::Key_Down){             //上下左右方向键处理，实际中没有设置上下左右键
                if(!next_page()){
                        bKeyProcessed=false;
                }
        }
        else if(k == Qt::Key_Left||k == Qt::Key_Up){
                if(!prev_page()){
                        bKeyProcessed=false;
                }
        }
        else if( u == 8 && k == Qt::Key_Backspace){      //中文模式,退格键处理,删除整个注音
                int i;
                int size;
                int len = 0;
                char str_pinyin[50];

                strcpy(str_pinyin, m_enginebig5.get_formatted_pinyin().c_str());
                size = strlen(str_pinyin);
                if (size > 0)
                {
                    while (str_pinyin[size - 1] == ' ')
                    {
                        str_pinyin[size - 1] = '\0';
                        size--;
                        if (size == 0) break;
                    }
                }

                if (size == 0)
                {
					QKeyEvent modifiedEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
					QApplication::sendEvent(QApplication::focusWidget(), &modifiedEvent);
//                    curVal.chop(1);                 //从字符串末尾开始删除1个字符
//                    ui->display->setText(curVal);   //设定光标显示位置

                }
                else
                {
                    for (i = 1; i < 24; i++)            //辅音表搜索
                    {
                        if (size >= scim_pinyin_initials[i].len)
                        {
                            if ((len < scim_pinyin_initials[i].len) && (scim_pinyin_initials[i].wlen == 1))
                            {
                                if (strncmp(str_pinyin + size - scim_pinyin_initials[i].len,
                                            scim_pinyin_initials[i].str, scim_pinyin_initials[i].len) == 0)
                                {
                                    len = scim_pinyin_initials[i].len;
                                }
                            }
                        }
                    }

                    for (i = 1; i < 42; i++)            //元音表搜索
                    {
                        if ((size >= scim_pinyin_finals[i].len) && (scim_pinyin_finals[i].wlen == 1))
                        {
                            if (len < scim_pinyin_finals[i].len)
                            {
                                if (strncmp(str_pinyin + size - scim_pinyin_finals[i].len,
                                            scim_pinyin_finals[i].str, scim_pinyin_finals[i].len) == 0)
                                {
                                    len = scim_pinyin_finals[i].len;
                                }
                            }
                        }
                    }

                    if (len > 0)
                    {
                        for (i = 0; i < len; i++)
                        {
                            String::iterator pos=m_ime_info.big5pinyin.end()-1;     //光标位置处理
                            m_ime_info.big5pinyin.erase(pos);
                        }
                        search();

						if (m_ime_info.big5pinyin.size() == 0)
						{
							resetState();
							str_zhuyin[0] = '\0';
						}

                        bUpdate=true;
                    }
                    else{
                        return false;
                            //SendKey(u,k);
                    }
               }
        }
        else if(k == Qt::Key_F1){
                if(m_bMakingPhrase){
                        //commit the new phrase
                        m_bMakingPhrase=false;
                        m_enginebig5.append_phrase(m_ime_info.phrase,m_making_phrase_big5pinyin.c_str());

                        //save table (just test)
                        m_enginebig5.save_table();
                        //**********************************

                        m_ime_info.phrase="";
                        resetState();
                        bUpdate=true;
                }
                else if(m_ime_info.big5pinyin.size()==0){
                        qDebug("entering making phrase mode...");
                        m_making_phrase_big5pinyin="";
                        m_bMakingPhrase=true;
                        bUpdate=true;
                }
        }
        else if(u == 9 && k == Qt::Key_Tab){                    //此处Tab用于中英文切换
                m_bEnglishMode=true;
                bUpdate=true;
        }
        else if(  k == Qt::Key_Escape){                         //返回原来的英文输入法
                if(m_bMakingPhrase){
                        m_ime_info.phrase="";
                        m_bMakingPhrase=false;
                }
                resetState();
                bUpdate=true;
        }
        else if(!send_hanzi_mark(k)){                       //非汉字输入的情况处理
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
