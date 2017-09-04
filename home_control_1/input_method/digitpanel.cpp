#include "digitpanel.h"
#include <QtCore/QSignalMapper>
#include "ui_digitpanel.h"
#include "maindialog.h"
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;

static int changflagjianti=1;   //dx
static int changflagjianti1=1; //zh
static int changflagjianti2=1; //eni
static int changflagjianti3=1;  //+- zh
static int changflagjianti4=1;  //=# en

extern MainDialog *g_pMainDlg;

Digitpanel::Digitpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Digitpanel),
    charCount(25),isEn(false),m_isDrag(false)
{
	ui->setupUi(this);

    int res = g_pScaleSrc->get_screen_version();

    QFont digitfont("arial", 30);

    //设置DigitBtnFrame控件下的按键字体大小 //lidh 20150110
    QObjectList q = ui->DigitBtnFrame->children();

    for (int i = q.length() - 1; i >= 0; i-- )
    {
        QObject* o = q.at(i);

        if(o->inherits(("QToolButton")))
        {
            QToolButton* d = qobject_cast<QToolButton*>(o);
            d->setFont(digitfont);

            if (res == 1)  // 800x480
            {
                d->setStyleSheet(QString::fromUtf8("font: 30px;"));
                ui->toolbiaodian->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btn_space->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btn_sym1->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btn_sym2->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btn_sym3->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btn_sym4->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btnPeriod->setStyleSheet(QString::fromUtf8("font: 25px;"));
                ui->btncomma->setStyleSheet(QString::fromUtf8("font: 25px;"));
            }
            else if (res == 2) //1024x600
            {
                d->setStyleSheet(QString::fromUtf8("font: 38px;"));
                ui->toolbiaodian->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btn_space->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btn_sym1->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btn_sym2->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btn_sym3->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btn_sym4->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btnPeriod->setStyleSheet(QString::fromUtf8("font: 31px;"));
                ui->btncomma->setStyleSheet(QString::fromUtf8("font: 31px;"));
            }
            else if (res == 3) //1280x800
            {
                d->setStyleSheet(QString::fromUtf8("font: 50px;"));
                ui->toolbiaodian->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btn_space->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btn_sym1->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btn_sym2->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btn_sym3->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btn_sym4->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btnPeriod->setStyleSheet(QString::fromUtf8("font: 41px;"));
                ui->btncomma->setStyleSheet(QString::fromUtf8("font: 41px;"));
            }
        }
    }

	if ( res == 2 )  // 1024x600 
	{
		ui->DigitBtnFrame->setGeometry(QRect(0, 105, 1024, 331));
		ui->btnS->setGeometry(QRect(170, 80, 81, 76));
		ui->btnB->setGeometry(QRect(570, 160, 81, 76));
		ui->btnH->setGeometry(QRect(570, 80, 81, 76));
		ui->btnJ->setGeometry(QRect(670, 80, 81, 76));
		ui->btnD->setGeometry(QRect(270, 80, 81, 76));
		ui->btnZ->setGeometry(QRect(160, 160, 81, 76));
		ui->btnX->setGeometry(QRect(270, 160, 81, 76));
		ui->btnF->setGeometry(QRect(370, 80, 81, 76));
		ui->btnM->setGeometry(QRect(770, 160, 81, 76));
		ui->btn_space->setGeometry(QRect(265, 240, 500, 76));
		ui->btnK->setGeometry(QRect(770, 80, 81, 76));
		ui->btnL->setGeometry(QRect(870, 80, 81, 76));
		ui->btnV->setGeometry(QRect(470, 160, 81, 76));
		ui->btnC->setGeometry(QRect(370, 160, 81, 76));
		ui->btnN->setGeometry(QRect(670, 160, 81, 76));
		ui->btnA->setGeometry(QRect(70, 80, 81, 76));
		ui->btnG->setGeometry(QRect(470, 80, 81, 76));
		ui->btnE->setGeometry(QRect(230, 0, 81, 76));
		ui->btnO->setGeometry(QRect(830, 0, 81, 76));
		ui->btnW->setGeometry(QRect(130, 0, 81, 76));
		ui->btnP->setGeometry(QRect(930, 0, 81, 76));
		ui->btnI->setGeometry(QRect(730, 0, 81, 76));
		ui->btnR->setGeometry(QRect(330, 0, 81, 76));
		ui->btnT->setGeometry(QRect(430, 0, 81, 76));
		ui->btnY->setGeometry(QRect(530, 0, 81, 76));
		ui->btnU->setGeometry(QRect(630, 0, 81, 76));
		ui->btnQ->setGeometry(QRect(30, 0, 81, 76));
		ui->btnCaps->setGeometry(QRect(21, 160, 104, 76));
		ui->btn_sym3->setGeometry(QRect(20, 160, 104, 76));
		ui->btn_sym4->setGeometry(QRect(20, 160, 104, 76));
		ui->btnBackspace->setGeometry(QRect(910, 160, 104, 76));
		ui->btn_sym2->setGeometry(QRect(20, 240, 104, 76));
		ui->btnchangeENGLISH->setGeometry(QRect(140, 240, 104, 76));
		ui->btnchangeCHINESE->setGeometry(QRect(140, 240, 104, 76));
		ui->btn_sym1->setGeometry(QRect(21, 240, 104, 76));
		ui->toolbiaodian->setGeometry(QRect(21, 240, 104, 76));
		ui->btncomma->setGeometry(QRect(790, 240, 104, 76));
		ui->btnPeriod->setGeometry(QRect(790, 240, 104, 76));
		ui->btnEnter->setGeometry(QRect(910, 240, 104, 76));
		ui->labelCHINESE->setGeometry(QRect(11, 6, 28, 28));
		ui->labelENGLISH->setGeometry(QRect(11, 6, 28, 28));
		ui->DigitPYFrame->setGeometry(QRect(0, -1, 1024, 101));
		ui->DigitChineseFrame->setGeometry(QRect(0, 40, 824, 61));
		ui->DigitJianFrame->setGeometry(QRect(0, 0, 1024, 41));
    }
	else if ( res == 3 )  // 1280x800
	{
		ui->DigitBtnFrame->setGeometry(QRect(0, 149, 1280, 421));
		ui->btnS->setGeometry(QRect(200, 110, 101, 101));
		ui->btnB->setGeometry(QRect(720, 215, 101, 101));
		ui->btnH->setGeometry(QRect(720, 110, 101, 101));
		ui->btnJ->setGeometry(QRect(850, 110, 101, 101));
		ui->btnD->setGeometry(QRect(330, 110, 101, 101));
		ui->btnZ->setGeometry(QRect(200, 215, 101, 101));
		ui->btnX->setGeometry(QRect(330, 215, 101, 101));
		ui->btnF->setGeometry(QRect(460, 110, 101, 101));
		ui->btnM->setGeometry(QRect(980, 215, 101, 101));
		ui->btn_space->setGeometry(QRect(330, 320, 624, 101));
		ui->btnK->setGeometry(QRect(980, 110, 101, 101));
		ui->btnL->setGeometry(QRect(1110, 110, 101, 101));
		ui->btnV->setGeometry(QRect(590, 215, 101, 101));
		ui->btnC->setGeometry(QRect(460, 215, 101, 101));
		ui->btnN->setGeometry(QRect(850, 215, 101, 101));
		ui->btnA->setGeometry(QRect(70, 110, 101, 101));
		ui->btnG->setGeometry(QRect(590, 110, 101, 101));
		ui->btnE->setGeometry(QRect(270, 5, 101, 101));
		ui->btnW->setGeometry(QRect(140, 5, 101, 101));
		ui->btnP->setGeometry(QRect(1170, 5, 101, 101));
		ui->btnO->setGeometry(QRect(1040, 5, 101, 101));
		ui->btnI->setGeometry(QRect(920, 5, 101, 101));
		ui->btnR->setGeometry(QRect(400, 5, 101, 101));
		ui->btnT->setGeometry(QRect(530, 5, 101, 101));
		ui->btnY->setGeometry(QRect(660, 5, 101, 101));
		ui->btnU->setGeometry(QRect(790, 5, 101, 101));
		ui->btnQ->setGeometry(QRect(10, 5, 101, 101));
		ui->btnCaps->setGeometry(QRect(10, 215, 129, 101));
		ui->btn_sym3->setGeometry(QRect(10, 215, 129, 101));
		ui->btn_sym4->setGeometry(QRect(10, 215, 129, 101));
		ui->btnBackspace->setGeometry(QRect(1140, 215, 129, 101));
		ui->btn_sym2->setGeometry(QRect(10, 320, 129, 101));
		ui->btnchangeENGLISH->setGeometry(QRect(160, 320, 129, 101));
		ui->btnchangeCHINESE->setGeometry(QRect(160, 320, 129, 101));
		ui->btn_sym1->setGeometry(QRect(10, 320, 129, 101));
		ui->toolbiaodian->setGeometry(QRect(10, 320, 129, 101));
		ui->btncomma->setGeometry(QRect(990, 320, 129, 101));
		ui->btnPeriod->setGeometry(QRect(990, 320, 129, 101));
		ui->btnEnter->setGeometry(QRect(1140, 320, 129, 101));
		ui->labelCHINESE->setGeometry(QRect(18, 8, 45, 45));
		ui->labelENGLISH->setGeometry(QRect(18, 8, 45, 45));
		ui->DigitPYFrame->setGeometry(QRect(0, 0, 1281, 151));
		ui->DigitChineseFrame->setGeometry(QRect(0, 58, 1040, 101));
		ui->DigitJianFrame->setGeometry(QRect(0, 0, 1280, 51));
    }

//    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    //20131105 zj
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);
    //end

    py = new QPinyinFrame(this);                              //拼音输入对象,作为display()槽函数的信号
    py->setMinimumSize(py->sizeHint().width(), py->sizeHint().height());
    py->resize(py->sizeHint().width(), py->sizeHint().height());

    isCaps = false;
    changeTextCaps(false);
    ui->btnchangeCHINESE->hide();
    signalMapper = new QSignalMapper(this);   //信号映射器
    allButtons = findChildren<QToolButton *>();   //findChildren函数:
    for (int i=0;i<allButtons.count();i++)        //获取所有在主窗口的QToolButton
    {                                        //根据按键编号逐个查找
        connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(allButtons.at(i), i);  //信号逐次扫描
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT( doOperation(int)));

    m_inputMethodStatus = 0;
	m_type = 0;
    //20131106 zwb:zj
    ui->btnPeriod->hide();
    ui->btn_sym1->hide();
    ui->btn_sym2->hide();
}

