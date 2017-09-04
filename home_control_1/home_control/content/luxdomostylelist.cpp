#include "luxdomostylelist.h"

LuxDomoStyleList::LuxDomoStyleList(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoStyleList::~LuxDomoStyleList()
{

}

void  LuxDomoStyleList::componentComplete()
{

}

void  LuxDomoStyleList::close()
{
      parentItem()->deleteLater();
}

void   LuxDomoStyleList::setDeviceData(QString  data)
{
       QMetaObject::invokeMethod(parentItem()->parentItem(),"setTypeData",Qt::DirectConnection, Q_ARG(QVariant,data));
}
