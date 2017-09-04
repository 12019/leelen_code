#include "luxdomofeedbacksetting.h"
#include"maindialog.h"
#include"MyBuffer.h"
#include"thread/luxdomomail.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
LuxDomoFeedbackSetting::LuxDomoFeedbackSetting(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    optionTypes.clear();
    types<<tr("远程控制时");
    types<<tr("房间添加时");
    types<<tr("房间编辑时");
    types<<tr("设备添加时");
    types<<tr("设备编辑时");
    types<<tr("红外学习时");
    types<<tr("场景设置时");
}

LuxDomoFeedbackSetting::~LuxDomoFeedbackSetting()
{

}

void   LuxDomoFeedbackSetting::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);
}

void  LuxDomoFeedbackSetting::init()
{
     QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
     for(int i=0;i<types.size();i++)
     {
         QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                                               Q_ARG(QVariant,types.at(i)),
                                               Q_ARG(QVariant,false));
     }
}

void  LuxDomoFeedbackSetting::editMessage()
{
    dataType=FeedbackMessage;
    QString  data=parentItem()->property("message").toString();
    g_pMainDlg->m_pHomeControlDlg->m_editName=data;
    g_pMainDlg->m_pHomeControlDlg->setHCLineText(data);
}

void  LuxDomoFeedbackSetting::setInformation(QString  data)
{
    if(dataType==FeedbackMessage)
    {
        parentItem()->setProperty("message",data);
    }
}

void  LuxDomoFeedbackSetting::optionType(int  index, QString  data)
{
    if(optionTypes.contains(index))
    {
        QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,index),
                                              Q_ARG(QVariant,tr("isOption")),
                                              Q_ARG(QVariant,false));
        optionTypes.remove(index);
    }
    else
    {
        QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,index),
                                              Q_ARG(QVariant,tr("isOption")),
                                              Q_ARG(QVariant,true));
        optionTypes.insert(index,data);
    }
}

void  LuxDomoFeedbackSetting::cancelFeedback()
{
    parentItem()->setProperty("message",tr(""));
    QMap<int,QString>::iterator  it=optionTypes.begin();
    for(;it!=optionTypes.end();it++)
    {
        QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                              Q_ARG(QVariant,it.key()),
                                              Q_ARG(QVariant,tr("isOption")),
                                              Q_ARG(QVariant,false));
    }
    optionTypes.clear();
}

void  LuxDomoFeedbackSetting::sendFeedback()
{
    QString  message=tr("类型:\n");
    QMap<int,QString>::iterator  it=optionTypes.begin();
    for(;it!=optionTypes.end();it++)
    {
        message.append(types.at(it.key()));
        message.append("\n");
    }
    message.append(tr("描述信息:\n"));
    message.append(parentItem()->property("message").toString());

    String  str=message.toStdString();
    LuxDomoMail   mail;
    if(0==mail.pSmtpMailContext(str.c_str()))
    {
        cancelFeedback();
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("发送成功"));
    }
    else
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("发送失败"));
    }
}