Digitpanel::~Digitpanel()
{
	delete py;
	delete ui;
	delete signalMapper;
}


void Digitpanel::setCharCount(int count)
{
    charCount = count;
}

void Digitpanel::setEchodMode(QLineEdit::EchoMode echmode)
{
    //ui->display->setEchoMode(echmode);
}

void Digitpanel::doOperation(int btn)//键盘按钮处理
{
    emit updateTime();       //emit 信号:调用void updateTime()函数
    QString strKeyId;
    strKeyId = allButtons.at(btn)->accessibleName();         //获取键值的ID号
    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16); //QString::toInt函数:将字符串(按键ID)转为16进制码

    if (keyId == Qt::Key_Escape || keyId == Qt::Key_Shift || keyId == Qt::Key_CapsLock || keyId == Qt::Key_Tab)
        return;
    py->filter(keyId);    //拼音键值处理,true or false
}

void Digitpanel::changeMarkjianti1()     //中文－全角标点  修改20131105 zwb
{
    ui->btnQ->setText(QChar('1'));
    ui->btnQ->setAccessibleName("0x31");
    ui->btnW->setText(QChar('2'));
    ui->btnW->setAccessibleName("0x32");
    ui->btnE->setText(QChar('3'));
    ui->btnE->setAccessibleName("0x33");
    ui->btnR->setText(QChar('4'));
    ui->btnR->setAccessibleName("0x34");
    ui->btnT->setText(QChar('5'));
    ui->btnT->setAccessibleName("0x35");
    ui->btnY->setText(QChar('6'));
    ui->btnY->setAccessibleName("0x36");
    ui->btnU->setText(QChar('7'));
    ui->btnU->setAccessibleName("0x37");
    ui->btnI->setText(QChar('8'));
    ui->btnI->setAccessibleName("0x38");
    ui->btnO->setText(QChar('9'));
    ui->btnO->setAccessibleName("0x39");
    ui->btnP->setText(QChar('0'));
    ui->btnP->setAccessibleName("0x30");

    ui->btnA->setText("{");
    ui->btnA->setAccessibleName("0xff5b");
    ui->btnS->setText("}");
    ui->btnS->setAccessibleName("0xff5d");
    ui->btnD->setText("[");
    ui->btnD->setAccessibleName("0x3010");
    ui->btnF->setText("]");
    ui->btnF->setAccessibleName("0x3011");
    ui->btnG->setText(QString::fromUtf8("“"));
    ui->btnG->setAccessibleName("0x201C");
    ui->btnH->setText(QString::fromUtf8("（"));
    ui->btnH->setAccessibleName("0xff08");
    ui->btnJ->setText(QString::fromUtf8("）"));
    ui->btnJ->setAccessibleName("0xff09");
    ui->btnK->setText(QString::fromUtf8("："));
    ui->btnK->setAccessibleName("0xff1a");
    ui->btnL->setText(QString::fromUtf8("、"));
    ui->btnL->setAccessibleName("0x3001");
    ui->btnX->setText(QString::fromUtf8("，"));
    ui->btnX->setAccessibleName("0xff0c");
    ui->btnC->setText(QString::fromUtf8("；"));
    ui->btnC->setAccessibleName("0xff1b");
    ui->btnV->setText(QString::fromUtf8("。"));
    ui->btnV->setAccessibleName("0x3002");
    ui->btnB->setText(QString::fromUtf8("？"));
    ui->btnB->setAccessibleName("0xff1f");
    ui->btnN->setText(QString::fromUtf8("《"));
    ui->btnN->setAccessibleName("0x300A");
    ui->btnM->setText(QString::fromUtf8("》"));
    ui->btnM->setAccessibleName("0x300B");
    ui->btnZ->setText(QString::fromUtf8("！"));
    ui->btnZ->setAccessibleName("0xff01");
}

