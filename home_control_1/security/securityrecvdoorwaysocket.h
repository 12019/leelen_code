#ifndef SECURITYRECVDOORWAYSOCKET_H
#define SECURITYRECVDOORWAYSOCKET_H

#include <Qt/qtcpsocket.h>
#include <Qt/qtcpserver.h>
#include <QDateTime>


class SecurityRecvDoorwaySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit SecurityRecvDoorwaySocket(QObject *parent = 0);

    unsigned char m_ucTag;

    short m_sNetNameLen;

    char *m_strNetName;

    char *m_strDateTime;

    short m_sDataTimeLen;

    QDateTime m_currDateTime;

signals:
    void sigOut();

    void sigDisArm(int);

public slots:
    void readClient();
};

class SecurityRecvDoorwayServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SecurityRecvDoorwayServer(QObject *parent = 0);
    SecurityRecvDoorwaySocket *socket;
private:
    void incomingConnection(int socketId);

public slots:

};
#endif // SECURITYRECVDOORWAYSOCKET_H
