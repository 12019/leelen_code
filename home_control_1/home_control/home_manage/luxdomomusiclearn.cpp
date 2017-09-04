#include "luxdomomusiclearn.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern MainDialog *g_pMainDlg;
extern LMap_6410_Msg mLMap_6410_Msg;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoMusicLearn::LuxDomoMusicLearn(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    m_musicFunction =  tr("打开*关闭*静音*非静音*mp3*cd*aux*radio*上一曲*下一曲*播放*暂停*音量递减*音量递增");
}

LuxDomoMusicLearn::~LuxDomoMusicLearn()
{
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem=NULL;
}

void   LuxDomoMusicLearn::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem=parentItem();
}

void    LuxDomoMusicLearn::close()
{
    cancelLearn();
    parentItem()->hide();
    parentItem()->deleteLater();
}

void   LuxDomoMusicLearn::cancelLearn()
{
    int   deviceId = deviceElement.attribute("id").toInt() & 0xff;
    char strCmd[6];
    strCmd[0] = 0x06;
    strCmd[1] = deviceId;  //设备号
    strCmd[2] = 0x00;    //源地址
    strCmd[3] = 0x0d;
    strCmd[4] = 0x00;
    strCmd[5] = 0x00;  //校验和
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
}

void   LuxDomoMusicLearn::learnClicked(int  cmd)
{
    int   deviceId = deviceElement.attribute("id").toInt() & 0xff;
    char strCmd[7];
    strCmd[0] = 0x07;
    strCmd[1] = deviceId;  //设备号
    strCmd[2] = 0x00;    //源地址
    strCmd[3] = 0x0d;
    strCmd[4] = 0x21;
    strCmd[5] = cmd; //指令
    strCmd[6] = 0x00;  //校验和
    m_code=cmd;
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
}

void  LuxDomoMusicLearn::init(QString  deviceUid)
{
    QString    roomUid=parentItem()->parentItem()->property("roomObjectName").toString();
    deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(deviceElement.isNull())
    {
          parentItem()->deleteLater();
    }

    bool isTrue;
    QSqlQuery  query; //以下执行相关QSL语句
    QString   mySql = "create table  music" + deviceUid + "(code int primary key,funName char[20],isLearned int)";
    isTrue = query.exec(mySql);
    if(isTrue)    //数据库是第一次创建
    {
        int code = 1;
        QString  head = "insert into music" + deviceUid + " values(";
        QStringList  m_TvFun = m_musicFunction.split(tr("*"));
        int size = m_TvFun.size();
        for(int i = 0;i < size;i ++)
        {
            QString sql = head +QString::number(code) + "," + "'" + m_TvFun.at(i) + "'" + "," +"0"+ ")";
            query.exec(sql);
            code ++;
        }
    }

    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    query.exec("select *  from music" + deviceUid);
    while (query.next())
    {
        int m_Code = query.value(0).toInt();
        QString m_FunName = query.value(1).toString();
        bool isLearned = query.value(2).toInt() == 1?true:false;
        QMetaObject::invokeMethod(parentItem(),
                                  "addModel",
                                  Qt::DirectConnection,
                                  Q_ARG(QVariant,m_Code),
                                  Q_ARG(QVariant,m_FunName),
                                  Q_ARG(QVariant,isLearned));
    }
}
void  LuxDomoMusicLearn::learnCode()
{
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("isLearned")), Q_ARG(QVariant,true));
    parentItem()->setProperty("pressIndex",-1);
    parentItem()->setProperty("isShowIrHint",false);
    QSqlQuery query; //以下执行相关QSL语句
    QString  sql="update  music"+deviceElement.attribute("uid")+"  set   isLearned=  "+"1"+"  where  code=" + QString::number(m_code);
    MyMsgLog<<query.exec(sql);
    MyMsgLog<<sql;
}
