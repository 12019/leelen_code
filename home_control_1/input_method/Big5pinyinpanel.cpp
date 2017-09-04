#include "Big5pinyinpanel.h"
#include "ui_big5pinyinpanel.h"
#include <QtCore/QSignalMapper>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include "Big5frame.h"

extern char str_zhuyin[50];

static int changflage=1;
static int changflage1=1;
static int changflage2=1;
static int changflage3=1;
static int changflage4=1;

Big5PinyinPanel::Big5PinyinPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Big5PinyinPanel),
    curVal(""),charCount(50),isEn(false),m_isDrag(false)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    py = new QBig5Frame(this);                              //拼音输入对象,作为display()槽函数的信号
    py->setMinimumSize(py->parentWidget()->width(), py->sizeHint().height());
    py->resize(py->parentWidget()->width(), py->sizeHint().height());
    //ui->gridLayout_4->addWidget(py, 1,0,1,1);    //gridLayout_4指向整个输入法窗口，在窗口中加入IME(输入法编辑器)
    //ui->gridLayout_4->addWidget(ui->keyboard, 2, 0, 1, 1); //IME与键盘关联
    QRect rc = py->geometry();
        rc.moveTo(30, 5);
    py->setGeometry(rc);
    py->lower();
    ui->frame->lower();
    //zj
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);
    /**/
    rc = ui->widget_2->geometry();
        rc.moveTo(0, 50);
    ui->widget_2->setGeometry(rc);
    ui->btnchangeChinese->hide();
    signalMapper = new QSignalMapper(this);                         //信号映射器
    allButtons = findChildren<QToolButton *>();                     //findChildren函数:获取所有在主窗口的QToolButton
    for (int i=0;i<allButtons.count();i++) {                                        //根据按键编号逐个查找
        connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(allButtons.at(i), i);                              //信号逐次扫描
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT( doOperation(int)));
    connect(py, SIGNAL(sendchar(QString)), this, SLOT(display(QString)));            //拼音显示

    ui->btn_sym1->hide();
    ui->btn_sym2->hide();
    ui->btn_sym4->hide();//zj
    ui->btn_caps->hide();
    ui->btn_enter->hide();
}

Big5PinyinPanel::~Big5PinyinPanel()
{
    delete py;
    delete ui;
    delete signalMapper;
}

void Big5PinyinPanel::setCharCount(int count)
{
    charCount = count;
}

void Big5PinyinPanel::setEchodMode(QLineEdit::EchoMode echmode)
{
	Q_UNUSED(echmode);
//    ui->display->setEchoMode(echmode);
}



void Big5PinyinPanel::doOperation(int btn)//键盘按钮处理
{
    emit updateTime();         //emit 信号:调用void updateTime()函数
    QString strKeyId;
    strKeyId = allButtons.at(btn)->accessibleName();         //获取键值的ID号
    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);                  //QString::toInt函数:将字符串(按键ID)转为16进制码

	printf("doOperation:%08x\n", keyId);

    //以下程序主要是屏蔽IME中的退格键与UI中的退格键，防止冲突
    if (/*keyId == Qt::Key_Backspace
     ||*/ keyId == Qt::Key_Escape
     || keyId == Qt::Key_Enter
     || keyId == Qt::Key_Shift
     || keyId == Qt::Key_CapsLock
     || keyId == Qt::Key_Tab
    )
        return;
    py->filter(keyId);                                      //拼音键值处理

}

void Big5PinyinPanel::display(QString ch)
{
	//Q_UNUSED(ch);

	printf("display %s\n", ch.toLatin1().data());
//    if(ui->display->cursorPosition() > charCount)//数字显示达到最大数，返回
//            return;
//    curVal += ch;
//    ui->display->setText(curVal);       //显示输入的所有字符
}




void Big5PinyinPanel::setCurval(QString curVal)     //光标位置设置
{
	Q_UNUSED(curVal);

    this->curVal = "";
    /*ui->display->clear();
    ui->warning->setText(curVal);                   //QLabel(用warning命名)用于定位光标*/
}


void Big5PinyinPanel::changeEvent(QEvent *e)     //Button按钮字符的改变
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void Big5PinyinPanel::on_btn_delete_clicked()
{
//    if(!py->filter(Qt::Key_Backspace))
//    {
//        curVal.chop(1);                 //从字符串末尾开始删除1个字符
//        ui->display->setText(curVal);   //设定光标显示位置
//    }

}

void Big5PinyinPanel::on_btn_hide_clicked()                 //隐藏输入法
{
    close();
    emit closeDigtal();
}


void Big5PinyinPanel::on_btn_enter_clicked()                //回车按键
{
    emit sendOrder(curVal);
}


void Big5PinyinPanel::mousePressEvent(QMouseEvent *e)
{
    int x,y;
    x=e->x();
    y=e->y();
    if(y>25 && y<55 &&  py->m_ime_info.candidates_count>0)
    {
        printf("on hanzi\n");
        if(x>30 && x<75)py->filter(49);
        if(x>77 && x<121)py->filter(50);
        if(x>123 && x<167)py->filter(51);
        if(x>169 && x<215)py->filter(52);
        if(x>217 && x<262)py->filter(53);
        if(x>264 && x<309)py->filter(54);
        if(x>310 && x<355)py->filter(55);
        if(x>356 && x<399)py->filter(56);
        return;
    }
    {
        if(e->button()==Qt::LeftButton)
        {           m_dragPosition = e->globalPos() - frameGeometry().topLeft();
            e->accept();
            m_isDrag = true;

            return;
        }
    }
}

void Big5PinyinPanel::mouseReleaseEvent(QMouseEvent* m)
{
    Q_UNUSED(m);
    m_isDrag=false;
    return;
}

void Big5PinyinPanel::mouseMoveEvent(QMouseEvent *e)
{
    int x,y;
    x=e->x();
    y=e->y();
    if(y>25 && y<55 && !py->m_bEnglishMode && x>30 && x<400)
    {
        printf("return....\n");
        return;
    }
    if(m_isDrag)
    {
        if(e->buttons() & Qt::LeftButton){
            move(e->globalPos()-m_dragPosition);
            e->accept();
        }
    }
}

