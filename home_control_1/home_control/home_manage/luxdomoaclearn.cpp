#include "luxdomoaclearn.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
extern   QMultiMap<QString,QDeclarativeEngine*>   engines;
LuxDomoAcLearn::LuxDomoAcLearn(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
    wind=tr("固定*摆风");
    mode=tr("制冷*加热*通风*除湿*自动");
    speed=tr("高速*中速*低速*自动");
    listEngine=NULL;
    listItem=NULL;
}

LuxDomoAcLearn::~LuxDomoAcLearn()
{
    resourceRelease();
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem=NULL;
}

void    LuxDomoAcLearn::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    g_pMainDlg->m_pHomeControlDlg->m_pRoomManage->infraredLearnItem=parentItem();
}

void   LuxDomoAcLearn::resourceRelease()
{
    parentItem()->setProperty("isShowIrHint",false);
    if(listEngine!=NULL)
    {
        listEngine->deleteLater();
        listEngine=NULL;
    }
    if(listItem!=NULL)
    {
        listItem->deleteLater();
        listItem=NULL;
    }
}

void  LuxDomoAcLearn::close()
{
    cancelLearn();
    parentItem()->hide();
    parentItem()->deleteLater();
}

void  LuxDomoAcLearn::cancelLearn()
{
    int deviceId = deviceElement.attribute("id").toInt() & 0xff;
    char strCmd[6];
    strCmd[0] = 0x06;
    strCmd[1] = deviceId;  //设备号
    strCmd[2] = 0x00;    //源地址
    strCmd[3] = 0x0d;
    strCmd[4] = 0x00;
    strCmd[5] = 0x00;  //校验和
    g_pMainDlg->m_pHomeControlDlg->cmdBuffer(strCmd);
}

void  LuxDomoAcLearn::learnClicked(int  cmd)
{
    resourceRelease();
    int deviceId = deviceElement.attribute("id").toInt() & 0xff;
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

void    LuxDomoAcLearn::init(QString   deviceUid)
{
    QString    roomUid=parentItem()->parentItem()->property("roomObjectName").toString();
    deviceElement=g_pMainDlg->m_pHomeControlDlg->m_pLoadingDataToUI->LoadingDeviceConditionData(roomUid,deviceUid);
    if(deviceElement.isNull())
    {
          parentItem()->deleteLater();
    }

    bool isTrue;
    QSqlQuery query; //以下执行相关QSL语句
    QString mySql = "create table ac" + deviceUid + "(code int primary key,power char[10],wind char[50],mode char[50],speed char[50],temp char[50],isLearned int)";
    isTrue = query.exec(mySql);
    if(isTrue)    //数据库是第一次创建
    {
        int code = 41;
        QString head = "insert into ac" + deviceUid + " values(";
        QString data = QString::number(code) + "," + tr("'关'") + ",'-','-','-','-',0)";
        QString sql = head + data;
        query.exec(sql);

        for(int i = 1;i <= 6;i++)
        {
            data = QString::number(code + i) + "," +   tr("'开'") + "," + tr("'固定'") + "," + tr("'制冷'") + "," + tr("'自动'") + "," +
                    "'" +QString::number(23 + i) + "℃" + "'" + "," + "0" + ")";
            sql = head + data;
            query.exec(sql);
        }
    }

    QMetaObject::invokeMethod(parentItem(),"clearModel",Qt::DirectConnection);
    query.exec("select * from ac" + deviceUid);
    while (query.next())
    {
        int code = query.value(0).toInt();
        QString power = query.value(1).toString();
        QString wind = query.value(2).toString();
        QString mode = query.value(3).toString();
        QString speed = query.value(4).toString();
        QString temp = query.value(5).toString();
        bool isLearned = query.value(6).toInt() == 1?true:false;
        QMetaObject::invokeMethod(parentItem(),
                                  "addModel",Qt::DirectConnection,
                                  Q_ARG(QVariant,code),Q_ARG(QVariant,power), Q_ARG(QVariant,wind),
                                  Q_ARG(QVariant,mode),Q_ARG(QVariant,speed), Q_ARG(QVariant,temp),
                                  Q_ARG(QVariant,isLearned));
    }
}

void     LuxDomoAcLearn::showTempList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine,QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(parentItem()->width()/5);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(parentItem()->width()*3/5);
    item->setY(mLMap_6410_Msg.screenH*2/3);
    item->setProperty("dataType",tr("temp"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    for(int i=16;i<33;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,QString::number(i))); }
    listEngine=engine;
    listItem=item;
}

void     LuxDomoAcLearn::showWindList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(parentItem()->width()/5);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(0);
    item->setY(mLMap_6410_Msg.screenH*2/3);
    item->setProperty("dataType",tr("wind"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    QStringList  m_acFun=wind.split(tr("*"));
    int  size=m_acFun.size();
    for(int i=0;i<size;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,m_acFun.at(i))); }
    listEngine=engine;
    listItem=item;
}