void Digitpanel::changeMarkjianti2()        //英文－半角标点
{
    ui->btnQ->setText(QChar('1'));
    ui->btnQ->setAccessibleName("0x31");
    ui->btnW->setText(QChar('2'));
    ui->btnW->setAccessibleName("0x32");
    ui->btnE->setText(QChar('3'));
    ui->btnE->setAccessibleName("0x33");
    ui->btnR->setText(QChar('4'));
    ui->btnR->setAccessibleName("0x34");
    ui->btnT->setText(QChar('5'));
    ui->btnT->setAccessibleName("0x35");
    ui->btnY->setText(QChar('6'));
    ui->btnY->setAccessibleName("0x36");
    ui->btnU->setText(QChar('7'));
    ui->btnU->setAccessibleName("0x37");
    ui->btnI->setText(QChar('8'));
    ui->btnI->setAccessibleName("0x38");
    ui->btnO->setText(QChar('9'));
    ui->btnO->setAccessibleName("0x39");
    ui->btnP->setText(QChar('0'));
    ui->btnP->setAccessibleName("0x30");

    ui->btnA->setText("{");
    ui->btnA->setAccessibleName("0x7b");
    ui->btnS->setText("}");
    ui->btnS->setAccessibleName("0x7d");
    ui->btnD->setText("[");
    ui->btnD->setAccessibleName("0x5b");
    ui->btnF->setText("]");
    ui->btnF->setAccessibleName("0x5d");
    ui->btnG->setText(QChar('"'));
    ui->btnG->setAccessibleName("0x22");
    ui->btnH->setText(QChar('"'));
    ui->btnH->setAccessibleName("0x22");
//    ui->btnH->setText("-");
//    ui->btnH->setAccessibleName("0x2d");//28
    ui->btnJ->setText("/");
    ui->btnJ->setAccessibleName("0x2f");//29
    ui->btnK->setText(":");
    ui->btnK->setAccessibleName("0x3a");
    ui->btnL->setText("'");
    ui->btnL->setAccessibleName("0x27");

    ui->btnX->setText(",");
    ui->btnX->setAccessibleName("0x2c");
    ui->btnC->setText(";");
    ui->btnC->setAccessibleName("0x3b");
    ui->btnV->setText(".");
    ui->btnV->setAccessibleName("0x2e");
    ui->btnB->setText("?");
    ui->btnB->setAccessibleName("0x3f");
    ui->btnN->setText("<");
    ui->btnN->setAccessibleName("0x3c");
    ui->btnM->setText(">");
    ui->btnM->setAccessibleName("0x3e");
    ui->btnZ->setText("!");
    ui->btnZ->setAccessibleName("0x21");
}

