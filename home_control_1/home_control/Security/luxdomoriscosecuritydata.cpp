#include "luxdomoriscosecuritydata.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  LMap_6410_Msg mLMap_6410_Msg;

LuxDomoRiscoSecurityData::LuxDomoRiscoSecurityData(QObject *parent) : QObject(parent)
{

}

QStringList  LuxDomoRiscoSecurityData::GetDefenceAreaCount(char data)
{
    QStringList   count;
    uint8_t      value=(int)data;

    if(value>>7&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>6&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>5&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>4&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>3&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>2&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value>>1&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    if(value&(uint8_t)1==1){  count<<tr("1");  }
    else{  count<<tr("0");  }

    return  count;
}

void   LuxDomoRiscoSecurityData::DealRiscoSecurityInformation(char *buf)
{
    MyMsgLog<<"HHHHHHHHHHHHH";
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
         return;
     }
    if (!document.setContent(&file))
    {
         file.close();
         return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        securityElement.removeChild(informationElement);
    }
    QDomElement information = document.createElement("information");
    for(int i=5;i<=8;i++)
    {
        QStringList   count=GetDefenceAreaCount(buf[i]);
        if(!count.contains(tr("1"))||count.size()!=8)
        {
            continue;
        }
        QDomElement  partition=document.createElement("partition");
        partition.setAttribute("number",QString::number(i-4));
        partition.setAttribute("condition",tr("9"));
        for(int j=0;j<count.size();j++)
        {
            if(count.at(j)==tr("0"))
            {
                continue;
            }
            QDomElement  defenceArea=document.createElement("defenceArea");
            defenceArea.setAttribute("condition",tr("0"));
            defenceArea.setAttribute("number",QString::number(j+1));
            partition.appendChild(defenceArea);
        }
        information.appendChild(partition);
    }
    securityElement.appendChild(information);
    QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream stream( &file1 );
    document.save( stream, 4 );
    file1.close();
}

void   LuxDomoRiscoSecurityData::DealPartition(char *buf)
{
    int  count=(int)buf[0];
    if(count<=8)
    {
        return;
    }

    QString   condition;
    condition.clear();
    for(int i=6;i<count-2;i++)
    {
        condition=condition+QString::number((int)buf[i]);
        if(i!=count-3){  condition=condition+tr("+");  }
    }

    SetPartitionCondition((int)buf[5],condition);
}

void   LuxDomoRiscoSecurityData::DealDefenceArea(char *buf)
{
    int  count=(int)buf[0];
    if(count<=8)
    {
        return;
    }

    QString   condition;
    condition.clear();
    for(int i=6;i<count-2;i++)
    {
        condition=condition+QString::number((int)buf[i]);
        if(i!=count-3){  condition=condition+tr("+");  }
    }

    int  partitionNumber=-1;
    int  defenceAreaNumber=(int)buf[5];
    if(defenceAreaNumber>=0&&defenceAreaNumber<8)
    {
        partitionNumber=0;
    }
    else    if(defenceAreaNumber>=8&&defenceAreaNumber<16)
    {
         partitionNumber=1;
    }
    else    if(defenceAreaNumber>=16&&defenceAreaNumber<24)
    {
          partitionNumber=2;
    }
    else
    {
         partitionNumber=3;
    }

    SetDefenceAreaCondition(partitionNumber,defenceAreaNumber,condition);
    AddRiscoEvent(QString::number(partitionNumber),QString::number(defenceAreaNumber),condition);
}

void   LuxDomoRiscoSecurityData::DealControl(char *buf)
{
    int  count=(int)buf[0];
    if(count<=8)
    {
        return;
    }

    int   number=(int)buf[5];
    int   condition=(int)buf[6];
    if(condition==1)
    {
        SetOutputCondition(number,tr("on"));
    }
    else
    {
        SetOutputCondition(number,tr("off"));
    }
}