//显示注音键盘
void Big5PinyinPanel::showzhuyinkeyboard()
{
    ui->btn_d->show();
    ui->btn_ai->show();
    ui->btn_v->show();
    ui->btn_ie->show();
		printf("lg 1\n");
        ui->btn_p->setText(QString::fromUtf8("ㄅ"));
        ui->btn_p->setAccessibleName("0x62");
        ui->btn_t->setText(QString::fromUtf8("ㄉ"));
        ui->btn_t->setAccessibleName("0x64");
        ui->btn_ch->setText(QString::fromUtf8("ㄓ"));
        ui->btn_ch->setAccessibleName("0x3113");
        ui->btn_o->setText(QString::fromUtf8("ㄚ"));
        ui->btn_o->setAccessibleName("0x61");
        ui->btn_ei->setText(QString::fromUtf8("ㄞ"));
        ui->btn_ei->setAccessibleName("0x311E");
        ui->btn_en->setText(QString::fromUtf8("ㄢ"));
        ui->btn_en->setAccessibleName("0x3122");
        ui->btn_d->setText(QString::fromUtf8("ㄦ"));
        ui->btn_d->setAccessibleName("0x3126");


        ui->btn_g->setText(QString::fromUtf8("ㄚ"));
        ui->btn_g->setAccessibleName("0x61");
        ui->btn_j->setText(QString::fromUtf8("ㄞ"));
        ui->btn_j->setAccessibleName("0x311E");
        ui->btn_z->setText(QString::fromUtf8("ㄢ"));
        ui->btn_z->setAccessibleName("0x3122");
        ui->btn_y->setText(QString::fromUtf8("ㄦ"));
        ui->btn_y->setAccessibleName("0x3126");


    ui->btn_m->move(65,40);
    ui->btn_n->move(110,40);
    ui->btn_k->move(155,40);
    ui->btn_q->move(200,40);
    ui->btn_sh->move(245,40);
    ui->btn_c->move(290,40);
    ui->btn_u->move(335,40);
    ui->btn_e->move(382,40);
    ui->btn_ao->move(428,40);

        ui->btn_ie->setText(QString::fromUtf8("ㄆ"));                  //注音:ㄆ所在的位置0x3106
        ui->btn_ie->setAccessibleName("0x70");                       //unicode码
        ui->btn_m->setText(QString::fromUtf8("ㄊ"));
        ui->btn_m->setAccessibleName("0x74");
        ui->btn_n->setText(QString::fromUtf8("ㄍ"));
        ui->btn_n->setAccessibleName("0x67");
        ui->btn_k->setText(QString::fromUtf8("ㄐ"));
        ui->btn_k->setAccessibleName("0x6a");
        ui->btn_q->setText(QString::fromUtf8("ㄔ"));
        ui->btn_q->setAccessibleName("0x3114");
        ui->btn_sh->setText(QString::fromUtf8("ㄗ"));
        ui->btn_sh->setAccessibleName("0x7a");
        ui->btn_c->setText(QString::fromUtf8("ㄧ"));
        ui->btn_c->setAccessibleName("0x69");
        ui->btn_u->setText(QString::fromUtf8("ㄛ"));
        ui->btn_u->setAccessibleName("0x6f");
        ui->btn_e->setText(QString::fromUtf8("ㄟ"));
        ui->btn_e->setAccessibleName("0x311F");
        ui->btn_ao->setText(QString::fromUtf8("ㄣ"));
        ui->btn_ao->setAccessibleName("0x3123");

        ui->btn_ai->setText(QString::fromUtf8("ㄇ"));
        ui->btn_ai->setAccessibleName("0x6d");
        ui->btn_an->setText(QString::fromUtf8("ㄋ"));
        ui->btn_an->setAccessibleName("0x6e");
        ui->btn_er->setText(QString::fromUtf8("ㄎ"));
        ui->btn_er->setAccessibleName("0x6b");
        ui->btn_f->setText(QString::fromUtf8("ㄑ"));
        ui->btn_f->setAccessibleName("0x71");
        ui->btn_l->setText(QString::fromUtf8("ㄕ"));
        ui->btn_l->setAccessibleName("0x3115");
        ui->btn_h->setText(QString::fromUtf8("ㄘ"));
        ui->btn_h->setAccessibleName("0x63");
        ui->btn_x->setText(QString::fromUtf8("ㄨ"));
        ui->btn_x->setAccessibleName("0x75");
        ui->btn_r->setText(QString::fromUtf8("ㄜ"));
        ui->btn_r->setAccessibleName("0x65");
        ui->btn_s->setText(QString::fromUtf8("ㄠ"));
        ui->btn_s->setAccessibleName("0x3120");
        ui->btn_v->setText(QString::fromUtf8("ㄤ"));
        ui->btn_v->setAccessibleName("0x3124");

        ui->btn_ou->setText(QString::fromUtf8("ㄈ"));
        ui->btn_ou->setAccessibleName("0x66");
        ui->btn_eng->setText(QString::fromUtf8("ㄌ"));
        ui->btn_eng->setAccessibleName("0x6c");
        ui->btn_a->setText(QString::fromUtf8("ㄏ"));
        ui->btn_a->setAccessibleName("0x68");
        ui->btn_zh->setText(QString::fromUtf8("ㄒ"));
        ui->btn_zh->setAccessibleName("0x78");
        ui->btn_ang->setText(QString::fromUtf8("ㄖ"));
        ui->btn_ang->setAccessibleName("0x72");
        ui->btn_b->setText(QString::fromUtf8("ㄙ"));
        ui->btn_b->setAccessibleName("0x73");
        ui->btncomma->setText(QString::fromUtf8("ㄩ"));
        ui->btncomma->setAccessibleName("0x76");
        ui->btnPeriod->setText(QString::fromUtf8("ㄝ"));
        ui->btnPeriod->setAccessibleName("0x311D");
        ui->btnPeriod_2->setText(QString::fromUtf8("ㄡ"));
        ui->btnPeriod_2->setAccessibleName("0x3121");
        ui->btnPeriod_3->setText(QString::fromUtf8("ㄥ"));
        ui->btnPeriod_3->setAccessibleName("0x3125");

        ui->btn_s_2->setText(QChar('-'));
        ui->btn_s_2->setAccessibleName("0x2d");
//        ui->btncomma->setText(QChar('-'));
//        ui->btncomma->setAccessibleName("0x2d");
//        ui->btnPeriod->setText(QChar('.'));
//        ui->btnPeriod->setAccessibleName("0x2e");

//		ui->btn_b->setText(QString::fromUtf8("ㄅ"));
//        ui->btn_b->setAccessibleName("0x62");
//		ui->btn_d->setText(QString::fromUtf8("ㄉ"));
//        ui->btn_d->setAccessibleName("0x64");
//		ui->btn_zh->setText(QString::fromUtf8("ㄓ"));
//        ui->btn_zh->setAccessibleName("0x3113");
//		ui->btn_a->setText(QString::fromUtf8("ㄚ"));
//        ui->btn_a->setAccessibleName("0x61");
//		ui->btn_ai->setText(QString::fromUtf8("ㄞ"));
//        ui->btn_ai->setAccessibleName("0x311E");
//		ui->btn_an->setText(QString::fromUtf8("ㄢ"));
//        ui->btn_an->setAccessibleName("0x3122");
//		ui->btn_er->setText(QString::fromUtf8("ㄦ"));
//        ui->btn_er->setAccessibleName("0x3126");

//		ui->btn_p->setText(QString::fromUtf8("ㄆ"));                  //注音:ㄆ所在的位置0x3106
//        ui->btn_p->setAccessibleName("0x70");                       //unicode码
//		ui->btn_t->setText(QString::fromUtf8("ㄊ"));
//        ui->btn_t->setAccessibleName("0x74");
//		ui->btn_g->setText(QString::fromUtf8("ㄍ"));
//        ui->btn_g->setAccessibleName("0x67");
//		ui->btn_j->setText(QString::fromUtf8("ㄐ"));
//        ui->btn_j->setAccessibleName("0x6a");
//		ui->btn_ch->setText(QString::fromUtf8("ㄔ"));
//        ui->btn_ch->setAccessibleName("0x3114");
//		ui->btn_z->setText(QString::fromUtf8("ㄗ"));
//        ui->btn_z->setAccessibleName("0x7a");
//		ui->btn_y->setText(QString::fromUtf8("ㄧ"));
//        ui->btn_y->setAccessibleName("0x69");
//		ui->btn_o->setText(QString::fromUtf8("ㄛ"));
//        ui->btn_o->setAccessibleName("0x6f");
//		ui->btn_ei->setText(QString::fromUtf8("ㄟ"));
//        ui->btn_ei->setAccessibleName("0x311F");
//		ui->btn_en->setText(QString::fromUtf8("ㄣ"));
//        ui->btn_en->setAccessibleName("0x3123");

//		ui->btn_m->setText(QString::fromUtf8("ㄇ"));
//        ui->btn_m->setAccessibleName("0x6d");
//		ui->btn_n->setText(QString::fromUtf8("ㄋ"));
//        ui->btn_n->setAccessibleName("0x6e");
//		ui->btn_k->setText(QString::fromUtf8("ㄎ"));
//        ui->btn_k->setAccessibleName("0x6b");
//		ui->btn_q->setText(QString::fromUtf8("ㄑ"));
//        ui->btn_q->setAccessibleName("0x71");
//		ui->btn_sh->setText(QString::fromUtf8("ㄕ"));
//        ui->btn_sh->setAccessibleName("0x3115");
//		ui->btn_c->setText(QString::fromUtf8("ㄘ"));
//        ui->btn_c->setAccessibleName("0x63");
//		ui->btn_u->setText(QString::fromUtf8("ㄨ"));
//        ui->btn_u->setAccessibleName("0x75");
//		ui->btn_e->setText(QString::fromUtf8("ㄜ"));
//        ui->btn_e->setAccessibleName("0x65");
//		ui->btn_ao->setText(QString::fromUtf8("ㄠ"));
//        ui->btn_ao->setAccessibleName("0x3120");
//		ui->btn_ang->setText(QString::fromUtf8("ㄤ"));
//        ui->btn_ang->setAccessibleName("0x3124");

//		ui->btn_f->setText(QString::fromUtf8("ㄈ"));
//        ui->btn_f->setAccessibleName("0x66");
//		ui->btn_l->setText(QString::fromUtf8("ㄌ"));
//        ui->btn_l->setAccessibleName("0x6c");
//		ui->btn_h->setText(QString::fromUtf8("ㄏ"));
//        ui->btn_h->setAccessibleName("0x68");
//		ui->btn_x->setText(QString::fromUtf8("ㄒ"));
//        ui->btn_x->setAccessibleName("0x78");
//		ui->btn_r->setText(QString::fromUtf8("ㄖ"));
//        ui->btn_r->setAccessibleName("0x72");
//		ui->btn_s->setText(QString::fromUtf8("ㄙ"));
//        ui->btn_s->setAccessibleName("0x73");
//		ui->btn_v->setText(QString::fromUtf8("ㄩ"));
//        ui->btn_v->setAccessibleName("0x76");
//		ui->btn_ie->setText(QString::fromUtf8("ㄝ"));
//        ui->btn_ie->setAccessibleName("0x311D");
//		ui->btn_ou->setText(QString::fromUtf8("ㄡ"));
//        ui->btn_ou->setAccessibleName("0x3121");
//		ui->btn_eng->setText(QString::fromUtf8("ㄥ"));
//        ui->btn_eng->setAccessibleName("0x3125");
//        ui->btncomma->setText(QChar('-'));
//        ui->btncomma->setAccessibleName("0x2d");
//        ui->btnPeriod->setText(QChar('.'));
//        ui->btnPeriod->setAccessibleName("0x2e");
}


