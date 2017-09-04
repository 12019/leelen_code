#include "maindialog.h"
#include "netrecvsocket.h"

/************************************************************
  该文件为网络接收输入(如接收发布的信息)的主程序
************************************************************/

/************************************************************
描述：网络接收Socket构造函数
参数：parent - 该Socket的父对象指针
返回：无
************************************************************/
NetRecvClientSocket::NetRecvClientSocket(QObject *parent)
	: QTcpSocket(parent)
{
	m_RequestFrame.PackageFlag = 0;
	m_RequestFrame.PackageLength.PackageLength32 = 0xffffffff;
	m_RequestFrame.CmdType = 0xff;
	m_PackageLen = 0;
	m_Buf = NULL;
	m_BufReadLen = 0;

	connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

NetRecvClientSocket::~NetRecvClientSocket()
{
	if (m_Buf) free(m_Buf);
}


/************************************************************
描述：网络接收Socket接收到数据后的处理函数
参数：无
返回：无
************************************************************/
void NetRecvClientSocket::readClient()
{
	int bytes_cnt;;
	MainDialog *pMainDlg = (MainDialog *)(parent()->parent());

_START_RECV:
	if (m_RequestFrame.PackageFlag == 0)
	{
		read((char *)&m_RequestFrame.PackageFlag, sizeof(uint8_t));
    }

	if (m_RequestFrame.PackageLength.PackageLength32 == 0xffffffff)
    {
		switch (m_RequestFrame.PackageFlag)
		{
		case PACKAGE_FLAG_UINT8:
			if (bytesAvailable() < 1) return;
			read((char *)&m_PackageLen, sizeof(uint8_t));
			m_RequestFrame.PackageLength.PackageLength8 = (uint8_t)m_PackageLen;
			m_PackageLen -= 1 + 1;
            //printf("lg length:%02x\n", m_RequestFrame.PackageLength.PackageLength8);
			break;
		case PACKAGE_FLAG_UINT32:
			if (bytesAvailable() < 4) return;
			read((char *)&m_PackageLen, sizeof(uint32_t));
			m_RequestFrame.PackageLength.PackageLength32 = m_PackageLen;
            m_PackageLen -= 1 + 4;
			break;
		default:
            /*MyMessageBox messBox;
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			//Q_UNUSED(okButton);
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("收到不支持的FLAG类型"));
			messBox.exec();
            */
            close();
			break;
		}
	}

	if (m_RequestFrame.CmdType == 0xff)
	{
		if (bytesAvailable() < 1) return;
		read((char *)&m_RequestFrame.CmdType, sizeof(uint8_t));
        //printf("lg cmd_type:%02x\n", m_RequestFrame.CmdType);
		m_PackageLen--;

		if (m_Buf) free(m_Buf);
		m_Buf = (char *)malloc(m_PackageLen);
	}

	if ((bytes_cnt = bytesAvailable()) < m_PackageLen - m_BufReadLen)
	{
		read(m_Buf + m_BufReadLen, bytes_cnt);
		m_BufReadLen += bytes_cnt;
	}
	else
	{
		int b_AnotherFrame = 0;
		int ret = 0;

		b_AnotherFrame = bytes_cnt > (m_PackageLen - m_BufReadLen);

		read(m_Buf + m_BufReadLen, m_PackageLen - m_BufReadLen);
        m_BufReadLen += bytes_cnt;

		switch (m_RequestFrame.CmdType)
		{
		case CMD_TYPE_INFO_SEND:
			{
				ret = pMainDlg->m_pInfoManMainDlg->InfoReceive(this, m_Buf, m_PackageLen);

				RESPONSE_FRAME_T response_frame;

				response_frame.Result = (ret == 0) ? 1 : 2;
				response_frame.PackageFlag = PACKAGE_FLAG_UINT8;
				response_frame.PackageLength.PackageLength8 = 3;

				write((char *)&response_frame.PackageFlag, sizeof(uint8_t));
				write((char *)&response_frame.PackageLength.PackageLength8, sizeof(uint8_t));
				write((char *)&response_frame.Result, sizeof(uint8_t));

				flush();
			}
			break;
		case CMD_TYPE_HOME_CONTROL:
			{
                /*if(!pMainDlg->m_pHomeControlSceneDlg->m_bMutex)
                {
                    ret = 0;
                }
                else
                {
                    ret =-1;
                }

				char response[5] = {0xFF, 0x05, 0xF0, 0x00, 0x00};
				if (ret == 0)
				{
					response[4] = 0x01;
				}
				else if (ret == -2)
				{
					response[4] = 0x02;
				}
				else
				{
					response[4] = 0x00;
				}

				write(response, 5);

                flush();

                close();

                if(!pMainDlg->m_pHomeControlSceneDlg->m_bMutex)
                {
                    ret = pMainDlg->m_pHomeControlSceneDlg->homeControlReceive(this,m_Buf,m_PackageLen);
                }*/
            }
			break;
        case CMD_TYPE_REMOTECHECK:
             {
                    char username[9 + 1] = "1111-2222";

                    char password[6 + 1] = "666666";

                    if (m_PackageLen == 15)
                    {
                        memcpy(username, m_Buf + 1, 4);
                        username[4] = '-';
                        memcpy(username + 5, m_Buf + 1 + 4, 4);
                        username[9] = '\0';

                        memcpy(password, m_Buf + 1 + 8, 6);

                        password[6] = '\0';
                     }
                    else
                    {
                        //如果不等于15则不正确
                        char ret[5];

                        ret[0] = 0xff;
                        ret[1] = 0x05;
                        ret[2] = 0xf1;
                        ret[3] = 0x00;
                        ret[4] = 0x00;

                        write(ret, 5);
                        flush();
                        break;
                    }

                    QSettings Settings(CONFIG_FILE, QSettings::IniFormat);

                    Settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

                    QString username_get = Settings.value("system/native_name", "1111-2222").toString();

                    QString password_get = Settings.value("system/remotecheckpassword", "666666").toString();

                    if ((username == username_get) && (password == password_get))
                    {
                        //0xFF  0x05  0xF1 表示为远程密码验证协议  0x00 保留
                        //0x01 表示验证成功
                        //0x00 表示验证失败
                        char retu[5];

                        retu[0] = 0xff;
                        retu[1] = 0x05;
                        retu[2] = 0xf1;
                        retu[3] = 0x00;
                        retu[4] = 0x01;

                        write(retu, 5);
                        flush();
                    }
                    else
                    {
                        char retu[5];

                        retu[0] = 0xff;
                        retu[1] = 0x05;
                        retu[2] = 0xf1;
                        retu[3] = 0x00;
                        retu[4] = 0x00;

                       write(retu, 5);
                        flush();
                    }
                }
                break;
		default:
            /*MyMessageBox messBox;
			CommonPushButton *okButton = messBox.addButton(QObject::tr("确定"));
			//Q_UNUSED(okButton);
			okButton->setFocus(Qt::TabFocusReason);
			messBox.setText(tr("收到不支持的CMD类型"));
            messBox.exec();*/
			break;
		}

		free(m_Buf);

		m_RequestFrame.PackageFlag = 0;
		m_RequestFrame.PackageLength.PackageLength32 = 0xffffffff;
		m_RequestFrame.CmdType = 0xff;
		m_PackageLen = 0;
		m_Buf = NULL;
		m_BufReadLen = 0;

        //david 2014.5.23
        if(m_RequestFrame.CmdType == CMD_TYPE_HOME_CONTROL){
            return;
        }

		if (b_AnotherFrame)
		{
			goto _START_RECV;
		}
	}
}

/************************************************************
描述：网络监听服务器Socket构造函数
参数：parent - 该Socket的父对象指针
返回：无
************************************************************/
NetRecvServer::NetRecvServer(QObject *parent)
	: QTcpServer(parent)
{

}

/************************************************************
描述：网络监听服务器Socket接收到socket连接的回调函数，负责打开网络接
	 收Socket
参数：socketId - 网络接收Socket的ID号
返回：无
************************************************************/
void NetRecvServer::incomingConnection(int socketId)
{
	NetRecvClientSocket *socket = new NetRecvClientSocket(this);
	printf("bcom NetRecvServer::incomingConnection\n");
	socket->setSocketDescriptor(socketId);
}
