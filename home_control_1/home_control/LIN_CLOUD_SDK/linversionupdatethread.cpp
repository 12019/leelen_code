#include "linversionupdatethread.h"
#include"homecontrolmaindlg.h"
LinVersionUpdateThread::LinVersionUpdateThread(QObject *parent) : QThread(parent)
{

}

void   LinVersionUpdateThread::run()
{
     HomeControlMainDlg *pHomeControlMainDlg = (HomeControlMainDlg*)parent();
     int       num=0;
     bool    flag=pHomeControlMainDlg->m_pLINCloudDEVSDK->DownloadUpdateFile();
      while(flag==false&&num<3)
      {
              num++;
              flag=pHomeControlMainDlg->m_pLINCloudDEVSDK->DownloadUpdateFile();
          continue;
      }
}
