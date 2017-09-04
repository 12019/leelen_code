#ifndef LUXDOMOMAIL_H
#define LUXDOMOMAIL_H

#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

class LuxDomoMail : public QObject
{
    Q_OBJECT
public:
    explicit LuxDomoMail(QObject *parent = 0);
    int  getRcvFromHost(); //服务器的反馈信息
    int  base64_encode(char*, int , char*, int ); //base64加密
    int  connectHost(const char*);//连接服务器
    int  loginToHost(char*,char*);//登陆服务器
    int  sendEmail(char*,char*,char*,const char*);
    int  pSmtpMailContext(const  char *);
signals:
public slots:
private:
    int sockfd;   //网络句柄
};

#endif // LUXDOMOMAIL_H
