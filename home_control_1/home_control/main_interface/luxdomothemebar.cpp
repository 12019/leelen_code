#include "luxdomothemebar.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;

LuxDomoThemebar::LuxDomoThemebar(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    timer=new  QTimer(this);
    sensorValues.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_checkSensorValue()));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI,SIGNAL(sigMainInterfaceData(QString,QString,QString)),this,SLOT(slot_addRoom(QString,QString,QString)));
    connect(g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData,SIGNAL(sigRefreshSensorValueFromCmd(QString,QString,QString,QString,QString)),this,SLOT(slot_sensorValue(QString,QString,QString,QString,QString)));
}

LuxDomoThemebar::~LuxDomoThemebar()
{
    g_pMainDlg->m_pHomeControlDlg->m_pThemebar=NULL;
}

void  LuxDomoThemebar::componentComplete()
{
    g_pMainDlg->m_pHomeControlDlg->m_pThemebar=this;
    parentItem()->setWidth(mLMap_6410_Msg.screenW*7/8);
    parentItem()->setHeight(mLMap_6410_Msg.screenH*2/5);
    parentItem()->setX(mLMap_6410_Msg.screenW/8);
    parentItem()->setY(0);
    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    timer->start(50000);
}

QStringList   LuxDomoThemebar::GetSensorValue(QString roomUid)
{
    QStringList   data;
    for(int  i=0;i<sensorValues.size();++i)
    {
        if(sensorValues.at(i).roomUid==roomUid)
        {
            data<<sensorValues.at(i).airValue<<sensorValues.at(i).tempValue<<sensorValues.at(i).humidnessValue<<sensorValues.at(i).airValue1;
            return  data;
        }
    }
    return  data;
}

void   LuxDomoThemebar::SetPMValue(QString roomUid,QString deviceUid,QString sensor)
{
    float   pm=sensor.toFloat();
    QString   mPMValue;
    MyMsgLog<<pm;
    if(pm >= 0.0 && pm <= 35.0)
    {
        mPMValue = tr("优");
    }
    else if(pm > 35.0 && pm <= 75.0)
    {
        mPMValue = tr("良");
    }
    else if(pm > 75.0 && pm <= 115.0)
    {
        mPMValue = tr("轻度污染");
    }
    else if(pm > 115.0 && pm <= 150.0)
    {
        mPMValue = tr("中度污染");
    }
    else if(pm > 150.0 && pm <= 250.0)
    {
        mPMValue = tr("重度污染");
    }
    else if(pm > 250.0)
    {
        mPMValue = tr("严重污染");
    }
    else
    {
        mPMValue = tr("异常");
    }

    for(int  i=0;i<sensorValues.size();++i)
    {
        if(sensorValues.at(i).roomUid==roomUid)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("airValue")),Q_ARG(QVariant,mPMValue));
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=deviceUid;
            sensorValue.tempValue=sensorValues.at(i).tempValue;
            sensorValue.airValue=mPMValue;
            sensorValue.airValue1=sensor;
            sensorValue.humidnessValue=sensorValues.at(i).humidnessValue;
            sensorValue.airValueUpdate=true;
            sensorValue.tempAndHumidnessValueUpdate=sensorValues.at(i).tempAndHumidnessValueUpdate;
            sensorValues.replace(i,sensorValue);
            break;
        }
    }
}

void  LuxDomoThemebar::SetHumitureValue(QString roomUid,QString deviceUid,QString  temp, QString humidness)
{
    for(int  i=0;i<sensorValues.size();++i)
    {
        if(sensorValues.at(i).roomUid==roomUid)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("tempValue")),Q_ARG(QVariant,temp));
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("humidnessValue")),Q_ARG(QVariant,humidness));
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=deviceUid;
            sensorValue.tempValue=temp;
            sensorValue.airValue=sensorValues.at(i).airValue;
            sensorValue.airValue1=sensorValues.at(i).airValue1;
            sensorValue.humidnessValue=humidness;
            sensorValue.airValueUpdate=sensorValues.at(i).airValueUpdate;
            sensorValue.tempAndHumidnessValueUpdate=true;
            sensorValues.replace(i,sensorValue);
            break;
        }
    }
}

void   LuxDomoThemebar::slot_sensorValue(QString  roomUid,QString deviceUid,QString  type,QString value1,QString value2)
{
    if(tr("PM")==type)
    {
        SetPMValue(roomUid,deviceUid,value1);
    }
    else  if(tr("Humiture")==type)
    {
        SetHumitureValue(roomUid,deviceUid,value1,value2);
    }

    if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        emit  sigRefreshSensorValue(roomUid,deviceUid);
    }
}

