#include "hcsceneapplythread.h"
#include<QDebug>
#include<QFile>
#include<MyBuffer.h>
#include<homecontrolmaindlg.h>
#include<QSettings>

extern short layerId,roomId;
#define timedelay 50000

HCSceneApplyThread::HCSceneApplyThread(QObject *parent) :
    QThread(parent)
{
    isQuit = false;
}

void HCSceneApplyThread::run()
{
    while (1) {
        useHCScene(1);
        sleep(1);
        useHCScene(2);
        sleep(1);
    }
    useHCScene(indx);
    indx = 0;

    HomeControlMainDlg *pHome = (HomeControlMainDlg*)parent();
    pHome->saveHouseMapXmlFile();
}

int HCSceneApplyThread::useHCScene(int index)
{
    char strFileName[256];
    sprintf(strFileName, "home_control/house-map-user-%d.xml", index);
    //qDebug()<<"xml path:"<<strFileName;
    QFile file(strFileName);
    if(!file.exists())
    {
        qDebug()<<strFileName<<"is not exist";
        return -1;
    }
    applyScene(strFileName);
    return  0;
}

int HCSceneApplyThread::applyScene(QString strSceneFileName)
{
    QFile file(strSceneFileName);
    QDomDocument doc;
    QDomElement houseElement;
    QDomElement layerElement;
    QDomElement roomElement;
    QDomElement deviceElement;
    if (!file.open(QIODevice::ReadOnly)) return -1;
    if (!doc.setContent(&file))
    {
        qDebug()<<"open error";
        file.close();
        return -1;
    }
    file.close();

    houseElement = doc.firstChildElement("house");
    layerElement = houseElement.firstChildElement("layer");
    while (!layerElement.isNull())
    {
        if(layerElement.attribute("mark").toShort() == layerId)
        {
            roomElement = layerElement.firstChildElement("room");
            while (!roomElement.isNull())
            {
                if(roomElement.attribute("id").toShort() == roomId)
                {
                    deviceElement = roomElement.firstChildElement("device");
                    while (!deviceElement.isNull())
                    {
                        if(isQuit)
                        {
                            return -1;
                        }
                        if(deviceElement.attribute("actflag") == "enable")
                            applyDevice(deviceElement);
                        deviceElement = deviceElement.nextSiblingElement("device");
                    }
                }
                roomElement = roomElement.nextSiblingElement("room");
            }
        }
        layerElement = layerElement.nextSiblingElement("layer");
    }
    return 0;
}

