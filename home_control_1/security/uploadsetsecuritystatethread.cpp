#include "uploadsetsecuritystatethread.h"
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "leelen_system.h"
#include "leelen_security.h"
#include "maindialog.h"
#include "getpeeripthread.h"
#include "core/leelen_utils.h"

extern MainDialog *g_pMainDlg;
extern char g_strLphoneName[3][50];
extern RESULT_E g_check_computercenter_udp_state ;//协议探测结果

UploadSetSecurityStateThread::UploadSetSecurityStateThread(QObject *parent) :
    QThread(parent)
{
    m_bRun = false;
}


UploadSetSecurityStateThread::~UploadSetSecurityStateThread()
{

}

void UploadSetSecurityStateThread::run()
{
    m_bRun = true;

    g_check_computercenter_udp_state = RESULT_NULL;

    ((SecurityMainDialog *)parent())->CheckUdpState();

    int time_count = 0;
    while (1)
    {
        usleep(100 * 1000);

        if (g_check_computercenter_udp_state != RESULT_NULL || time_count++ >= 60)
        {
            break;
        }
    }

    stop_system_0x0501();

    if (g_check_computercenter_udp_state == RESULT_SUCCESS)
    {   printf("UdpUploadSetSecurityState..............................\n");
        UdpUploadSetSecurityState();
    }
    else
    {   printf("TcpUploadSetSecurityState.....................\n");
        TcpUploadSetSecurityState();
    }

    m_bRun = false;
}

void UploadSetSecurityStateThread::UdpUploadSetSecurityState()
{
    addr_info_t    addr;
    security_record_t rec;
    char loc_ip[20] = {'\0'};
    char loc_usr[20] = {'\0'};
    char rem_ip[20] = {'\0'};
    char rem_usr[20] = {'\0'};
    char strIP[50] = {'\0'};
    int i;
    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    strcpy(loc_ip, pGetPeerIPThread->m_nativeIP.toLatin1().data());
    strcpy(loc_usr, pGetPeerIPThread->m_nativeName.toLatin1().data());

    if (pGetPeerIPThread->GetPeerIP(g_strLphoneName[2], strIP) != 0)
    {
        return ;
    }
    char *ptr;
    ptr = strstr(strIP, "?");
    if (ptr)
    {
       *ptr = '\0';
    }

    strcpy(rem_ip,strIP);
    strcpy(rem_usr,g_strLphoneName[2]);

    addr.loc_ip = (uint8_t *)loc_ip;
    addr.loc_usr = (uint8_t *)loc_usr;
    addr.rem_ip = (uint8_t *)rem_ip;
    addr.rem_usr = (uint8_t *)rem_usr;

    rec.attrib = 0;
    rec.seq = 1;
    rec.state = 0x00;

    SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

    QSettings settingconf("/mnt/disk/config/defense.conf", QSettings::IniFormat);
    settingconf.setIniCodec(QTextCodec::codecForName("UTF8"));

    QSettings settingdefense("/mnt/disk/config/defense", QSettings::IniFormat);
    settingdefense.setIniCodec(QTextCodec::codecForName("UTF8"));

    QSettings settingconfig(CONFIG_FILE,QSettings::IniFormat);
    settingconfig.setIniCodec(QTextCodec::codecForName("UTF8"));
    int specialDefense;
    specialDefense = settingconfig.value("alert_param/special_defense","0").toInt();

    int DefenseEnable = 0;
    int property;
    for(i = 0; i < 8; i++)
    {
        DefenseEnable = settingdefense.value("wiredefense/defense" + QString::number(i+1),"0").toInt();
        printf ("DefenseEnable %d............................\n",DefenseEnable);
        if(DefenseEnable)
        {
            property = settingconf.value("wiredefense" + QString::number(i+1) + "/property", "-1").toInt();
            if(specialDefense && i==7)
            {
                continue;
            }
            if(pSecurityMainDlg->m_securityState == STATUS_GOOUT)
            {
                rec.state |= 1<<i;
            }
            else if(pSecurityMainDlg->m_securityState == STATUS_ATHOME)
            {
                if(property != DEFENSE_PROPERTY_ATHOME)
                {
                    rec.state |= 1<<i;
                }
            }
            else if(pSecurityMainDlg->m_securityState == STATUS_DISARM)
            {
                if(property == DEFENSE_PROPERTY_NORMAL)
                {
                    rec.state |= 1<<i;
                }
            }
        }
    }

    printf("**********rec.state = %02x..........................\n",rec.state);
    int res = start_security_0x0001(&addr,1,1,&rec, NULL);
    printf("start_security_0x0001 res = %d\n",res);
}