//显示英式键盘
void Big5PinyinPanel::showEngishkeyboard()
{
      ui->btn_caps->setEnabled(true);

      ui->btn_eng->setText(QChar('!'));
      ui->btn_eng->setAccessibleName("0x21");
      ui->btn_a->setText(QChar('@'));
      ui->btn_a->setAccessibleName("0x40");
      ui->btn_zh->setText(QChar('#'));
      ui->btn_zh->setAccessibleName("0x23");
      ui->btn_ang->setText(QChar('$'));
      ui->btn_ang->setAccessibleName("0x24");
      ui->btn_b->setText(QChar('%'));
      ui->btn_b->setAccessibleName("0x25");
//      ui->btncomma->setText(QChar('^'));
//      ui->btncomma->setAccessibleName("0x5e");
      ui->btncomma->setText("&&");
      ui->btncomma->setAccessibleName("0x26");
      ui->btnPeriod->setText(QChar('*'));
      ui->btnPeriod->setAccessibleName("0x2a");
  //    ui->btncomma->setText(QChar('$'));///////
  //    ui->btncomma->setAccessibleName("0x24");
      ui->btnPeriod_3->setText(QChar(')'));
      ui->btnPeriod_3->setAccessibleName("0x3e");
      ui->btnPeriod_2->setText(QChar('('));
      ui->btnPeriod_2->setAccessibleName("0x3c");

    ui->btn_d->hide();
    ui->btn_ai->hide();
    ui->btn_v->hide();
    ui->btn_ie->hide();

    ui->btn_y->setText(QChar('u'));
    ui->btn_y->setAccessibleName("0x75");//75  u
    ui->btn_o->setText(QChar('i'));
    ui->btn_o->setAccessibleName("0x69");//69  i
        ui->btn_p->setText(QChar('q'));
        ui->btn_p->setAccessibleName("0x71");
        ui->btn_t->setText(QChar('w'));     //w 77
        ui->btn_t->setAccessibleName("0x77");
        ui->btn_g->setText(QChar('e'));
        ui->btn_g->setAccessibleName("0x65");
        ui->btn_j->setText(QChar('r'));
        ui->btn_j->setAccessibleName("0x72");
        ui->btn_ch->setText(QChar('t'));
        ui->btn_ch->setAccessibleName("0x74");
        ui->btn_z->setText(QChar('y'));      //79  y
        ui->btn_z->setAccessibleName("0x79");
        ui->btn_ei->setText(QChar('o'));
        ui->btn_ei->setAccessibleName("0x6f");
        ui->btn_en->setText(QChar('p'));
        ui->btn_en->setAccessibleName("0x70");

ui->btn_s->setText(QChar('-'));
ui->btn_s->setAccessibleName("0x2d");
   ui->btn_an->setText(QChar('.'));
   ui->btn_an->setAccessibleName("0x2e");
   ui->btn_m->move(37,40);
   ui->btn_n->move(82,40);
   ui->btn_k->move(127,40);
   ui->btn_q->move(172,40);
   ui->btn_sh->move(217,40);
   ui->btn_c->move(262,40);
   ui->btn_u->move(307,40);
   ui->btn_e->move(354,40);
   ui->btn_ao->move(400,40);
        ui->btn_m->setText(QChar('a'));
        ui->btn_m->setAccessibleName("0x61");
        ui->btn_n->setText(QChar('s'));
        ui->btn_n->setAccessibleName("0x73");
        ui->btn_k->setText(QChar('d'));
        ui->btn_k->setAccessibleName("0x64");
        ui->btn_q->setText(QChar('f'));
        ui->btn_q->setAccessibleName("0x66");
        ui->btn_sh->setText(QChar('g'));
        ui->btn_sh->setAccessibleName("0x67");
        ui->btn_c->setText(QChar('h'));
        ui->btn_c->setAccessibleName("0x68");
        ui->btn_u->setText(QChar('j'));
        ui->btn_u->setAccessibleName("0x6a");
        ui->btn_e->setText(QChar('k'));
        ui->btn_e->setAccessibleName("0x6b");
        ui->btn_ao->setText(QChar('l'));
        ui->btn_ao->setAccessibleName("0x6c");

        ui->btn_er->setText(QChar('z'));
        ui->btn_er->setAccessibleName("0x7a");
        ui->btn_f->setText(QChar('x'));
        ui->btn_f->setAccessibleName("0x78");
        ui->btn_l->setText(QChar('c'));
        ui->btn_l->setAccessibleName("0x63");
        ui->btn_h->setText(QChar('v'));
        ui->btn_h->setAccessibleName("0x76");
        ui->btn_x->setText(QChar('b'));
        ui->btn_x->setAccessibleName("0x62");
        ui->btn_r->setText(QChar('n'));
        ui->btn_r->setAccessibleName("0x6e");
        ui->btn_s->setText(QChar('m'));
        ui->btn_s->setAccessibleName("0x6d");



}

