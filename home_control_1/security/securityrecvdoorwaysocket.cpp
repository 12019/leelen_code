#include "securityrecvdoorwaysocket.h"
#include <unistd.h>
#include "maindialog.h"

extern char g_strNativeName[50];
extern MainDialog *g_pMainDlg;
extern bool g_Digital_Channel_Flag;				// 标识数字通道的变量

SecurityRecvDoorwaySocket::SecurityRecvDoorwaySocket(QObject *parent) :
    QTcpSocket(parent)
{
    m_ucTag = 0xff;

    m_sNetNameLen = -1;

    m_strNetName = NULL;

   connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));

   connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));

   connect(this,SIGNAL(sigOut()),g_pMainDlg->m_pSecurityMainDlg->m_pSecurityAbnormalDlg,
           SLOT(on_pushButtonSecAbnormalOut_pressed()));

   connect(this,SIGNAL(sigDisArm(int)),g_pMainDlg->m_pSecurityMainDlg,
           SLOT(setSecurityState(int)));

   printf("SecurityRecvDoorwaySocket\n");
}

void SecurityRecvDoorwaySocket::readClient()
{
    printf("Security readClient\n");
start_jump:
    if (m_ucTag == 0xff)
    {
        read((char *)&m_ucTag, sizeof(unsigned char));
    }
    unsigned char answer =0;//
    if (m_ucTag == (unsigned char)0x80)
    {
        if ((m_sNetNameLen == -1) && (bytesAvailable() >= sizeof(short)))
        {
            read((char *)&m_sNetNameLen, sizeof(short));
            m_strNetName = new char[m_sNetNameLen + 1];
            m_strNetName[0] = '\0';
        }
        if ((m_strNetName[0] == '\0') && (bytesAvailable() >= m_sNetNameLen))
        {
            read((char *)m_strNetName, m_sNetNameLen);

            m_strNetName[m_sNetNameLen] = '\0';

            int sum=0;

            for(int i =0;i < m_sNetNameLen-1;i++)
            {
                sum += m_strNetName[i];
            }
            sum = (-sum)&0xff;

            if(sum == m_strNetName[m_sNetNameLen-1])
            {
                if(m_strNetName[0] == 1)
                {
                    QSettings settings(CONFIG_FILE, QSettings::IniFormat);
                    int state = settings.value("alert_param/security_state").toInt();
                    printf("state:%d\n",state);
                    if(state == 1 || state ==2)
                    {
                        printf("dd\n");
                        answer = 0x01;//撤防
                        write((char *)&answer, sizeof(unsigned char));
                        SecurityMainDialog *pSecurityMainDlg = (SecurityMainDialog *)parent();
                        if (pSecurityMainDlg->m_securityState == STATUS_DISARM) return;
                        QString strWaveFile = "disarm";
                        if (g_pMainDlg->m_inputMethodMode == 0)
                        {
                            strWaveFile += "-en";
                        }
                        strWaveFile = "wav/" + strWaveFile + ".wav";
			if ( g_Digital_Channel_Flag == false )		// 如果是模拟通道需要进行延迟
				usleep(50000);;
                        playWaveFile(strWaveFile.toLocal8Bit().data(), g_pMainDlg->m_pVoipMainDlg->m_nRingVolume, 1);
                        emit sigDisArm(STATUS_DISARM);
                    }
                    else if(state == 0)
                    {
                        answer = 0x02;//设防
                        write((char *)&answer, sizeof(unsigned char));
                        emit sigOut();
                    }
                }
            }
        }
    }

}

SecurityRecvDoorwayServer::SecurityRecvDoorwayServer(QObject *parent)
    :QTcpServer(parent)
{

}

void SecurityRecvDoorwayServer::incomingConnection(int socketId)
{
    printf("SecurityRecvDoorwayServer\n");
    socket = new SecurityRecvDoorwaySocket(this);

    socket->setSocketDescriptor(socketId);
}