void    LuxDomoAcLearn::showModeList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(parentItem()->width()/5);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(parentItem()->width()/5);
    item->setY(mLMap_6410_Msg.screenH*2/3);
    item->setProperty("dataType",tr("mode"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    QStringList  m_acFun=mode.split(tr("*"));
    int  size=m_acFun.size();
    for(int i=0;i<size;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,m_acFun.at(i))); }
    listEngine=engine;
    listItem=item;
}

void   LuxDomoAcLearn::showSpeedList()
{
    resourceRelease();
    QDeclarativeEngine  *engine = new QDeclarativeEngine(parentItem());
    QDeclarativeComponent component(engine, QUrl("qrc:/QML/content/LuxDomoListDialog.qml"));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(component.create());
    item->setParentItem(parentItem());
    item->setWidth(parentItem()->width()/5);
    item->setHeight(mLMap_6410_Msg.screenH/3);
    item->setX(parentItem()->width()*2/5);
    item->setY(mLMap_6410_Msg.screenH*2/3);
    item->setProperty("dataType",tr("speed"));
    QMetaObject::invokeMethod(item,"clearModel",Qt::DirectConnection);
    QStringList  m_acFun=speed.split(tr("*"));
    int  size=m_acFun.size();
    for(int i=0;i<size;i++)
    {    QMetaObject::invokeMethod(item,"addModel",Qt::DirectConnection, Q_ARG(QVariant,m_acFun.at(i))); }
    listEngine=engine;
    listItem=item;
}

void   LuxDomoAcLearn::deleteCode(int code,int  index)
{
    bool isTrue;
    QSqlQuery query;
    isTrue = query.exec("delete from ac" + deviceElement.attribute("uid") + " where code = " + QString::number(code));
    if(isTrue)
    {
        QMetaObject::invokeMethod(parentItem(),"removeModel",Qt::DirectConnection,Q_ARG(QVariant,index));
        parentItem()->setProperty("pressCode",-1);
        parentItem()->setProperty("pressIndex",-1);
    }
    else   //没找到对应的code，删除失败
    {

    }
}

void   LuxDomoAcLearn::addCode()
{
    QString   acStatus=tr("开");
    QString   acWind=parentItem()->property("wind").toString();
    QString   acSpeed=parentItem()->property("speed").toString();
    QString   acTemp=parentItem()->property("temp").toString();
    QString   acMode=parentItem()->property("mode").toString();

    QSqlQuery query;
    QString    head = "select  *  from ac" + deviceElement.attribute("uid") + " where ";
    QString    _power = " power = '" + acStatus + "'" + " and";
    QString    _wind = " wind = '" + acWind + "'" + " and";
    QString    _mode = " mode = '" + acMode + "'" + " and";
    QString    _speed = " speed = '" + acSpeed + "'" + " and";
    QString    _temp = " temp = '" + acTemp + tr("℃")+"'";
    QString    sql = head + _power  +_wind + _mode + _speed + _temp;      //合并数据库查询语句,不允许重复添加
    MyMsgLog<<sql;
    query.exec(sql);
    if(query.next())
    {
        g_pMainDlg->m_pHomeControlDlg->ShowHintInformation(tr("该指令已学习"));
        return;
    }

    bool  findValidCode = false;
    int     code = 41;
    query.exec("select code from ac" +deviceElement.attribute("uid"));
    QList<int>  codes;
    while (query.next())               //收集所有空调红外编
    {
        codes.append(query.value(0).toInt());
    }
    int size = codes.size();
    for(int i = 0;i < size ;i ++)               //分配红外编码
    {
        ++code;
       if(!codes.contains(code) && code <= 109)
       {
           findValidCode = true;
           break;
       }
    }
    if(!findValidCode)
    {
        return;
    }

    head = "insert into ac" + deviceElement.attribute("uid") + " values(";
    sql = head +QString::number(code)  + "," +  "'" + acStatus + "'" + "," + "'" + acWind + "'" + "," + "'" + acMode + "'" + "," +
               "'" + acSpeed + "'" + "," + "'" + acTemp + tr("℃")+"'" + "," + "0" +")";
    query.exec(sql);
    MyMsgLog<<sql;
    QMetaObject::invokeMethod(parentItem(),
                              "addModel",
                              Qt::DirectConnection,
                              Q_ARG(QVariant,code),
                              Q_ARG(QVariant,acStatus),
                              Q_ARG(QVariant,acWind),
                              Q_ARG(QVariant,acMode),
                              Q_ARG(QVariant,acSpeed),
                              Q_ARG(QVariant,acTemp+tr("℃")),
                              Q_ARG(QVariant,false));
}
void  LuxDomoAcLearn::learnCode()
{ 
    QMetaObject::invokeMethod(parentItem(),"changModel",Qt::DirectConnection,
                                                     Q_ARG(QVariant,tr("isLearned")), Q_ARG(QVariant,true));
    parentItem()->setProperty("pressIndex",-1);
    parentItem()->setProperty("isShowIrHint",false);
     QSqlQuery query; //以下执行相关QSL语句
     QString  sql="update  ac"+deviceElement.attribute("uid")+"  set   isLearned=  "+"1"+"  where  code=" + QString::number(m_code);
     MyMsgLog<<query.exec(sql);
     MyMsgLog<<sql;
}