void Big5PinyinPanel::returnzhuyinenglish()
{
//    ui->btn1->setText(QChar('1'));
//    ui->btn1->setAccessibleName("0x31");
//    ui->btn2->setText(QChar('2'));
//    ui->btn2->setAccessibleName("0x32");
//    ui->btn3->setText(QChar('3'));
//    ui->btn3->setAccessibleName("0x33");
//    ui->btn4->setText(QChar('4'));
//    ui->btn4->setAccessibleName("0x34");
//    ui->btn5->setText(QChar('5'));
//    ui->btn5->setAccessibleName("0x35");
//    ui->btn6->setText(QChar('6'));
//    ui->btn6->setAccessibleName("0x36");
//    ui->btn7->setText(QChar('7'));
//    ui->btn7->setAccessibleName("0x37");
//    ui->btn8->setText(QChar('8'));
//    ui->btn8->setAccessibleName("0x38");
//    ui->btn9->setText(QChar('9'));
//    ui->btn9->setAccessibleName("0x39");
//    ui->btn0->setText(QChar('0'));
//    ui->btn0->setAccessibleName("0x30");

	if (!py->m_bEnglishMode)
	{
		//printf("lg 2\n");
        ui->btn_p->setText(QString::fromUtf8("ㄆ"));                  //注音:ㄆ所在的位置0x3106
        ui->btn_p->setAccessibleName("0x70");                       //unicode码
        ui->btn_t->setText(QString::fromUtf8("ㄊ"));
        ui->btn_t->setAccessibleName("0x74");
        ui->btn_g->setText(QString::fromUtf8("ㄍ"));
        ui->btn_g->setAccessibleName("0x67");
        ui->btn_j->setText(QString::fromUtf8("ㄐ"));
        ui->btn_j->setAccessibleName("0x6a");
        ui->btn_ch->setText(QString::fromUtf8("ㄔ"));
        ui->btn_ch->setAccessibleName("0x3114");
        ui->btn_z->setText(QString::fromUtf8("ㄗ"));
        ui->btn_z->setAccessibleName("0x7a");
        ui->btn_y->setText(QString::fromUtf8("ㄧ"));
        ui->btn_y->setAccessibleName("0x69");
//		ui->btn_b->setText(QString::fromUtf8("ㄅ"));
//		ui->btn_b->setAccessibleName("0x62");
//		ui->btn_d->setText(QString::fromUtf8("ㄉ"));
//		ui->btn_d->setAccessibleName("0x64");
//		ui->btn_zh->setText(QString::fromUtf8("ㄓ"));
//		ui->btn_zh->setAccessibleName("0x3113");
//		ui->btn_a->setText(QString::fromUtf8("ㄚ"));
//		ui->btn_a->setAccessibleName("0x61");
//		ui->btn_ai->setText(QString::fromUtf8("ㄞ"));
//		ui->btn_ai->setAccessibleName("0x311E");
//		ui->btn_an->setText(QString::fromUtf8("ㄢ"));
//		ui->btn_an->setAccessibleName("0x3122");
//		ui->btn_er->setText(QString::fromUtf8("ㄦ"));
//		ui->btn_er->setAccessibleName("0x3126");
	}
	else
	{
//        ui->btn_p->setText(QChar('!'));//b
//        ui->btn_p->setAccessibleName("0x21");
//        ui->btn_t->setText(QChar('?'));
//        ui->btn_t->setAccessibleName("0x3F");
//        ui->btn_g->setText(QChar(':'));
//        ui->btn_g->setAccessibleName("0x3A");
//        ui->btn_j->setText("'");
//        ui->btn_j->setAccessibleName("0x27");
//        ui->btn_ch->setText(QChar('"'));
//        ui->btn_ch->setAccessibleName("0x22");
//        ui->btn_z->setText(QChar('%'));
//        ui->btn_z->setAccessibleName("0x25");
//        ui->btn_y->setText(QChar('~'));
//        ui->btn_y->setAccessibleName("0x7E");
        ui->btn_p->setText(QChar('q'));
        ui->btn_p->setAccessibleName("0x71");
        ui->btn_t->setText(QChar('w'));
        ui->btn_t->setAccessibleName("0x77");
        ui->btn_g->setText(QChar('e'));
        ui->btn_g->setAccessibleName("0x65");
        ui->btn_j->setText(QChar('r'));
        ui->btn_j->setAccessibleName("0x72");
        ui->btn_ch->setText(QChar('t'));
        ui->btn_ch->setAccessibleName("0x74");
        ui->btn_z->setText(QChar('y'));
        ui->btn_z->setAccessibleName("0x79");
        ui->btn_y->setText(QChar('u'));
        ui->btn_y->setAccessibleName("0x75");
        ui->btn_o->setText(QChar('i'));
        ui->btn_o->setAccessibleName("0x69");
        ui->btn_ei->setText(QChar('o'));
        ui->btn_ei->setAccessibleName("0x6f");
        ui->btn_en->setText(QChar('p'));
        ui->btn_en->setAccessibleName("0x70");

        ui->btn_m->setText(QChar('a'));
        ui->btn_m->setAccessibleName("0x61");
        ui->btn_n->setText(QChar('s'));
        ui->btn_n->setAccessibleName("0x73");
        ui->btn_k->setText(QChar('d'));
        ui->btn_k->setAccessibleName("0x64");
        ui->btn_q->setText(QChar('f'));
        ui->btn_q->setAccessibleName("0x66");
        ui->btn_sh->setText(QChar('g'));
        ui->btn_sh->setAccessibleName("0x67");
        ui->btn_c->setText(QChar('h'));
        ui->btn_c->setAccessibleName("0x68");
        ui->btn_u->setText(QChar('j'));
        ui->btn_u->setAccessibleName("0x6a");
        ui->btn_e->setText(QChar('k'));
        ui->btn_e->setAccessibleName("0x6b");
        ui->btn_ao->setText(QChar('l'));
        ui->btn_ao->setAccessibleName("0x6c");
        ui->btn_ang->setText(QChar(';'));
        ui->btn_ang->setAccessibleName("0x3b");

        ui->btn_f->setText(QChar('z'));
        ui->btn_f->setAccessibleName("0x7a");
        ui->btn_l->setText(QChar('x'));
        ui->btn_l->setAccessibleName("0x78");
        ui->btn_h->setText(QChar('c'));
        ui->btn_h->setAccessibleName("0x63");
        ui->btn_x->setText(QChar('v'));
        ui->btn_x->setAccessibleName("0x76");
        ui->btn_r->setText(QChar('b'));
        ui->btn_r->setAccessibleName("0x62");
        ui->btn_s->setText(QChar('n'));
        ui->btn_s->setAccessibleName("0x6e");
        ui->btn_v->setText(QChar('m'));
	}


}

/*void Big5PinyinPanel::returnzhuyin()
{
    ui->btn1->setText(QChar('1'));
    ui->btn1->setAccessibleName("0x31");
    ui->btn2->setText(QChar('2'));
    ui->btn2->setAccessibleName("0x32");
    ui->btn3->setText(QChar('3'));
    ui->btn3->setAccessibleName("0x33");
    ui->btn4->setText(QChar('4'));
    ui->btn4->setAccessibleName("0x34");
    ui->btn5->setText(QChar('5'));
    ui->btn5->setAccessibleName("0x35");
    ui->btn6->setText(QChar('6'));
    ui->btn6->setAccessibleName("0x36");
    ui->btn7->setText(QChar('7'));
    ui->btn7->setAccessibleName("0x37");
    ui->btn8->setText(QChar('8'));
    ui->btn8->setAccessibleName("0x38");
    ui->btn9->setText(QChar('9'));
    ui->btn9->setAccessibleName("0x39");
    ui->btn0->setText(QChar('0'));
    ui->btn0->setAccessibleName("0x30");

	if (!py->m_bEnglishMode)
	{
		ui->btn_b->setText("ㄅ");
		ui->btn_b->setAccessibleName("0x62");
		ui->btn_d->setText("ㄉ");
		ui->btn_d->setAccessibleName("0x64");
		ui->btn_zh->setText("ㄓ");
		ui->btn_zh->setAccessibleName("0x3113");
		ui->btn_a->setText("ㄚ");
		ui->btn_a->setAccessibleName("0x61");
		ui->btn_ai->setText("ㄞ");
		ui->btn_ai->setAccessibleName("0x311E");
		ui->btn_an->setText("ㄢ");
		ui->btn_an->setAccessibleName("0x3122");
		ui->btn_er->setText("ㄦ");
		ui->btn_er->setAccessibleName("0x3126");
	}
	else
	{
		ui->btn_b->setText(QChar('!'));
		ui->btn_b->setAccessibleName("0x21");
		ui->btn_d->setText(QChar('?'));
		ui->btn_d->setAccessibleName("0x3F");
		ui->btn_zh->setText(QChar(':'));
		ui->btn_zh->setAccessibleName("0x3A");
		ui->btn_a->setText("'");
		ui->btn_a->setAccessibleName("0x27");
		ui->btn_ai->setText(QChar('"'));
		ui->btn_ai->setAccessibleName("0x22");
		ui->btn_an->setText(QChar('%'));
		ui->btn_an->setAccessibleName("0x25");
		ui->btn_er->setText(QChar('~'));
		ui->btn_er->setAccessibleName("0x7E");
	}
}*/

