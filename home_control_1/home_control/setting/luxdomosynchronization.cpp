#include "luxdomosynchronization.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoSynchronization::LuxDomoSynchronization(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{

}

LuxDomoSynchronization::~LuxDomoSynchronization()
{

}

void  LuxDomoSynchronization::componentComplete()
{
    MyMsgLog<<parentItem()->objectName()<<objectName();
    parentItem()->setWidth(mLMap_6410_Msg.screenW*9/16);
    parentItem()->setHeight(mLMap_6410_Msg.screenH);
    parentItem()->setX(mLMap_6410_Msg.screenW*5/16);
    parentItem()->setY(0);

    QString   describe(tr("       文件同步是指把当前的配置文件同步到客户端（手机，iPad等等），"
                                       "使得各控制端的文件一致。同步的文件有多种，除基本的配置文件外，"
                                       "还有场景XML文件，红外数据库指令文件，RISCO安防报警记录等等。"
                                       "终端机选择同步后，其他客户端重启即可完成同步。"));
    parentItem()->setProperty("describe",describe);
}

void    LuxDomoSynchronization::ChangVersion()
{
    QSettings  qsettings1(XmlFilePath+tr("version.ini"),QSettings::IniFormat);
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int xxx=qrand()%0xffffffff;
    MyMsgLog<<"the srand is"<<xxx;
    qsettings1.setValue("version",xxx);
    MyMsgLog<<qsettings1.value("version").toString();
}

void    LuxDomoSynchronization::fileSynchronization()
{
    QFile  map("/mnt/disk/home_control/house-map-default1.xml");
    QFile  backups("/mnt/disk/home_control/house-map-default_backups.xml");
    if(map.size()<backups.size())
    {
        QFile::remove("/mnt/disk/home_control/house-map-default1.xml");
        QFile::copy("/mnt/disk/home_control/house-map-default_backups.xml","/mnt/disk/home_control/house-map-default1.xml");
    }

    ChangVersion();
    QDir  *fileDir = new QDir(XmlFilePath);
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(fileDir->entryInfoList(filter));

    for(int i=0;i<fileInfo->size();i++)         //先删除原有的文件，再复制文件
    {
        QString fileName = fileInfo->at(i).fileName();
        if(fileName == "." || fileName == ".."||fileName==tr("house-map-default_backups.xml"))
        {
            continue;
        }
        QFile  file(XmlFilePath+tr("home_control/")+fileName);
        if(file.exists())
        {
            file.remove();
        }
        QFile::copy(XmlFilePath+fileName,XmlFilePath+tr("home_control/")+fileName);
    }

    parentItem()->setProperty("result",tr("成功"));
    delete  fileDir;
    delete  fileInfo;
}