QString    LuxDomoRiscoSecurityData::ConditionTransition1(QString  condition)
{
    QString   value;
    QStringList    list=condition.split(tr("+"));

    if(list.contains(tr("7")))
    {
        value=tr("on");
    }
    else   if(list.contains(tr("8")))
    {
        value=tr("wait");
    }
    else
    {
        value=tr("off");
    }

    return  value;
}

QString   LuxDomoRiscoSecurityData::ConditionTransition2(QString  value,QString   condition)
{
    QStringList   list=condition.split(tr("+"));

    if(value==tr("on"))
    {
        if(list.contains(tr("8")))
        {
            condition.replace(tr("8"),tr("7"));
        }
        else   if(list.contains(tr("8")))
        {
            condition.replace(tr("9"),tr("7"));
        }
        else
        {
            condition=condition+tr("+7");
        }
    }
    else  if(value==tr("wait"))
    {
        if(list.contains(tr("7")))
        {
            condition.replace(tr("7"),tr("8"));
        }
        else   if(list.contains(tr("9")))
        {
            condition.replace(tr("9"),tr("8"));
        }
        else
        {
            condition=condition+tr("+8");
        }
    }
    else
    {
        if(list.contains(tr("7")))
        {
            condition.replace(tr("7"),tr("9"));
        }
        else   if(list.contains(tr("8")))
        {
            condition.replace(tr("8"),tr("9"));
        }
        else
        {
            condition=condition+tr("+9");
        }
    }

    return   condition;
}

QStringList   LuxDomoRiscoSecurityData::GetPartition()
{
    QStringList     data;
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return   data;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return   data;
     }
    if (!document.setContent(&file))
    {
        file.close();
        return  data;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            QString  value;
            value=partitionElement.attribute("number")+tr("*");
            value=value+ConditionTransition1(partitionElement.attribute("condition"));
            MyMsgLog<<value;
            data<<value;
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }
    return  data;
}

QStringList  LuxDomoRiscoSecurityData::GetDefenceArea()
{
    QStringList     data;
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return   data;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return   data;
     }
    if (!document.setContent(&file))
    {
        file.close();
        return  data;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            QDomElement    defenceAreaElement=partitionElement.firstChildElement("defenceArea");
            while(!defenceAreaElement.isNull())
            {
                QString  value;
                value=partitionElement.attribute("number")+tr("*");
                value=value+defenceAreaElement.attribute("number")+tr("*");
                QStringList   list=defenceAreaElement.attribute("condition").split(tr("+"));
                if(list.contains(tr("8")))
                {
                    value=value+tr("on");
                }
                else
                {
                    value=value+tr("off");
                }
                MyMsgLog<<value;
                data<<value;
                defenceAreaElement=defenceAreaElement.nextSiblingElement("defenceArea");
            }
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }

    return  data;
}

QStringList  LuxDomoRiscoSecurityData::GetEvent(QString type)
{
    QStringList   data;
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return data;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return data;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return data;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   eventElement;
    securityElement=document.firstChildElement("security");
    eventElement=securityElement.firstChildElement("event");
    if(!eventElement.isNull())
    {
        QDomElement  message=eventElement.firstChildElement("message");
        while(!message.isNull())
        {
            QStringList  condition=message.attribute("condition").split(tr("+"));
            if(type==tr("troubleEvent"))
            {
                if(!condition.contains(tr("5")))
                {
                    message=message.nextSiblingElement("message");
                    continue;
                }
            }
            else  if(type==tr("alarmEvent"))
            {
                if(!condition.contains(tr("3")))
                {
                    message=message.nextSiblingElement("message");
                    continue;
                }
            }

            QString   time=message.attribute("time");
            QString   partitionNumber=message.attribute("partitionNumber");
            QString   defenceAreaNumber=message.attribute("defenceAreaNumber");
            QString   conditionStr;
            for(int i=0;i<condition.size();i++)
            {
                conditionStr=conditionStr+ReceiveDefenceAreaCondition(condition.at(i));
                if(i!=condition.size()-1)
                {
                    conditionStr=conditionStr+tr(",");
                }
            }
            QString  str=time+tr(" 分区")+partitionNumber+tr("-防区")+defenceAreaNumber+tr("-")+conditionStr;
            data<<str;
            message=message.nextSiblingElement("message");
        }
    }

    return  data;
}