void Big5PinyinPanel::changeMark1()    //全角中文标点符号
{
//    ui->btn1->setText("{");
//    ui->btn1->setAccessibleName("0xff5b");
//    ui->btn2->setText("}");
//    ui->btn2->setAccessibleName("0xff5d");
//    ui->btn3->setText("[");
//    ui->btn3->setAccessibleName("0x3010");
//    ui->btn4->setText("]");
//    ui->btn4->setAccessibleName("0x3011");
//	ui->btn5->setText(QString::fromUtf8("“"));
//    ui->btn5->setAccessibleName("0xff62");
//	ui->btn6->setText(QString::fromUtf8("”"));
//    ui->btn6->setAccessibleName("0xff63");
//	ui->btn7->setText(QString::fromUtf8("（"));
//    ui->btn7->setAccessibleName("0xff08");
//	ui->btn8->setText(QString::fromUtf8("）"));
//    ui->btn8->setAccessibleName("0xff09");
//	ui->btn9->setText(QString::fromUtf8("："));
//    ui->btn9->setAccessibleName("0xff1a");
//	ui->btn0->setText(QString::fromUtf8("、"));
//    ui->btn0->setAccessibleName("0x3001");
//e1s
    ui->btn_p->setText(QChar('1'));
    ui->btn_p->setAccessibleName("0x31");
    ui->btn_t->setText(QChar('2'));
    ui->btn_t->setAccessibleName("0x32");
    ui->btn_g->setText(QChar('3'));
    ui->btn_g->setAccessibleName("0x33");
    ui->btn_j->setText(QChar('4'));
    ui->btn_j->setAccessibleName("0x34");
    ui->btn_ch->setText(QChar('5'));
    ui->btn_ch->setAccessibleName("0x35");
    ui->btn_z->setText(QChar('6'));
    ui->btn_z->setAccessibleName("0x36");
    ui->btn_y->setText(QChar('7'));
    ui->btn_y->setAccessibleName("0x37");
    ui->btn_o->setText(QChar('8'));
    ui->btn_o->setAccessibleName("0x38");
    ui->btn_ei->setText(QChar('9'));
    ui->btn_ei->setAccessibleName("0x39");
    ui->btn_en->setText(QChar('0'));
    ui->btn_en->setAccessibleName("0x30");

//    ui->btn_m->setText(QChar('a'));
//    ui->btn_m->setAccessibleName("0x61");
//    ui->btn_n->setText(QChar('s'));
//    ui->btn_n->setAccessibleName("0x73");
//    ui->btn_k->setText(QChar('d'));
//    ui->btn_k->setAccessibleName("0x64");
//    ui->btn_q->setText(QChar('f'));
//    ui->btn_q->setAccessibleName("0x66");
//    ui->btn_sh->setText(QChar('g'));
//    ui->btn_sh->setAccessibleName("0x67");
//    ui->btn_c->setText(QChar('h'));
//    ui->btn_c->setAccessibleName("0x68");
//    ui->btn_u->setText(QChar('j'));
//    ui->btn_u->setAccessibleName("0x6a");
//    ui->btn_e->setText(QChar('k'));
//    ui->btn_e->setAccessibleName("0x6b");
//    ui->btn_ao->setText(QChar('l'));
//    ui->btn_ao->setAccessibleName("0x6c");
//    ui->btn_ang->setText(QChar(';'));
//    ui->btn_ang->setAccessibleName("0x3b");

//    ui->btn_f->setText(QChar('z'));
//    ui->btn_f->setAccessibleName("0x7a");
//    ui->btn_l->setText(QChar('x'));
//    ui->btn_l->setAccessibleName("0x78");
//    ui->btn_h->setText(QChar('c'));
//    ui->btn_h->setAccessibleName("0x63");
//    ui->btn_x->setText(QChar('v'));
//    ui->btn_x->setAccessibleName("0x76");
//    ui->btn_r->setText(QChar('b'));
//    ui->btn_r->setAccessibleName("0x62");
//    ui->btn_s->setText(QChar('n'));
//    ui->btn_s->setAccessibleName("0x6e");
//    ui->btn_v->setText(QChar('m'));
//    ui->btn_v->setAccessibleName("0x6d");

    ui->btn_m->setText(QString::fromUtf8("，"));//d
    ui->btn_m->setAccessibleName("0xff0c");
    ui->btn_n->setText(QString::fromUtf8("；"));//b
    ui->btn_n->setAccessibleName("0xff1b");
    ui->btn_k->setText(QString::fromUtf8("。"));//zh
    ui->btn_k->setAccessibleName("0x3002");
    ui->btn_q->setText(QString::fromUtf8("？"));//a
    ui->btn_q->setAccessibleName("0xff1f");
    ui->btn_sh->setText(QString::fromUtf8("《"));//ai
    ui->btn_sh->setAccessibleName("0x300A");
    ui->btn_c->setText(QString::fromUtf8("》"));//an
    ui->btn_c->setAccessibleName("0x300B");
    ui->btn_u->setText(QString::fromUtf8("！"));//er
    ui->btn_u->setAccessibleName("0xff01");

    ui->btn_ie->setText(QString::fromUtf8("“"));
    ui->btn_ie->setAccessibleName("0x201C");
    ui->btn_e->setText(QString::fromUtf8("："));
    ui->btn_e->setAccessibleName("0xff1a");
    ui->btn_ao->setText(QString::fromUtf8("、"));
    ui->btn_ao->setAccessibleName("0x3001");

    ui->btn_s_2->setText(QChar('-'));
    ui->btn_s_2->setAccessibleName("0x2d");

}