int HCSceneApplyThread::applyDevice(QDomElement &deviceElement)
{
    HomeControlMainDlg *pHome = (HomeControlMainDlg*)parent();
    char cmd[10];
    memset(cmd,0,10);
    int deviceId = deviceElement.attribute("id").toInt();
    int attribute = deviceId>>8; //属性

    bool bPowerOn = deviceElement.attribute("power","off") == "on";
    if(attribute >=0x01 && attribute <= 0x08)  //单模触摸开关输出模块、四路开关输出模块
    {
        int code1 = bPowerOn ? 0x01:0x00;
        cmd[0] = 0x06;
        cmd[1] = deviceId & 0xff;
        cmd[3] = attribute;  //属性
        cmd[4] = code1;
        cmd[5] = 0x00;

        pHome->slot_sceneBuffer(cmd,6,"");
        //emit sig_sceneBuffer(cmd,6,"");
        usleep(timedelay);
    }
    else if(attribute >= 0x09 && attribute <=0x0c)  //单模调光驱动模块
    {
        int code1 = bPowerOn ? 0x01:0x00;
        cmd[1] = deviceId & 0xff;
        cmd[3] = attribute;  //属性

        if(code1 == 0x01)
        {
            int iVal = deviceElement.attribute("value").toInt();

            cmd[0] = 0x07;
            cmd[4] = 0x11;
            cmd[5] = iVal;

            pHome->slot_sceneBuffer(cmd,7,"");
            //emit sig_sceneBuffer(cmd,7,"");
            usleep(timedelay);
        }

        cmd[0] = 0x06;
        cmd[4] = code1;

        pHome->slot_sceneBuffer(cmd,6,"");
        //emit sig_sceneBuffer(cmd,6,"");
        usleep(timedelay);

    }
    else if(attribute == 0x0d)  //红外家电模块
    {
        QString strType = deviceElement.attribute("type");
        QDomElement actionElement = deviceElement.firstChildElement("action");
        if(strType == "ac")
        {
            /****************************开关*******************************/
            int code1 = 0xff; //无效值
            QString arg1 = "power"+deviceElement.attribute("power");  //开关
            QString arg2 = actionElement.attribute(arg1);            //指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[0] = 0x08;
            cmd[1] = deviceId & 0xff;
            cmd[3] = 0x0D;   //设备属性
            cmd[4] = 0x20;
            cmd[5] = code1;
            cmd[6] = 0x01;
            cmd[7] = 0;   //校验和

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);

            /*********************摆风*************************/
            code1 = 0xff;
            arg1 = "swing"+deviceElement.attribute("swing");  //摆风
            arg2 = actionElement.attribute(arg1);            //摆风指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);


            /*******************模式************************/
            code1 = 0xff;
            arg1 = deviceElement.attribute("mode");  //模式
            if(arg1 == "auto")
                arg1 = "automode";
            arg2 = actionElement.attribute(arg1);            //，模式指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);


            /*******************风速************************/
            code1 = 0xff;
            arg1 = deviceElement.attribute("wind");          //模式
            if(arg1 == "auto")
                arg1 = "windauto";
            arg2 = actionElement.attribute(arg1);            //模式指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);


            /*******************温度************************/
            code1 = 0xff;
            arg1 = "t"+deviceElement.attribute("temperature");          //模式
            arg2 = actionElement.attribute(arg1);            //模式指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);

        }
        else if(strType == "tv")
        {
            /****************************开关*******************************/
            int code1 = 0xff; //无效值
            QString arg1 = "power"+deviceElement.attribute("power");  //开关
            QString arg2 = actionElement.attribute(arg1);            //指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[0] = 0x08;
            cmd[1] = deviceId & 0xff;
            cmd[2] = 0x00;
            cmd[3] = 0x0D;   //设备属性
            cmd[4] = 0x20;
            cmd[5] = code1;
            cmd[6] = 0x01;
            cmd[7] = 0;   //校验和

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);

            /*********************摆风*************************/
            code1 = 0xff;
            arg1 = deviceElement.attribute("mode");  //模式
            arg2 = actionElement.attribute(arg1);            //模式指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);
        }
        else if(strType == "music")
        {
            /****************************开关*******************************/
            int code1 = 0xff; //无效值
            QString arg1 = "power"+deviceElement.attribute("power");  //开关
            QString arg2 = actionElement.attribute(arg1);            //指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[0] = 0x08;
            cmd[1] = deviceId & 0xff;
            cmd[3] = 0x0D;   //设备属性
            cmd[4] = 0x20;
            cmd[5] = code1;
            cmd[6] = 0x01;
            cmd[7] = 0;   //校验和

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);

            /***********************资源*************************/
            code1 = 0xff;
            arg1 = deviceElement.attribute("source");        //资源
            arg2 = actionElement.attribute(arg1);            //资源指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);

            /**********************静音*************************/
            code1 = 0xff;
            arg1 = deviceElement.attribute("mute");          //静音
            if(arg1 == "on")
                arg1 = "mute";
            else
                arg1 = "umute";
            arg2 = actionElement.attribute(arg1);            //静音指令
            if(!arg2.isEmpty())
                code1 = arg2.toInt();
            cmd[5] = code1;

            pHome->slot_sceneBuffer(cmd,8,strType);
            //emit sig_sceneBuffer(cmd,8,strType);
            usleep(timedelay);
        }
    }
    else
    {
        if(attribute == Central_AC)    //中央空調
        {

        }
    }
   return 0;
}