void  LuxDomoThemebar::slot_checkSensorValue()
{
    MyMsgLog<<"check ..................";
    for(int  i=0;i<sensorValues.size();++i)
    {
        if(!sensorValues.at(i).airValueUpdate)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("airValue")),Q_ARG(QVariant,tr("离线")));
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=sensorValues.at(i).deviceUid;
            sensorValue.tempValue=sensorValues.at(i).tempValue;
            sensorValue.airValue=tr("离线");
            sensorValue.airValue1=tr("-1");
            sensorValue.humidnessValue=sensorValues.at(i).humidnessValue;
            sensorValue.airValueUpdate=sensorValues.at(i).airValueUpdate;
            sensorValue.tempAndHumidnessValueUpdate=sensorValues.at(i).tempAndHumidnessValueUpdate;
            sensorValues.replace(i,sensorValue);
            if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
            {
                emit  sigRefreshSensorValue(sensorValues.at(i).roomUid,sensorValues.at(i).deviceUid);
            }
        }
        else
        {
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=sensorValues.at(i).deviceUid;
            sensorValue.tempValue=sensorValues.at(i).tempValue;
            sensorValue.airValue=sensorValues.at(i).airValue;
            sensorValue.airValue1=sensorValues.at(i).airValue1;
            sensorValue.humidnessValue=sensorValues.at(i).humidnessValue;
            sensorValue.airValueUpdate=false;
            sensorValue.tempAndHumidnessValueUpdate=sensorValues.at(i).tempAndHumidnessValueUpdate;
            sensorValues.replace(i,sensorValue);
        }

        if(!sensorValues.at(i).tempAndHumidnessValueUpdate)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("tempValue")),Q_ARG(QVariant,tr("离线")));
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,i), Q_ARG(QVariant,tr("humidnessValue")),Q_ARG(QVariant,tr("离线")));
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=sensorValues.at(i).deviceUid;
            sensorValue.tempValue=tr("离线");
            sensorValue.airValue=sensorValues.at(i).airValue;
            sensorValue.airValue1=sensorValues.at(i).airValue1;
            sensorValue.humidnessValue=tr("离线");
            sensorValue.airValueUpdate=sensorValues.at(i).airValueUpdate;
            sensorValue.tempAndHumidnessValueUpdate=sensorValues.at(i).tempAndHumidnessValueUpdate;
            sensorValues.replace(i,sensorValue);
            if(g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
            {
                emit  sigRefreshSensorValue(sensorValues.at(i).roomUid,sensorValues.at(i).deviceUid);
            }
        }
        else
        {
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=sensorValues.at(i).deviceUid;
            sensorValue.tempValue=sensorValues.at(i).tempValue;
            sensorValue.airValue=sensorValues.at(i).airValue;
            sensorValue.airValue1=sensorValues.at(i).airValue1;
            sensorValue.humidnessValue=sensorValues.at(i).humidnessValue;
            sensorValue.airValueUpdate=sensorValues.at(i).airValueUpdate;
            sensorValue.tempAndHumidnessValueUpdate=false;
            sensorValues.replace(i,sensorValue);
        }
    }
}

void   LuxDomoThemebar::slot_addRoom(QString  roomUid, QString  roomName, QString type)
{
    if(tr("room")!=type)
    {
        return;
    }

    QMetaObject::invokeMethod(parentItem(),"addModel",Qt::DirectConnection,
                            Q_ARG(QVariant,roomName),Q_ARG(QVariant,tr("离线")),Q_ARG(QVariant,tr("离线")),Q_ARG(QVariant,tr("离线")));
    SensorValue   sensorValue;
    sensorValue.index=sensorValues.size();
    sensorValue.roomUid=roomUid;
    sensorValue.deviceUid=tr("");
    sensorValue.tempValue=tr("离线");
    sensorValue.airValue=tr("离线");
    sensorValue.airValue1=tr("-1");
    sensorValue.humidnessValue=tr("离线");
    sensorValue.airValueUpdate=false;
    sensorValue.tempAndHumidnessValueUpdate=false;
    sensorValues.append(sensorValue);
}

void   LuxDomoThemebar::slot_changRoomName(QString  roomUid, QString roomName)
{
    for(int  i=0;i<sensorValues.size();++i)
    {   MyMsgLog<<i<<sensorValues.at(i).index;
        if(sensorValues.at(i).roomUid==roomUid)
        {
            QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,sensorValues.at(i).index), Q_ARG(QVariant,tr("roomName")),Q_ARG(QVariant,roomName));
            break;
        }
    }
}

void  LuxDomoThemebar::slot_deleteRoom(QString roomUid)
{
    int   index=-1;
    for(int  i=0;i<sensorValues.size();++i)
    {
        if(sensorValues.at(i).roomUid==roomUid)
        {
            index=sensorValues.at(i).index;
            QMetaObject::invokeMethod(parentItem(),"removeModel",Qt::DirectConnection,Q_ARG(QVariant,index));
            sensorValues.remove(i);
            break;
        }
    }

    for(int  i=0;i<sensorValues.size();++i)
    {
        if(sensorValues.at(i).index>index)
        {
            SensorValue   sensorValue;
            sensorValue.index=sensorValues.at(i).index-1;
            sensorValue.roomUid=sensorValues.at(i).roomUid;
            sensorValue.deviceUid=sensorValues.at(i).deviceUid;
            sensorValue.tempValue=sensorValues.at(i).tempValue;
            sensorValue.airValue=sensorValues.at(i).airValue;
            sensorValue.airValue1=sensorValues.at(i).airValue1;
            sensorValue.humidnessValue=sensorValues.at(i).humidnessValue;
            sensorValue.airValueUpdate=sensorValues.at(i).airValueUpdate;
            sensorValue.tempAndHumidnessValueUpdate=sensorValues.at(i).tempAndHumidnessValueUpdate;
            sensorValues.replace(i,sensorValue);
        }
    }
}