void Big5PinyinPanel::changeMark2()    //半角－英文符号
{
    ui->btn_p->setText(QChar('1'));
    ui->btn_p->setAccessibleName("0x31");
    ui->btn_t->setText(QChar('2'));
    ui->btn_t->setAccessibleName("0x32");
    ui->btn_g->setText(QChar('3'));
    ui->btn_g->setAccessibleName("0x33");
    ui->btn_j->setText(QChar('4'));
    ui->btn_j->setAccessibleName("0x34");
    ui->btn_ch->setText(QChar('5'));
    ui->btn_ch->setAccessibleName("0x35");
    ui->btn_z->setText(QChar('6'));
    ui->btn_z->setAccessibleName("0x36");
    ui->btn_y->setText(QChar('7'));
    ui->btn_y->setAccessibleName("0x37");
    ui->btn_o->setText(QChar('8'));
    ui->btn_o->setAccessibleName("0x38");
    ui->btn_ei->setText(QChar('9'));
    ui->btn_ei->setAccessibleName("0x39");
    ui->btn_en->setText(QChar('0'));
    ui->btn_en->setAccessibleName("0x30");

    ui->btn_m->setText("{");
    ui->btn_m->setAccessibleName("0x7b");
    ui->btn_n->setText("}");
    ui->btn_n->setAccessibleName("0x7d");
    ui->btn_k->setText("^");
    ui->btn_k->setAccessibleName("0x5e");
    ui->btn_q->setText("[");
    ui->btn_q->setAccessibleName("0x5b");
    ui->btn_sh->setText("]");
    ui->btn_sh->setAccessibleName("0x5d");
    ui->btn_c->setText(QChar('"'));
    ui->btn_c->setAccessibleName("0x22");
    ui->btn_u->setText(QChar('"'));
    ui->btn_u->setAccessibleName("0x22");
    ui->btn_e->setText("(");
    ui->btn_e->setAccessibleName("0x28");
    ui->btn_ao->setText(")");
    ui->btn_ao->setAccessibleName("0x29");

//    ui->btn_er->setText(QChar('z'));
//    ui->btn_er->setAccessibleName("0x7a");
//    ui->btn_f->setText(QChar('x'));
//    ui->btn_f->setAccessibleName("0x78");
//    ui->btn_l->setText(QChar('c'));
//    ui->btn_l->setAccessibleName("0x63");
//    ui->btn_h->setText(QChar('v'));
//    ui->btn_h->setAccessibleName("0x76");
//    ui->btn_x->setText(QChar('b'));
//    ui->btn_x->setAccessibleName("0x62");
//    ui->btn_r->setText(QChar('n'));
//    ui->btn_r->setAccessibleName("0x6e");
//    ui->btn_s->setText(QChar('m'));
//    ui->btn_s->setAccessibleName("0x6d");
    //
    ui->btn_er->setText(QChar('!'));
    ui->btn_er->setAccessibleName("0x21");
    ui->btn_f->setText(QChar(':'));
    ui->btn_f->setAccessibleName("0x3A");
    ui->btn_l->setText("'");
    ui->btn_l->setAccessibleName("0x27");
    ui->btn_h->setText(QChar(';'));
    ui->btn_h->setAccessibleName("0x3b");
    ui->btn_x->setText(QChar('.'));
    ui->btn_x->setAccessibleName("0x2e");
    ui->btn_r->setText(QChar('/'));
    ui->btn_r->setAccessibleName("0x2f");
    ui->btn_s->setText(QChar('-'));
    ui->btn_s->setAccessibleName("0x2d");

//    ui->btn_p->setText(",");  //xiugai
//    ui->btn_p->setAccessibleName("0x2c");
//    ui->btn_h->setText(";");
//    ui->btn_h->setAccessibleName("0x3b");
//    ui->btn_g->setText(".");
//    ui->btn_g->setAccessibleName("0x2e");
//    ui->btn_j->setText("?");
//    ui->btn_j->setAccessibleName("0x3f");
//    ui->btn_ch->setText("<");
//    ui->btn_ch->setAccessibleName("0x3c");
//    ui->btn_z->setText(">");
//    ui->btn_z->setAccessibleName("0x3e");
//    ui->btn_y->setText("!");
//    ui->btn_y->setAccessibleName("0x21");
}

void Big5PinyinPanel::changeTextShift(bool isShift)    //英文模式下才起作用
{
    if (isShift) {
        ui->btn_p->setText(QChar('!'));
        ui->btn_p->setAccessibleName("0x21");
        ui->btn_t->setText(QChar('@'));
        ui->btn_t->setAccessibleName("0x40");
        ui->btn_g->setText(QChar('#'));
        ui->btn_g->setAccessibleName("0x23");
        ui->btn_j->setText(QChar('$'));
        ui->btn_j->setAccessibleName("0x24");
        ui->btn_ch->setText(QChar('%'));
        ui->btn_ch->setAccessibleName("0x25");
        ui->btn_z->setText(QChar('^'));
        ui->btn_z->setAccessibleName("0x5e");
        ui->btn_y->setText("&&");
        ui->btn_y->setAccessibleName("0x26");
        ui->btn_o->setText(QChar('*'));
        ui->btn_o->setAccessibleName("0x2a");
        ui->btn_en->setText(QChar(')'));
        ui->btn_en->setAccessibleName("0x28");
        ui->btn_ei->setText(QChar('('));
        ui->btn_ei->setAccessibleName("0x29");
    }
    else {
        ui->btn_p->setText(QChar('1'));
        ui->btn_p->setAccessibleName("0x31");
        ui->btn_t->setText(QChar('2'));
        ui->btn_t->setAccessibleName("0x32");
        ui->btn_g->setText(QChar('3'));
        ui->btn_g->setAccessibleName("0x33");
        ui->btn_j->setText(QChar('4'));
        ui->btn_j->setAccessibleName("0x34");
        ui->btn_ch->setText(QChar('5'));
        ui->btn_ch->setAccessibleName("0x35");
        ui->btn_z->setText(QChar('6'));
        ui->btn_z->setAccessibleName("0x36");
        ui->btn_y->setText(QChar('7'));
        ui->btn_y->setAccessibleName("0x37");
        ui->btn_o->setText(QChar('8'));
        ui->btn_o->setAccessibleName("0x38");
        ui->btn_ei->setText(QChar('9'));
        ui->btn_ei->setAccessibleName("0x39");
        ui->btn_en->setText(QChar('0'));
        ui->btn_en->setAccessibleName("0x30");


//        ui->btn1->setText(QChar('1'));
//        ui->btn1->setAccessibleName("0x31");
//        ui->btn2->setText(QChar('2'));
//        ui->btn2->setAccessibleName("0x32");
//        ui->btn3->setText(QChar('3'));
//        ui->btn3->setAccessibleName("0x33");
//        ui->btn4->setText(QChar('4'));
//        ui->btn4->setAccessibleName("0x34");
//        ui->btn5->setText(QChar('5'));
//        ui->btn5->setAccessibleName("0x35");
//        ui->btn6->setText(QChar('6'));
//        ui->btn6->setAccessibleName("0x36");
//        ui->btn7->setText(QChar('7'));
//        ui->btn7->setAccessibleName("0x37");
//        ui->btn8->setText(QChar('8'));
//        ui->btn8->setAccessibleName("0x38");
//        ui->btn9->setText(QChar('9'));
//        ui->btn9->setAccessibleName("0x39");
//        ui->btn0->setText(QChar('0'));
//        ui->btn0->setAccessibleName("0x30");

    }
}

