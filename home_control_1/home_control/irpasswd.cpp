#include "irpasswd.h"
#include<homecontrolmaindlg.h>
#include<maindialog.h>

extern MainDialog *g_pMainDlg;
extern QDeclarativeItem *item;

Irpasswd::Irpasswd(QObject *parent) :
    QObject(parent)
{
    setPasswd(tr("密码编辑框"));
}

Irpasswd::~Irpasswd()
{
    //delete m_pIrLearnDialog;
    //delete m_pIrpasswdQml;
}


void Irpasswd::close()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->setMainState("");
    //pHomeControlDlg->hide();
    setPasswd(tr("密码编辑框"));
    //QMetaObject::invokeMethod(item,"setState",Q_ARG(QVariant, ""));
}


void Irpasswd::loadIrDevice()
{
    HomeControlMainDlg *pHomeControlDlg = (HomeControlMainDlg*)parent();
    pHomeControlDlg->m_phouseMapXmlThread->eventType = irlearn;
    pHomeControlDlg->m_phouseMapXmlThread->start();
}

void Irpasswd::setPwdName(QString str)
{
    if(str == "error")
        setPasswd(tr("密码错误"));
    else if(str == "pwd")
        setPasswd(tr("密码编辑框"));
}

void Irpasswd::Translate()
{
    setPasswd(tr("密码编辑框"));
}

/*******************密码编辑框*********************/
QString Irpasswd::passwd()const
{
    return m_Pwd;
}

void Irpasswd::setPasswd(const QString &passwd)
{
    m_Pwd = passwd;
    emit sig_Passwd();
}



