#ifndef TERMINALHELPTHREAD_H
#define TERMINALHELPTHREAD_H

#include <QThread>
#define SEND_RES_NULL 0
#define SEND_RES_SUCCESS 1
#define SEND_RES_FALSE -1
class TerminalHelpThread : public QThread
{
    Q_OBJECT
public:
    explicit TerminalHelpThread(QObject *parent = 0);

    void TcpTerminalHelp(int destination);
    void UdpTerminalHelp(int destination);
	void run();
    int m_managerHelpResultFlag;  //udp终端求助管理机回调结果标志
    int m_computerHelpResultFlag; //udp终端求助物业中心回调结果标志

signals:
	//void sigSendHelp();
	void sigShowTermHelpResult(QString strInfo);

public slots:

};

#endif // TERMINALHELPTHREAD_H