void Digitpanel::changeTextCaps(bool isCaps)
{
    if(!ui->btnCaps->isVisible())
    {
        ui->btnCaps->show();
        ui->btn_sym3->hide();
        ui->btn_sym4->hide();
    }
    if (isCaps)                                          //大写字母
    {
        ui->btnQ->setText(QChar('Q'));
        ui->btnQ->setAccessibleName("0x51");            //ASCII码
        ui->btnW->setText(QChar('W'));
        ui->btnW->setAccessibleName("0x57");
        ui->btnE->setText(QChar('E'));
        ui->btnE->setAccessibleName("0x45");
        ui->btnR->setText(QChar('R'));
        ui->btnR->setAccessibleName("0x52");
        ui->btnT->setText(QChar('T'));
        ui->btnT->setAccessibleName("0x54");
        ui->btnY->setText(QChar('Y'));
        ui->btnY->setAccessibleName("0x59");
        ui->btnU->setText(QChar('U'));
        ui->btnU->setAccessibleName("0x55");
        ui->btnI->setText(QChar('I'));
        ui->btnI->setAccessibleName("0x49");
        ui->btnO->setText(QChar('O'));
        ui->btnO->setAccessibleName("0x4f");
        ui->btnP->setText(QChar('P'));
        ui->btnP->setAccessibleName("0x50");

        ui->btnA->setText(QChar('A'));
        ui->btnA->setAccessibleName("0x41");
        ui->btnS->setText(QChar('S'));
        ui->btnS->setAccessibleName("0x53");
        ui->btnD->setText(QChar('D'));
        ui->btnD->setAccessibleName("0x44");
        ui->btnF->setText(QChar('F'));
        ui->btnF->setAccessibleName("0x46");
        ui->btnG->setText(QChar('G'));
        ui->btnG->setAccessibleName("0x47");
        ui->btnH->setText(QChar('H'));
        ui->btnH->setAccessibleName("0x48");
        ui->btnJ->setText(QChar('J'));
        ui->btnJ->setAccessibleName("0x4a");
        ui->btnK->setText(QChar('K'));
        ui->btnK->setAccessibleName("0x4b");
        ui->btnL->setText(QChar('L'));
        ui->btnL->setAccessibleName("0x4c");

        ui->btnZ->setText(QChar('Z'));
        ui->btnZ->setAccessibleName("0x5a");
        ui->btnX->setText(QChar('X'));
        ui->btnX->setAccessibleName("0x58");
        ui->btnC->setText(QChar('C'));
        ui->btnC->setAccessibleName("0x43");
        ui->btnV->setText(QChar('V'));
        ui->btnV->setAccessibleName("0x56");
        ui->btnB->setText(QChar('B'));
        ui->btnB->setAccessibleName("0x42");
        ui->btnN->setText(QChar('N'));
        ui->btnN->setAccessibleName("0x4e");
        ui->btnM->setText(QChar('M'));
        ui->btnM->setAccessibleName("0x4d");
        ui->btncomma->setText(QChar('-'));
        ui->btncomma->setAccessibleName("0x2d");
        ui->btnPeriod->setText(QChar('.'));
        ui->btnPeriod->setAccessibleName("0x2e");
    }
    else                                               //小写字母
    {
        ui->btnQ->setText(QChar('q'));
        ui->btnQ->setAccessibleName("0x71");
        ui->btnW->setText(QChar('w'));
        ui->btnW->setAccessibleName("0x77");
        ui->btnE->setText(QChar('e'));
        ui->btnE->setAccessibleName("0x65");
        ui->btnR->setText(QChar('r'));
        ui->btnR->setAccessibleName("0x72");
        ui->btnT->setText(QChar('t'));
        ui->btnT->setAccessibleName("0x74");
        ui->btnY->setText(QChar('y'));
        ui->btnY->setAccessibleName("0x79");
        ui->btnU->setText(QChar('u'));
        ui->btnU->setAccessibleName("0x75");
        ui->btnI->setText(QChar('i'));
        ui->btnI->setAccessibleName("0x69");
        ui->btnO->setText(QChar('o'));
        ui->btnO->setAccessibleName("0x6f");
        ui->btnP->setText(QChar('p'));
        ui->btnP->setAccessibleName("0x70");

        ui->btnA->setText(QChar('a'));
        ui->btnA->setAccessibleName("0x61");
        ui->btnS->setText(QChar('s'));
        ui->btnS->setAccessibleName("0x73");
        ui->btnD->setText(QChar('d'));
        ui->btnD->setAccessibleName("0x64");
        ui->btnF->setText(QChar('f'));
        ui->btnF->setAccessibleName("0x66");
        ui->btnG->setText(QChar('g'));
        ui->btnG->setAccessibleName("0x67");
        ui->btnH->setText(QChar('h'));
        ui->btnH->setAccessibleName("0x68");
        ui->btnJ->setText(QChar('j'));
        ui->btnJ->setAccessibleName("0x6a");
        ui->btnK->setText(QChar('k'));
        ui->btnK->setAccessibleName("0x6b");
        ui->btnL->setText(QChar('l'));
        ui->btnL->setAccessibleName("0x6c");

        ui->btnZ->setText(QChar('z'));
        ui->btnZ->setAccessibleName("0x7a");
        ui->btnX->setText(QChar('x'));
        ui->btnX->setAccessibleName("0x78");
        ui->btnC->setText(QChar('c'));
        ui->btnC->setAccessibleName("0x63");
        ui->btnV->setText(QChar('v'));
        ui->btnV->setAccessibleName("0x76");
        ui->btnB->setText(QChar('b'));
        ui->btnB->setAccessibleName("0x62");
        ui->btnN->setText(QChar('n'));
        ui->btnN->setAccessibleName("0x6e");
        ui->btnM->setText(QChar('m'));
        ui->btnM->setAccessibleName("0x6d");
        ui->btncomma->setText(QChar('-')); //,gai
        ui->btncomma->setAccessibleName("0x2d");
        ui->btnPeriod->setText(QChar('.'));
        ui->btnPeriod->setAccessibleName("0x2e");
    }
}

