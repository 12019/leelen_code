#include "luxdomopulldownlist.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;

LuxDomoPullDownList::LuxDomoPullDownList(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoPullDownList::~LuxDomoPullDownList()
{


}

void  LuxDomoPullDownList::componentComplete()
{
           MyMsgLog<<parentItem()->objectName()<<objectName();
}

void  LuxDomoPullDownList::close()
{
          parentItem()->deleteLater();
}

void  LuxDomoPullDownList::setDeviceData(QString  data)
{
    QString    str=parentItem()->property("dataType").toString();
    QByteArray  type=str.toLatin1();
    parentItem()->parentItem()->setProperty(type.data(),data);
}

















