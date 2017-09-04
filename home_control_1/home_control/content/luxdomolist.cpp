#include "luxdomolist.h"

LuxDomoList::LuxDomoList(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoList::~LuxDomoList()
{

}


void  LuxDomoList::componentComplete()
{

}

void  LuxDomoList::setDeviceData(QString  data)
{
    QString    str=parentItem()->property("dataType").toString();
    QByteArray   type=str.toLatin1();
    parentItem()->parentItem()->setProperty(type.data(),data);
    parentItem()->parentItem()->setProperty("arrowsCondition",tr(""));
    parentItem()->hide();
}