void Digitpanel::changeTextShift(bool isShift)
{
    if (isShift) {                                                     //shift键选项
        ui->btnQ->setText(QChar('!'));
        ui->btnQ->setAccessibleName("0x21");
        ui->btnW->setText(QChar('@'));
        ui->btnW->setAccessibleName("0x40");
        ui->btnE->setText(QChar('#'));
        ui->btnE->setAccessibleName("0x23");
        ui->btnR->setText(QChar('$'));
        ui->btnR->setAccessibleName("0x24");
        ui->btnT->setText(QChar('%'));
        ui->btnT->setAccessibleName("0x25");
        ui->btnY->setText(QChar('^'));
        ui->btnY->setAccessibleName("0x5e");
        ui->btnU->setText("&&");
        ui->btnU->setAccessibleName("0x26");
        ui->btnI->setText(QChar('*'));
        ui->btnI->setAccessibleName("0x2a");
        ui->btnO->setText(QChar('('));
        ui->btnO->setAccessibleName("0x28");
        ui->btnP->setText(QChar(')'));
        ui->btnP->setAccessibleName("0x29");
        }
    else {
        ui->btnQ->setText(QChar('1'));
        ui->btnQ->setAccessibleName("0x31");                 //ASCII码十六进制表示
        ui->btnW->setText(QChar('2'));
        ui->btnW->setAccessibleName("0x32");
        ui->btnE->setText(QChar('3'));
        ui->btnE->setAccessibleName("0x33");
        ui->btnR->setText(QChar('4'));
        ui->btnR->setAccessibleName("0x34");
        ui->btnT->setText(QChar('5'));
        ui->btnT->setAccessibleName("0x35");
        ui->btnY->setText(QChar('6'));
        ui->btnY->setAccessibleName("0x36");
        ui->btnU->setText(QChar('7'));
        ui->btnU->setAccessibleName("0x37");
        ui->btnI->setText(QChar('8'));
        ui->btnI->setAccessibleName("0x38");
        ui->btnO->setText(QChar('9'));
        ui->btnO->setAccessibleName("0x39");
        ui->btnP->setText(QChar('0'));
        ui->btnP->setAccessibleName("0x30");
    }
}

void Digitpanel::on_btnCaps_clicked()      //大写－小写切换
{
    if (!py->m_bEnglishMode)        //m_bEnglishMode初始值为false
	{
        //ui->btnCaps->setChecked(false);
		changflagjianti = 0;
		isCaps = 0;
		return;
	}
    if(changflagjianti)
    {
		changeTextCaps(true);
        ui->btnCaps->setChecked(true);
	}
	else
	{
		changeTextCaps(false);
        ui->btnCaps->setChecked(true);
	}

	changflagjianti = !changflagjianti;
	isCaps = changflagjianti;
}

