#include "luxdomocheckwlan.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern  MainDialog *g_pMainDlg;
extern  OperateWlan    operateWlan;

LuxDomoCheckWlan::LuxDomoCheckWlan(QThread *parent) : QThread(parent)
{

}

void   LuxDomoCheckWlan::run()
{
    while(1)
    {
        msleep(3000);
        if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning()==false
                &&g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->GetWlanCondition()==true
                &&g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->WlanOn_off()==true)
        {
            system("route -n >  /mnt/disk/home_control/wifiRoute.txt");
            QFile file("/mnt/disk/home_control/wifiRoute.txt");
            if(!file.exists()) {  continue;    }
            if (!file.open(QIODevice::ReadOnly))  {  continue; }
            QTextStream myStream(&file);
            QString myString;
            QStringList  list;
            while(myStream.atEnd() == 0)
            {
                //MyMsgLog<<myString;
                list.clear();
                myString=myStream.readLine();
                list=myString.split(QRegExp("\\s+"));
                if(list.size()!=8){ continue; }
                if(list.at(7)==tr("wlan0")&&list.at(1)!=tr("0.0.0.0"))
                {
                    //MyMsgLog<<list.at(1)<<"ping ping ping ping";
                    QString  str=tr("ping  -c  2  ")+list.at(1)+tr("  -w 4  > /mnt/disk/home_control/wifiPing.txt");
                    system(str.toLatin1().data());
                    QFile file1("/mnt/disk/home_control/wifiPing.txt");
                    if(!file1.exists()) {  break;    }
                    if (!file1.open(QIODevice::ReadOnly))  {  break; }
                    QTextStream myStream1(&file1);
                    myString=myStream1.readLine();
                    myString=myStream1.readLine();
                    if(myString.contains("time"))
                    {
                        break;
                    }
                    file1.close();
                    system("rm  /mnt/disk/home_control/wifiPing.txt");

                    if(g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->isRunning()==false
                            &&g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->GetWlanCondition()==true
                            &&g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->WlanOn_off()==true)
                    {
                        QStringList  data=g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->GetWifiConf();
                        if(data.size()==6)
                        {
                            g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->SetData(data.at(0),data.at(1),data.at(2),data.at(3),data.at(4),data.at(5));
                            operateWlan=LinkWifi;
                            g_pMainDlg->m_pHomeControlDlg->m_pDealWlanData->start();
                        }
                        break;
                    }
                }
            }
            file.close();
            system("rm /mnt/disk/home_control/wifiRoute.txt");
        }
    }
}