void Big5PinyinPanel::changeTextCaps(bool isCaps)     //英文模式下才有作用
{
    if (isCaps)                                          //大写字母
    {
//        ui->btn_d->setText(QChar('?'));
//        ui->btn_d->setAccessibleName("0x3F");
//        ui->btn_b->setText(QChar('!'));
//        ui->btn_b->setAccessibleName("0x21");
//        ui->btn_zh->setText(QChar(':'));
//        ui->btn_zh->setAccessibleName("0x3A");
//        ui->btn_a->setText("'");
//        ui->btn_a->setAccessibleName("0x27");
//        ui->btn_ai->setText(QChar('"'));
//        ui->btn_ai->setAccessibleName("0x22");
//        ui->btn_an->setText(QChar('%'));
//        ui->btn_an->setAccessibleName("0x25");
//        ui->btn_er->setText(QChar('~'));
//        ui->btn_er->setAccessibleName("0x7E");

        ui->btn_p->setText(QChar('Q'));                  //注音:ㄆ所在的位置
        ui->btn_p->setAccessibleName("0x51");                       //unicode码
        ui->btn_t->setText(QChar('W'));                  // ㄊ
        ui->btn_t->setAccessibleName("0x57");
        ui->btn_g->setText(QChar('E'));                  // ㄍ
        ui->btn_g->setAccessibleName("0x45");
        ui->btn_j->setText(QChar('R'));                  // ㄐ
        ui->btn_j->setAccessibleName("0x52");
        ui->btn_ch->setText(QChar('T'));                 // ㄔ
        ui->btn_ch->setAccessibleName("0x54");
        ui->btn_z->setText(QChar('Y'));                  // ㄗ
        ui->btn_z->setAccessibleName("0x59");
        ui->btn_y->setText(QChar('U'));                  // ㄧ
        ui->btn_y->setAccessibleName("0x55");
        ui->btn_o->setText(QChar('I'));                  // ㄛ
        ui->btn_o->setAccessibleName("0x49");
        ui->btn_ei->setText(QChar('O'));                 // ㄟ
        ui->btn_ei->setAccessibleName("0x4f");
        ui->btn_en->setText(QChar('P'));                 // ㄣ
        ui->btn_en->setAccessibleName("0x50");

        ui->btn_m->setText(QChar('A'));                  // ㄇ
        ui->btn_m->setAccessibleName("0x41");
        ui->btn_n->setText(QChar('S'));                  // ㄋ
        ui->btn_n->setAccessibleName("0x53");
        ui->btn_k->setText(QChar('D'));                  // ㄎ
        ui->btn_k->setAccessibleName("0x44");
        ui->btn_q->setText(QChar('F'));                  // ㄑ
        ui->btn_q->setAccessibleName("0x46");
        ui->btn_sh->setText(QChar('G'));                 // ㄕ
        ui->btn_sh->setAccessibleName("0x47");
        ui->btn_c->setText(QChar('H'));                  // ㄘ
        ui->btn_c->setAccessibleName("0x48");
        ui->btn_u->setText(QChar('J'));                  // ㄨ
        ui->btn_u->setAccessibleName("0x4a");
        ui->btn_e->setText(QChar('K'));                  // ㄜ
        ui->btn_e->setAccessibleName("0x4b");
        ui->btn_ao->setText(QChar('L'));                 // ㄠ
        ui->btn_ao->setAccessibleName("0x4c");


        ui->btn_er->setText(QChar('Z'));                  // ㄈ
        ui->btn_er->setAccessibleName("0x5a");
        ui->btn_f->setText(QChar('X'));                  // ㄌ
        ui->btn_f->setAccessibleName("0x58");
        ui->btn_l->setText(QChar('C'));                  // ㄏ
        ui->btn_l->setAccessibleName("0x43");
        ui->btn_h->setText(QChar('V'));                  // ㄒ
        ui->btn_h->setAccessibleName("0x56");
        ui->btn_x->setText(QChar('B'));                  // ㄖ
        ui->btn_x->setAccessibleName("0x42");
        ui->btn_r->setText(QChar('N'));                  // ㄙ
        ui->btn_r->setAccessibleName("0x4e");
        ui->btn_s->setText(QChar('M'));                  // ㄩ
        ui->btn_s->setAccessibleName("0x4d");
    }
    else{

//        ui->btn_b->setText(QChar('!'));
//        ui->btn_b->setAccessibleName("0x21");
//        ui->btn_d->setText(QChar('?'));
//        ui->btn_d->setAccessibleName("0x3F");
//        ui->btn_zh->setText(QChar(':'));
//        ui->btn_zh->setAccessibleName("0x3A");
//        ui->btn_a->setText("'");
//        ui->btn_a->setAccessibleName("0x27");
//        ui->btn_ai->setText(QChar('"'));
//        ui->btn_ai->setAccessibleName("0x22");
//        ui->btn_an->setText(QChar('%'));
//        ui->btn_an->setAccessibleName("0x25");
//        ui->btn_er->setText(QChar('~'));
//        ui->btn_er->setAccessibleName("0x7E");
//英文按下
        ui->btn_p->setText(QChar('q'));
        ui->btn_p->setAccessibleName("0x71");
        ui->btn_t->setText(QChar('w'));
        ui->btn_t->setAccessibleName("0x77");
        ui->btn_g->setText(QChar('e'));
        ui->btn_g->setAccessibleName("0x65");
        ui->btn_j->setText(QChar('r'));
        ui->btn_j->setAccessibleName("0x72");
        ui->btn_ch->setText(QChar('t'));
        ui->btn_ch->setAccessibleName("0x74");
        ui->btn_z->setText(QChar('y'));
        ui->btn_z->setAccessibleName("0x79");
        ui->btn_y->setText(QChar('u'));
        ui->btn_y->setAccessibleName("0x75");
        ui->btn_o->setText(QChar('i'));
        ui->btn_o->setAccessibleName("0x69");
        ui->btn_ei->setText(QChar('o'));
        ui->btn_ei->setAccessibleName("0x6f");
        ui->btn_en->setText(QChar('p'));
        ui->btn_en->setAccessibleName("0x70");

        ui->btn_m->setText(QChar('a'));
        ui->btn_m->setAccessibleName("0x61");
        ui->btn_n->setText(QChar('s'));
        ui->btn_n->setAccessibleName("0x73");
        ui->btn_k->setText(QChar('d'));
        ui->btn_k->setAccessibleName("0x64");
        ui->btn_q->setText(QChar('f'));
        ui->btn_q->setAccessibleName("0x66");
        ui->btn_sh->setText(QChar('g'));
        ui->btn_sh->setAccessibleName("0x67");
        ui->btn_c->setText(QChar('h'));
        ui->btn_c->setAccessibleName("0x68");
        ui->btn_u->setText(QChar('j'));
        ui->btn_u->setAccessibleName("0x6a");
        ui->btn_e->setText(QChar('k'));
        ui->btn_e->setAccessibleName("0x6b");
        ui->btn_ao->setText(QChar('l'));
        ui->btn_ao->setAccessibleName("0x6c");

        ui->btn_er->setText(QChar('z'));
        ui->btn_er->setAccessibleName("0x7a");
        ui->btn_f->setText(QChar('x'));
        ui->btn_f->setAccessibleName("0x78");
        ui->btn_l->setText(QChar('c'));
        ui->btn_l->setAccessibleName("0x63");
        ui->btn_h->setText(QChar('v'));
        ui->btn_h->setAccessibleName("0x76");
        ui->btn_x->setText(QChar('b'));
        ui->btn_x->setAccessibleName("0x62");
        ui->btn_r->setText(QChar('n'));
        ui->btn_r->setAccessibleName("0x6e");
        ui->btn_s->setText(QChar('m'));
        ui->btn_s->setAccessibleName("0x6d");
    }
}

//void Big5PinyinPanel::changekeyboard(bool inputchange)
//{
//    if(!inputchange)           //注音键盘
//    {
//        showzhuyinkeyboard(true);
//    }

//    else if(inputchange)       //英式键盘
//    {
//        showEngishkeyboard(true);
//    }
//}


void Big5PinyinPanel::on_btn_caps_clicked()
{
	if (!py->m_bEnglishMode)
	{
        //ui->btn_caps->setChecked(false);
		changflage = 1;
		return;
	}
    ui->btn_caps->setChecked(true);
	if(changflage){
        btn_return_default_state(0);
		changeTextCaps(true);
        //ui->btn_caps->setChecked(true);
	}
	else// if(changflage%2==0)
    {
        changeTextCaps(false);
        //ui->btn_caps->setChecked(false);
	}

	changflage = !changflage;
}

void Big5PinyinPanel::btn_return_default_state(int ignore_id)
{
//	if ((ignore_id != 0) && ui->btn_caps->isChecked())
//	{
//		on_btn_caps_clicked();
//	}

//	if ((ignore_id != 1) && ui->btn_sym1->isChecked())
//	{
//		on_btn_sym1_clicked();
//	}

//	if ((ignore_id != 2) && ui->btn_sym2->isChecked())
//	{
//		on_btn_sym2_clicked();
//	}

//	if ((ignore_id != 3) && ui->btn_sym3->isChecked())
//	{
//		on_btn_sym3_clicked();
//	}

//	if ((ignore_id != 4) && ui->btn_sym4->isChecked())
//	{
//		on_btn_sym4_clicked();
//	}
}

