#include "luxdomomajorsetting.h"
#include"MyBuffer.h"
LuxDomoMajorSetting::LuxDomoMajorSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoMajorSetting::~LuxDomoMajorSetting()
{

}

void LuxDomoMajorSetting::componentComplete()
{
       MyMsgLog<<parentItem()->objectName()<<objectName();
}