void Digitpanel::changeEvent(QEvent *e)     //键盘－鼠标事件切换
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
void Digitpanel::showEvent(QShowEvent *e)
{
    if ( m_type == 0 )
    {
	    if (g_pMainDlg->m_inputMethodMode == 0)
		    on_btnchangeENGLISH_clicked();
	    else
		    on_btnchangeCHINESE_clicked();
    }
    else if ( m_type == 20 )    // 输入IP用的
    {
	    on_btnchangeENGLISH_clicked();
    }
    else if ( m_type == 30 )    // 输入门口机
    {
	    on_btnchangeENGLISH_clicked();
	    if ( changflagjianti2 == 1 )
	    	on_btn_sym2_clicked();
    }
    else if ( m_type == 40 )    // 输入特殊IP用的
    {
	    on_btnchangeENGLISH_clicked();
	    on_btn_sym2_clicked();
    }

    if (m_inputMethodStatus)
    {
        ui->toolbiaodian->setText("abc");
    }
    else
    {
        ui->toolbiaodian->setText("123");
    }
}
void Digitpanel::mousePressEvent(QMouseEvent *e)
{
    //printf("shubiaoanxia \n");
    int x,y;
    x=e->x();
    y=e->y();
//    printf("%d,%d\n",e->x(),e->y());

        printf("on hanzi\n");
	if ( g_pScaleSrc->get_screen_version() == 1 )
	{
	    if(y>40 && y<90 &&  py->m_ime_info.candidates_count>0)
	    {
		if(x>0 && x<78)py->filter(49);
		if(x>80 && x<158)py->filter(50);
		if(x>160 && x<238)py->filter(51);
		if(x>240 && x<318)py->filter(52);
		if(x>320 && x<398)py->filter(53);
		if(x>400 && x<478)py->filter(54);
		if(x>480 && x<558)py->filter(55);
		if(x>560 && x<638)py->filter(56);
	    }
	}
	else if ( g_pScaleSrc->get_screen_version() == 2 )
	{
	    if(y>40 && y<100 &&  py->m_ime_info.candidates_count>0)
	    {
		if(x>0 && x<101)py->filter(49);
		if(x>103 && x<204)py->filter(50);
		if(x>206 && x<307)py->filter(51);
		if(x>309 && x<410)py->filter(52);
		if(x>412 && x<513)py->filter(53);
		if(x>515 && x<616)py->filter(54);
		if(x>618 && x<719)py->filter(55);
		if(x>721 && x<824)py->filter(56);
	    }
	}
	else if ( g_pScaleSrc->get_screen_version() == 3 )
	{
	    if(y>50 && y<150 &&  py->m_ime_info.candidates_count>0)
	    {
		if(x>0 && x<128)py->filter(49);
		if(x>130 && x<258)py->filter(50);
		if(x>260 && x<388)py->filter(51);
		if(x>390 && x<518)py->filter(52);
		if(x>520 && x<648)py->filter(53);
		if(x>650 && x<778)py->filter(54);
		if(x>780 && x<908)py->filter(55);
		if(x>910 && x<1038)py->filter(56);
	    }
	}

	{
		if(e->button()==Qt::LeftButton)
		{           m_dragPosition = e->globalPos() - frameGeometry().topLeft();
			e->accept();
			m_isDrag = true;

			return;
		}
	}

        return;
}

void Digitpanel::mouseReleaseEvent(QMouseEvent* m)
{
    //printf("shubiaoshifang \n");
    Q_UNUSED(m);
    m_isDrag=false;
    return;
}

#if 0
void Digitpanel::mouseMoveEvent(QMouseEvent *e)
{
    int x,y;
    x=e->x();
    y=e->y();
    if(y>25 && y<55 && !py->m_bEnglishMode && x>30 && x<400)
    {
        printf("return....\n");
        return;
    }
    printf("move...\n");
    if(m_isDrag && y<60)
    {
        if(e->buttons() & Qt::LeftButton){
            move(e->globalPos()-m_dragPosition);
            e->accept();
        }
    }
}
#endif

void Digitpanel::on_btn_sym1_clicked()  //全角－中文标点
{
//	py->resetState();
//	py->repaint();  //xiugai
    if (changflagjianti1)
    {
		changeMarkjianti1();

		ui->toolbiaodian->setText("abc");
		ui->btn_sym4->hide();//zj
		ui->btn_sym3->show();
		ui->btnCaps->hide();
	}
    else
    {
        ui->btn_sym4->hide();//zj
        ui->btn_sym3->hide();
        ui->btnCaps->show();

        ui->toolbiaodian->setText("123");
        ui->btnQ->setText(QChar('q'));
        ui->btnQ->setAccessibleName("0x71");
        ui->btnW->setText(QChar('w'));
        ui->btnW->setAccessibleName("0x77");
        ui->btnE->setText(QChar('e'));
        ui->btnE->setAccessibleName("0x65");
        ui->btnR->setText(QChar('r'));
        ui->btnR->setAccessibleName("0x72");
        ui->btnT->setText(QChar('t'));
        ui->btnT->setAccessibleName("0x74");
        ui->btnY->setText(QChar('y'));
        ui->btnY->setAccessibleName("0x79");
        ui->btnU->setText(QChar('u'));
        ui->btnU->setAccessibleName("0x75");
        ui->btnI->setText(QChar('i'));
        ui->btnI->setAccessibleName("0x69");
        ui->btnO->setText(QChar('o'));
        ui->btnO->setAccessibleName("0x6f");
        ui->btnP->setText(QChar('p'));
        ui->btnP->setAccessibleName("0x70");

        ui->btnA->setText(QChar('a'));
        ui->btnA->setAccessibleName("0x61");
        ui->btnS->setText(QChar('s'));
        ui->btnS->setAccessibleName("0x73");
        ui->btnD->setText(QChar('d'));
        ui->btnD->setAccessibleName("0x64");
        ui->btnF->setText(QChar('f'));
        ui->btnF->setAccessibleName("0x66");
        ui->btnG->setText(QChar('g'));
        ui->btnG->setAccessibleName("0x67");
        ui->btnH->setText(QChar('h'));
        ui->btnH->setAccessibleName("0x68");
        ui->btnJ->setText(QChar('j'));
        ui->btnJ->setAccessibleName("0x6a");
        ui->btnK->setText(QChar('k'));
        ui->btnK->setAccessibleName("0x6b");
        ui->btnL->setText(QChar('l'));
        ui->btnL->setAccessibleName("0x6c");

        ui->btnZ->setText(QChar('z'));
        ui->btnZ->setAccessibleName("0x7a");
        ui->btnX->setText(QChar('x'));
        ui->btnX->setAccessibleName("0x78");
        ui->btnC->setText(QChar('c'));
        ui->btnC->setAccessibleName("0x63");
        ui->btnV->setText(QChar('v'));
        ui->btnV->setAccessibleName("0x76");
        ui->btnB->setText(QChar('b'));
        ui->btnB->setAccessibleName("0x62");
        ui->btnN->setText(QChar('n'));
        ui->btnN->setAccessibleName("0x6e");
        ui->btnM->setText(QChar('m'));
        ui->btnM->setAccessibleName("0x6d");
        ui->btncomma->setText(QChar('-'));
        ui->btncomma->setAccessibleName("0x2d");//ff0c
        ui->btnPeriod->setText(QChar('.'));
        ui->btnPeriod->setAccessibleName("0x2e");
	}

	changflagjianti1 = !changflagjianti1;
}