QStringList   LuxDomoRiscoSecurityData::GetOutput()
{
    QStringList   data;
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return data;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return data;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return data;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   outputElement;
    securityElement=document.firstChildElement("security");
    outputElement=securityElement.firstChildElement("output");
    if(!outputElement.isNull())
    {
        QDomElement  routeElement=outputElement.firstChildElement("route");
        while(!routeElement.isNull())
        {
            data<<routeElement.attribute("condition");
            MyMsgLog<<routeElement.attribute("condition");
            routeElement=routeElement.nextSiblingElement("route");
        }
    }
    return  data;
}

void   LuxDomoRiscoSecurityData::PartitionDefence(int  partitionNumber, QString  value)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            if(partitionElement.attribute("number")==QString::number(partitionNumber))
            {
                QString  condition=ConditionTransition2(value,partitionElement.attribute("condition"));
                partitionElement.setAttribute("condition",condition);
                QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
                if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    return;
                }
                QTextStream stream( &file1 );
                document.save( stream, 4 );
                file1.close();
                return;
            }
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }
}

void  LuxDomoRiscoSecurityData::DefenceAreaDypass(int  partitionNumber, int  defenceAreaNumber, QString  value)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            if(partitionElement.attribute("number")==QString::number(partitionNumber))
            {
                QDomElement   defenceAreaElement=partitionElement.firstChildElement("defenceArea");
                while(!defenceAreaElement.isNull())
                {
                    if(defenceAreaElement.attribute("number")==QString::number(defenceAreaNumber))
                    {
                        QString   condition;
                        condition.clear();
                        if(value==tr("on"))
                        {
                            condition=defenceAreaElement.attribute("condition")+tr("+8");
                        }
                        else
                        {
                           condition=defenceAreaElement.attribute("condition").remove(tr("+8"));
                        }
                        defenceAreaElement.setAttribute("condition",condition);
                        QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
                        if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                        {
                            return;
                        }
                        QTextStream stream( &file1 );
                        document.save( stream, 4 );
                        file1.close();
                        return;
                    }
                    defenceAreaElement=defenceAreaElement.nextSiblingElement("defenceArea");
                }
            }
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }
}

QString   LuxDomoRiscoSecurityData::ReceiveDefenceAreaCondition(QString value)
{
    switch (value.toInt())
    {
    case 1:
        return  tr("Open (Trip)");
        break;
    case 2:
        return  tr("Arm");
        break;
    case 3:
        return  tr("Alarm");
        break;
    case 4:
         return  tr("Tamper");
        break;
    case 5:
        return  tr("Trouble");
        break;
    case 6:
         return  tr("Lost");
        break;
    case 7:
        return  tr("Low Battery trouble");
        break;
    case 8:
        return  tr("Bypass");
        break;
    case 9:
        return  tr("Communication trouble");
        break;
    case 10:
        return  tr("Soak Test trouble");
        break;
    default:
        break;
    }
}

QString  LuxDomoRiscoSecurityData::ReceivePartitionCondition(QString  value)
{
    switch (value.toInt())
    {
    case 1:
        return  tr("Alarm");
        break;
    case 2:
        return  tr("Duress");
        break;
    case 3:
        return  tr("False Coed");
        break;
    case 4:
         return  tr("Fire");
        break;
    case 5:
        return  tr("Panic");
        break;
    case 6:
         return  tr("Medic");
        break;
    case 7:
        return  tr("Arm");
        break;
    case 8:
        return  tr("Home (Stay)");
        break;
    case 9:
        return  tr("Ready to Arm");
        break;
    case 10:
        return  tr("Exit Open zone is open");
        break;
    case 11:
        return  tr("Exist");
        break;
    case 12:
        return  tr("Reset required");
        break;
    case 13:
        return  tr("No Activity Alert");
        break;
    case 14:
        return  tr("Group 'A' Arm");
        break;
    case 15:
        return  tr("Group 'B' Arm");
        break;
    case 16:
        return  tr("Group 'C' Arm");
        break;
    case 17:
        return  tr("Group 'D' Arm");
        break;
    default:
        break;
    }
}