void UploadSetSecurityStateThread::TcpUploadSetSecurityState()
{
    unsigned char tag = 0x03;
    unsigned char cmdFlag = 0x00;
    int length = 18;
    unsigned char version = 0x01;
    unsigned char keyWord = 0xf0;
    unsigned char strNativeName[5] = {0};
    unsigned char tmpstrNativeName[5] = {0};
    unsigned char defenseState[8] = {0};
    unsigned char reserveByte[2] = {0};
    unsigned char checkSum = 0;
    int total = 0;
    int i = 0;
    QTcpSocket tcpSocket;

    char strIP[80];

    GetPeerIPThread *pGetPeerIPThread = g_pMainDlg->m_pVoipMainDlg->m_pGetPeerIPThread;
    QString NativeName = pGetPeerIPThread->m_nativeName;
    if(pGetPeerIPThread->GetPeerIP(g_strLphoneName[2],strIP) != 0)
    {
        printf("can not conenct to manage center software\n");
        return ;
    }

    char *ptr;
    int iModalType = 0;

    ptr = strstr(strIP,"?");

    if(ptr)
    {
        iModalType = atoi(ptr + strlen("?"));
        *ptr = '\0';
    }

    if(iModalType != MANCENTER_MODAL_TYPE)  return ;

    tcpSocket.connectToHost(strIP,17721);
    if(!tcpSocket.waitForConnected(500))
    {
        return ;
    }

    total = (int)(total + tag);
    total = (int)(total + cmdFlag);
    total = (int)(total + length);
    total = (int)(total + version);
    total = (int)(total + keyWord);
    addrtobyte((uint8_t*)NativeName.toLocal8Bit().data(),tmpstrNativeName);
    printf("tmpstrNativeName = %02x%02x%02x%02x%02x\n",tmpstrNativeName[0],tmpstrNativeName[1],tmpstrNativeName[2],tmpstrNativeName[3],tmpstrNativeName[4]);
    for(i = 0; i<5; i++)
    {
        strNativeName[i] = tmpstrNativeName[4-i];
    }
    printf("strNativeName = %02x%02x%02x%02x%02x\n",strNativeName[0],strNativeName[1],strNativeName[2],strNativeName[3],strNativeName[4]);
    if(strNativeName[4] == 0xff)
    {
        strNativeName[4] = 0;
    }
    for(i = 0; i<5; i++)
    {
        total = (int)(total + strNativeName[i]);
    }

    SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();

    QSettings settingconf("/mnt/disk/config/defense.conf", QSettings::IniFormat);
    settingconf.setIniCodec(QTextCodec::codecForName("UTF8"));

    QSettings settingdefense("/mnt/disk/config/defense", QSettings::IniFormat);
    settingdefense.setIniCodec(QTextCodec::codecForName("UTF8"));

    QSettings settingconfig(CONFIG_FILE,QSettings::IniFormat);
    settingconfig.setIniCodec(QTextCodec::codecForName("UTF8"));
    int specialDefense;
    specialDefense = settingconfig.value("alert_param/special_defense","0").toInt();
    //doorDefense8 = settingdefense.value("wiredefense/defense" + QString::number(8)).toInt();//用来判断第8防区当前状态

    int DefenseEnable = 0;
    int property;
    for(i = 0; i < 8; i++)
    {
        DefenseEnable = settingdefense.value("wiredefense/defense" + QString::number(i+1),"0").toInt();
        printf ("DefenseEnable %d............................\n",DefenseEnable);
        if(DefenseEnable)
        {
            property = settingconf.value("wiredefense" + QString::number(i+1) + "/property", "-1").toInt();
            if(specialDefense && i==7)
            {
                continue;
            }
            if(pSecurityMainDlg->m_securityState == STATUS_GOOUT)
            {
                reserveByte[0] = 0;
                defenseState[0] |= 1<<i ;
            }
            else if(pSecurityMainDlg->m_securityState == STATUS_ATHOME)
            {
                reserveByte[0] = 0;
                if(property != DEFENSE_PROPERTY_ATHOME)
                {
                    defenseState[0] |= 1<<i ;
                }
            }
            else if(pSecurityMainDlg->m_securityState == STATUS_DISARM)
            {
                reserveByte[0] = 1;
                if(property == DEFENSE_PROPERTY_NORMAL)
                {
                    defenseState[0] |= 1<<i ;
                }
            }
        }
    }

    total = (int)(total + defenseState[0]);
    total = (int)(total + reserveByte[0]);
    total = (int)(total + reserveByte[1]);

    checkSum = (uint8_t)(0 - (uint8_t)total);
    printf("total....................%02x...................\n",checkSum);
    tcpSocket.write((char *)&tag,sizeof(unsigned char));
    tcpSocket.write((char *)&cmdFlag,sizeof(unsigned char));
    tcpSocket.write((char *)&length,sizeof(int));
    tcpSocket.write((char *)&version,sizeof(unsigned char));
    tcpSocket.write((char *)&keyWord,sizeof(unsigned char));
    tcpSocket.write((char *)strNativeName,sizeof(strNativeName));
    tcpSocket.write((char *)defenseState,sizeof(defenseState));
    tcpSocket.write((char *)reserveByte,sizeof(reserveByte));
    tcpSocket.write((char *)&checkSum,sizeof(unsigned char));
    tcpSocket.flush();


}

