void Digitpanel::on_btn_sym2_clicked()   //半角－英文标点
{
	py->resetState();
    py->update();
	if (changflagjianti2){
        ui->toolbiaodian->setText("abc");
		changeMarkjianti2();
        printf("\tbiaodian\n");
        ui->btn_sym4->show();//zj
        ui->btn_sym3->hide();
        ui->btnCaps->hide();
	}
    else// if(changflagjianti2%2==0)    //20131105修改 zwb：
    {
        ui->toolbiaodian->setText("123");
        printf("\tzimu\n");
        ui->btn_sym4->hide();//zj
        ui->btn_sym3->hide();
        ui->btnCaps->show();

        ui->btnQ->setText(QChar('q'));
        ui->btnQ->setAccessibleName("0x71");
        ui->btnW->setText(QChar('w'));
        ui->btnW->setAccessibleName("0x77");
        ui->btnE->setText(QChar('e'));
        ui->btnE->setAccessibleName("0x65");
        ui->btnR->setText(QChar('r'));
        ui->btnR->setAccessibleName("0x72");
        ui->btnT->setText(QChar('t'));
        ui->btnT->setAccessibleName("0x74");
        ui->btnY->setText(QChar('y'));
        ui->btnY->setAccessibleName("0x79");
        ui->btnU->setText(QChar('u'));
        ui->btnU->setAccessibleName("0x75");
        ui->btnI->setText(QChar('i'));
        ui->btnI->setAccessibleName("0x69");
        ui->btnO->setText(QChar('o'));
        ui->btnO->setAccessibleName("0x6f");
        ui->btnP->setText(QChar('p'));
        ui->btnP->setAccessibleName("0x70");

        ui->btnA->setText(QChar('a'));
        ui->btnA->setAccessibleName("0x61");
        ui->btnS->setText(QChar('s'));
        ui->btnS->setAccessibleName("0x73");
        ui->btnD->setText(QChar('d'));
        ui->btnD->setAccessibleName("0x64");
        ui->btnF->setText(QChar('f'));
        ui->btnF->setAccessibleName("0x66");
        ui->btnG->setText(QChar('g'));
        ui->btnG->setAccessibleName("0x67");
        ui->btnH->setText(QChar('h'));
        ui->btnH->setAccessibleName("0x68");
        ui->btnJ->setText(QChar('j'));
        ui->btnJ->setAccessibleName("0x6a");
        ui->btnK->setText(QChar('k'));
        ui->btnK->setAccessibleName("0x6b");
        ui->btnL->setText(QChar('l'));
        ui->btnL->setAccessibleName("0x6c");

        ui->btnZ->setText(QChar('z'));
        ui->btnZ->setAccessibleName("0x7a");
        ui->btnX->setText(QChar('x'));
        ui->btnX->setAccessibleName("0x78");
        ui->btnC->setText(QChar('c'));
        ui->btnC->setAccessibleName("0x63");
        ui->btnV->setText(QChar('v'));
        ui->btnV->setAccessibleName("0x76");
        ui->btnB->setText(QChar('b'));
        ui->btnB->setAccessibleName("0x62");
        ui->btnN->setText(QChar('n'));
        ui->btnN->setAccessibleName("0x6e");
        ui->btnM->setText(QChar('m'));
        ui->btnM->setAccessibleName("0x6d");
        ui->btncomma->setText(QChar('-'));
        ui->btncomma->setAccessibleName("0x2d");
        ui->btnPeriod->setText(QChar('.'));
        ui->btnPeriod->setAccessibleName("0x2e");
    }

	changflagjianti2 = !changflagjianti2;
}