void Big5PinyinPanel::on_btn_sym1_clicked()    //中文标点
{
	extern char str_zhuyin[50];

    //py->resetState();
    //str_zhuyin[0] = '\0';
    //py->repaint();

	if (changflage1) {
		btn_return_default_state(1);

        ui->btn_sym3->show();
        ui->btn_ou->hide();
        ui->btn_d->hide();
		changeMark1();
        //ui->btn_sym1->setChecked(true);
	}
	else
    {
        py->resetState();
        py->repaint();

        ui->btn_d->show();
        ui->btn_sym3->hide();
        ui->btn_ou->show();
        //returnzhuyinenglish(); zhu
        showzhuyinkeyboard();
        //ui->btn_sym1->setChecked(false);
	}


	changflage1 = !changflage1;
}

void Big5PinyinPanel::on_btn_sym2_clicked()  //英文标点
{
	extern char str_zhuyin[50];

	py->resetState();
	str_zhuyin[0] = '\0';
	py->repaint();

	if (changflage2) {
        ui->btn_caps->setEnabled(false);
        ui->btn_sym3->hide();
		btn_return_default_state(2);
		changeMark2();
        //ui->btn_sym2->setChecked(true);
	}
	else
	{
        ui->btn_caps->setEnabled(true);
        ui->btn_sym3->hide();
        ui->btn_caps->show();
        ui->btn_ou->hide();
        //returnzhuyinenglish();
        showEngishkeyboard();
        //ui->btn_sym2->setChecked(false);
	}

	changflage2 = !changflage2;
}

void Big5PinyinPanel::on_btn_sym3_clicked()   //+ -运算符
{
	extern char str_zhuyin[50];

	py->resetState();
	str_zhuyin[0] = '\0';
	py->repaint();

	if (changflage3){
		btn_return_default_state(3);

        ui->btn_p->setText("+");
        ui->btn_p->setAccessibleName("0x2b");
                ui->btn_t->setText(QString::fromUtf8("-"));
        ui->btn_t->setAccessibleName("0x2d");
        ui->btn_g->setText(QString::fromUtf8("×"));
        ui->btn_g->setAccessibleName("0xd7");
        ui->btn_j->setText(QString::fromUtf8("÷"));
        ui->btn_j->setAccessibleName("0xf7");
        ui->btn_ch->setText("=");
        ui->btn_ch->setAccessibleName("0x3d");
        ui->btn_z->setText(QString::fromUtf8("…"));
        ui->btn_z->setAccessibleName("0x2026");
        ui->btn_y->setText(QString::fromUtf8("±"));
        ui->btn_y->setAccessibleName("0xb1");
        ui->btn_o->setText("/");
        ui->btn_o->setAccessibleName("0x2f");
        ui->btn_ei->setText("\\");
        ui->btn_ei->setAccessibleName("0x5c");
        ui->btn_en->setText("_");
        ui->btn_en->setAccessibleName("0x5f");

        //ui->btn_sym3->setChecked(true);
	}
	else// if(changflage3%2==0)
    {
        ui->btn_p->setText(QChar('1'));
        ui->btn_p->setAccessibleName("0x31");
        ui->btn_t->setText(QChar('2'));
        ui->btn_t->setAccessibleName("0x32");
        ui->btn_g->setText(QChar('3'));
        ui->btn_g->setAccessibleName("0x33");
        ui->btn_j->setText(QChar('4'));
        ui->btn_j->setAccessibleName("0x34");
        ui->btn_ch->setText(QChar('5'));
        ui->btn_ch->setAccessibleName("0x35");
        ui->btn_z->setText(QChar('6'));
        ui->btn_z->setAccessibleName("0x36");
        ui->btn_y->setText(QChar('7'));
        ui->btn_y->setAccessibleName("0x37");
        ui->btn_o->setText(QChar('8'));
        ui->btn_o->setAccessibleName("0x38");
        ui->btn_ei->setText(QChar('9'));
        ui->btn_ei->setAccessibleName("0x39");
        ui->btn_en->setText(QChar('0'));
        ui->btn_en->setAccessibleName("0x30");

        //ui->btn_sym3->setChecked(false);
    }

	changflage3 = !changflage3;
}

void Big5PinyinPanel::on_btn_sym4_clicked()
{
	extern char str_zhuyin[50];

	py->resetState();
	str_zhuyin[0] = '\0';
	py->repaint();

	if (changflage4) {
		btn_return_default_state(4);
		changeTextShift(true);
        //ui->btn_sym4->setChecked(true);
	}
	else// if (changflage4%2==0)
    {
        changeTextShift(false);
        //ui->btn_sym4->setChecked(false);
    }

    changflage4 = !changflage4;
}

//切换到英文
void Big5PinyinPanel::on_btnchangeEnglish_clicked()
{
    ui->btn_caps->setEnabled(true);

    ui->btn_caps->show();
    ui->btn_sym3->hide();

    py->m_bEnglishMode = true;
    ui->btnchangeEnglish->hide();
    ui->btnchangeChinese->show();
    py->resetState();
    str_zhuyin[0] = '\0';
//changeTextCaps(false);
//    ui->btn_caps->setChecked(false);
//    btn_return_default_state(-1);
    showEngishkeyboard();
    py->update();
    ui->btnchangeEnglish->setChecked(true);
}


//切换到繁体中文
void Big5PinyinPanel::on_btnchangeChinese_clicked()
{
    ui->btn_caps->hide();
    ui->btn_sym3->hide();


    py->m_bEnglishMode = false;
    ui->btnchangeEnglish->show();
    ui->btnchangeChinese->hide();
    py->resetState();
    py->repaint();
//    changeTextCaps(false);
//    ui->btn_caps->setChecked(false);
//    btn_return_default_state(-1);
    showzhuyinkeyboard();      
    py->update();
    ui->btn_ou->show();
    ui->btnchangeChinese->setChecked(true);
}


void Big5PinyinPanel::on_toolButtonPrevPage_clicked()
{
    py->prev_page();
}

void Big5PinyinPanel::on_toolButtonNextPage_clicked()
{
    py->next_page();
}



//*********************************************
//**  modified by wangdongxing at 2012.6.5   **
//**  to accelarate the speed of reaction    **
//*********************************************
void Big5PinyinPanel::on_btn_hide_pressed()
{
    on_btn_hide_clicked();
}
void Big5PinyinPanel::on_btn_delete_pressed()
{
    on_btn_delete_clicked();
}
void Big5PinyinPanel::on_btn_enter_pressed()
{
    on_btn_enter_clicked();
}
void Big5PinyinPanel::on_btn_sym1_pressed()
{
    //on_btn_sym1_clicked();
}
void Big5PinyinPanel::on_btn_sym2_pressed()
{
    //on_btn_sym2_clicked();
}
void Big5PinyinPanel::on_btn_sym3_pressed()
{
    on_btn_sym3_clicked();
}
void Big5PinyinPanel::on_btn_sym4_pressed()
{
    on_btn_sym4_clicked();
}
void Big5PinyinPanel::on_btnchangeEnglish_pressed()
{
    on_btnchangeEnglish_clicked();
}
void Big5PinyinPanel::on_toolButtonPrevPage_pressed()
{
    on_toolButtonPrevPage_clicked();
}
void Big5PinyinPanel::on_toolButtonNextPage_pressed()
{
    on_toolButtonNextPage_clicked();
}
void Big5PinyinPanel::on_btn_caps_pressed()
{
    on_btn_caps_clicked();
}
void Big5PinyinPanel::on_btnchangeChinese_pressed()
{
    on_btnchangeChinese_clicked();
}
//*********************************************
//**    End to modify at 2012.6.5            **
//**                                         **
//*********************************************

void Big5PinyinPanel::on_toolbiaodian_pressed()  //zengjia 20131107
{
    if(py->m_bEnglishMode)
    {
        on_btn_sym2_clicked();
    }
    else
    {
        on_btn_sym1_clicked();
    }
}
