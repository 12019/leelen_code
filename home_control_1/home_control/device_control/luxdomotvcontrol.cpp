#include "luxdomotvcontrol.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   OperateType   operatingType;
extern   QDeclarativeItem    *smartHomeItem;
LuxDomoTVControl::LuxDomoTVControl(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoTVControl::~LuxDomoTVControl()
{

}

void   LuxDomoTVControl::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(0);
    parentItem()->setY(0);
    parentItem()->hide();
}

void   LuxDomoTVControl::init(QString   roomUid, QString   deviceUid)
{
    m_deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(m_deviceElement.isNull())
    {
        parentItem()->hide();
    }
    int deviceId=m_deviceElement.attribute("id").toInt();
    m_tvAddr=deviceId & 0xff;
    m_tvAttr=(deviceId>>8) & 0xff;
    m_roomUid=roomUid;
    parentItem()->setProperty("tvCondition",true);
    QString   mode=m_deviceElement.attribute("mode");
    if(mode!=tr("av")&&mode!=tr("tv"))
    {
        mode=tr("av");
        m_deviceElement.setAttribute("mode",mode);
    }
    parentItem()->setProperty("tvMode",mode);
    parentItem()->setProperty("deviceUid",deviceUid);
    parentItem()->setProperty("tvName",m_deviceElement.attribute("name"));
    MyMsgLog<<m_deviceElement.attribute("power")<<m_deviceElement.attribute("mode")<<m_deviceElement.attribute("value");
}

void   LuxDomoTVControl::close()
{
    parentItem()->hide();
    parentItem()->setParentItem(smartHomeItem);
    if( g_pMainDlg->m_pHomeControlDlg->roomControl!=NULL)
    {
        g_pMainDlg->m_pHomeControlDlg->roomControl->controlDeviceItem=NULL;
    }
}

void   LuxDomoTVControl::TvStrCmdSend(int  cmd)
{
    MyMsgLog<<cmd;
    if(cmd <= 0)
    {
        return;
    }
    QByteArray m_array;
    m_array.resize(8);
    m_array.data()[0] = 0x08;
    m_array.data()[1] = m_tvAddr;
    m_array.data()[2] = 0x00;
    m_array.data()[3] = m_tvAttr;
    m_array.data()[4] = 0x20;      //指令
    m_array.data()[5] = cmd;
    m_array.data()[6] = 0x01;
    m_array.data()[7] = 0x00;    //校验和，未校验
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(m_array.data());
    m_array.clear();
}

void  LuxDomoTVControl::tvChannelDownClicked()
{
    int cmd = 0;
    cmd = obtainTVFunOrder(tr("频道递减"));
    TvStrCmdSend(cmd);
}

void  LuxDomoTVControl::tvChannelUpClicked()
{
    int cmd = 0;
    cmd = obtainTVFunOrder(tr("频道递增"));
    TvStrCmdSend(cmd);
}

void  LuxDomoTVControl::tvModeClicked()
{
    int cmd = -1;
    QString  mFunName;
    QString  attributeValue;
    if(parentItem()->property("tvMode").toString() == tr("tv"))
    {
        mFunName = tr("AV模式");
        attributeValue = tr("av");
    }
    else
    {
        mFunName = tr("TV模式");
        attributeValue = tr("tv");
    }
    parentItem()->setProperty("tvMode",attributeValue);
    cmd = obtainTVFunOrder(mFunName);
    TvStrCmdSend(cmd);
    m_deviceElement.setAttribute("mode",attributeValue);
}

void  LuxDomoTVControl::tvPowerClicked()
{
    int   cmd=-1;   //指令
    QString    mFunName;
    QString    attributeValue;
    if(m_deviceElement.attribute("power")==tr("off"))
    {
        mFunName=tr("开机");
        attributeValue=tr("on");
    }
    else
    {
        mFunName=tr("关机");
        attributeValue=tr("off");
    }
    cmd = obtainTVFunOrder(mFunName);
    TvStrCmdSend(cmd);
    QMetaObject::invokeMethod(parentItem()->parentItem(),"on_offDevice",Qt::DirectConnection,Q_ARG(QVariant,m_deviceElement.attribute(tr("uid"))),Q_ARG(QVariant,true));
    m_deviceElement.setAttribute("power",attributeValue);
}

void  LuxDomoTVControl::tvVolumeClicked(int  value)
{
    int  oldValue=m_deviceElement.attribute(tr("value")).toInt();
    if(value == oldValue)
    {
        return;
    }
    QString mFunName;
    int cmd = 0;
    if(value > oldValue)   //slider to right
    {
        mFunName = "音量递增";
    }
    else
    {
        mFunName = "音量递减";
    }
    cmd = obtainTVFunOrder(mFunName);
    TvStrCmdSend(cmd);
    g_pMainDlg->m_pHomeControlDlg->m_pChangDeviceData->slot_changDeviceAttribute(m_roomUid,m_deviceElement.attribute("uid"),tr("value"),QString::number(value));
}

void   LuxDomoTVControl::tvVolumeDownClicked()
{
    int cmd = 0;
    cmd = obtainTVFunOrder(tr("音量递减"));
    TvStrCmdSend(cmd);
}

void   LuxDomoTVControl::tvVolumelUpClicked()
{
    int cmd = 0;
    cmd = obtainTVFunOrder(tr("音量递增"));
    TvStrCmdSend(cmd);
}

int     LuxDomoTVControl::obtainTVFunOrder(QString funName)
{
    QSqlQuery query;
    QString mSqlOrder;
    int mOrder;
    bool mIsLearned;
    bool mOrderIsValid;
    mSqlOrder = "select code,isLearned from tv" + m_deviceElement.attribute("uid") + " where funName = " +
            "'" + funName + "'";
    MyMsgLog<<mSqlOrder;
    mOrderIsValid = query.exec(mSqlOrder);
    if(!mOrderIsValid || !query.next())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -1;
    }
    mOrder = query.value(0).toInt();
    mIsLearned = query.value(1).toBool();
    MyMsgLog<<mOrder<<mIsLearned;
    if(!mIsLearned)
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令未红外学习"));
        return -2;
    }
    else
    {
        return mOrder;
    }
}