void  LuxDomoRiscoSecurityData::SetPartitionCondition(int   partitionNumber,QString  condition)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            if(partitionElement.attribute("number")==QString::number(partitionNumber))
            {
                partitionElement.setAttribute("condition",condition);
                QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
                if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    return;
                }
                QTextStream stream( &file1 );
                document.save( stream, 4 );
                file1.close();
                return;
            }
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }
}

void  LuxDomoRiscoSecurityData::SetDefenceAreaCondition(int partitionNumber,int  defenceAreaNumber,QString  condition)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   informationElement;
    securityElement=document.firstChildElement("security");
    informationElement=securityElement.firstChildElement("information");
    if(!informationElement.isNull())
    {
        QDomElement   partitionElement=informationElement.firstChildElement("partition");
        while(!partitionElement.isNull())
        {
            if(partitionElement.attribute("number")==QString::number(partitionNumber))
            {
                QDomElement   defenceAreaElement=partitionElement.firstChildElement("defenceArea");
                while(!defenceAreaElement.isNull())
                {
                    if(defenceAreaElement.attribute("number")==QString::number(defenceAreaNumber))
                    {
                        defenceAreaElement.setAttribute("condition",condition);
                        QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
                        if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                        {
                            return;
                        }
                        QTextStream stream( &file1 );
                        document.save( stream, 4 );
                        file1.close();
                        return;
                    }
                    defenceAreaElement=defenceAreaElement.nextSiblingElement("defenceArea");
                }
            }
            partitionElement=partitionElement.nextSiblingElement("partition");
        }
    }
}

void    LuxDomoRiscoSecurityData::SetOutputCondition(int  number, QString  value)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return ;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return ;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   outputElement;
    securityElement=document.firstChildElement("security");
    outputElement=securityElement.firstChildElement("output");
    if(!outputElement.isNull())
    {
        QDomElement  routeElement=outputElement.firstChildElement("route");
        while(!routeElement.isNull())
        {
            if(routeElement.attribute("number").toInt()==number)
            {
                routeElement.setAttribute("condition",value);
                QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
                if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    return;
                }
                QTextStream stream( &file1 );
                document.save( stream, 4 );
                file1.close();
                return;
            }
            routeElement=routeElement.nextSiblingElement("route");
        }
    }
}

void   LuxDomoRiscoSecurityData::AddRiscoEvent(QString partitionNumber,QString  defenceAreaNumber,QString  condition)
{
    QDomDocument  document;
    QFile   file(XmlFilePath+tr("risco-security-default.xml"));
    if(!file.exists()||file.size()==0)
    {
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
     }
    if(!document.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement   securityElement;
    QDomElement   eventElement;
    securityElement=document.firstChildElement("security");
    eventElement=securityElement.firstChildElement("event");
    if(!eventElement.isNull())
    {
        QDomElement  message=document.createElement("message");
        message.setAttribute("partitionNumber",partitionNumber);
        message.setAttribute("defenceAreaNumber",defenceAreaNumber);
        message.setAttribute("condition",condition);
        QDateTime  current_date_time = QDateTime::currentDateTime();
        QString  current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        message.setAttribute("time",current_date);
        eventElement.appendChild(message);

        QFile   file1(XmlFilePath+tr("risco-security-default.xml"));
        if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
        QTextStream stream( &file1 );
        document.save( stream, 4 );
        file1.close();
        return;
    }
}