void Digitpanel::on_btn_sym3_clicked()  //+ -运算符号  ZWB:修改20131105
{
	py->resetState();
	py->repaint();

	if (changflagjianti3){
        printf("qiehuan:yunsuanfu0 \n");
        ui->btnQ->setText("+");
        ui->btnQ->setAccessibleName("0x2b");
        ui->btnW->setText(QString::fromUtf8("-"));
        ui->btnW->setAccessibleName("0x2d");
        ui->btnE->setText(QString::fromUtf8("×"));
        ui->btnE->setAccessibleName("0xd7");
        ui->btnR->setText(QString::fromUtf8("÷"));
        ui->btnR->setAccessibleName("0xf7");
        ui->btnT->setText("=");
        ui->btnT->setAccessibleName("0x3d");
        ui->btnY->setText(QString::fromUtf8("…"));
        ui->btnY->setAccessibleName("0x2026");
        ui->btnU->setText(QString::fromUtf8("±"));
        ui->btnU->setAccessibleName("0xb1");
        ui->btnI->setText("/");
        ui->btnI->setAccessibleName("0x2f");
        ui->btnO->setText("\\");
        ui->btnO->setAccessibleName("0x5c");
        ui->btnP->setText("_");
        ui->btnP->setAccessibleName("0x5f");
	}
    else
    {
        ui->btnQ->setText(QChar('1'));
        ui->btnQ->setAccessibleName("0x31");
        ui->btnW->setText(QChar('2'));
        ui->btnW->setAccessibleName("0x32");
        ui->btnE->setText(QChar('3'));
        ui->btnE->setAccessibleName("0x33");
        ui->btnR->setText(QChar('4'));
        ui->btnR->setAccessibleName("0x34");
        ui->btnT->setText(QChar('5'));
        ui->btnT->setAccessibleName("0x35");
        ui->btnY->setText(QChar('6'));
        ui->btnY->setAccessibleName("0x36");
        ui->btnU->setText(QChar('7'));
        ui->btnU->setAccessibleName("0x37");
        ui->btnI->setText(QChar('8'));
        ui->btnI->setAccessibleName("0x38");
        ui->btnO->setText(QChar('9'));
        ui->btnO->setAccessibleName("0x39");
        ui->btnP->setText(QChar('0'));
        ui->btnP->setAccessibleName("0x30");
    }

	changflagjianti3 = !changflagjianti3;
}

void Digitpanel::on_btn_sym4_clicked()   //＃％符号
{
	py->resetState();
	py->repaint();

	if (changflagjianti4){
        changeTextShift(true);
	}
    else
    {
        changeTextShift(false);
    }

	changflagjianti4 = !changflagjianti4;
}

//切换到英文输入
void Digitpanel::on_btnchangeENGLISH_clicked()
{
    changflagjianti=1;   //dx
    changflagjianti2=1; //en
    changflagjianti4=1;  //=# en

    //printf("En  anxia \n");
    ui->btnCaps->show();//
    ui->btnCaps->setEnabled(true);
    ui->btn_sym3->hide();
    ui->btn_sym4->hide();
    py->m_bEnglishMode = true;
    ui->btnchangeENGLISH->hide();
    ui->btnchangeCHINESE->show();
    ui->labelCHINESE->hide();
    ui->labelENGLISH->show();
    ui->toolbiaodian->setText("123");
    py->resetState();
    if (m_inputMethodStatus)
        changeMarkjianti2();
    else
        changeTextCaps(false);
    py->update();
}

//切换到中文输入
void Digitpanel::on_btnchangeCHINESE_clicked()
{
    changflagjianti1=1; //zh
    changflagjianti3=1;  //+- zh

    ui->btnCaps->show();
    ui->btnCaps->setEnabled(false);
    ui->btn_sym3->hide();
    ui->btn_sym4->hide();

    py->m_bEnglishMode = false;
    ui->btnchangeCHINESE->hide();
    ui->btnchangeENGLISH->show();
    ui->labelENGLISH->hide();
    ui->labelCHINESE->show();
    ui->toolbiaodian->setText("123");
    if (m_inputMethodStatus)
        changeMarkjianti1();
    else
        changeTextCaps(false);
    py->resetState();
    py->update();
}

//*********************************************
//**  modified by wangdongxing at 2012.6.5   **
//**  to accelarate the speed of reaction    **
//*********************************************
void Digitpanel::on_btn_sym1_pressed()
{
    on_btn_sym1_clicked();
}
void Digitpanel::on_btn_sym2_pressed()
{
    on_btn_sym2_clicked();
}
void Digitpanel::on_btn_sym3_pressed()
{
    on_btn_sym3_clicked();
}
void Digitpanel::on_btn_sym4_pressed()
{
    on_btn_sym4_clicked();
}
void Digitpanel::on_btnchangeENGLISH_pressed()
{
    on_btnchangeENGLISH_clicked();
}
void Digitpanel::on_btnCaps_pressed()
{
    on_btnCaps_clicked();
}
void Digitpanel::on_btnchangeCHINESE_pressed()
{
    on_btnchangeCHINESE_clicked();
}
//*********************************************
//**    End to modify at 2012.6.5            **
//**                                         **
//*********************************************

void Digitpanel::on_toolbiaodian_pressed()  //zengjia 20131107
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
