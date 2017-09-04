#include "luxdomoclient.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include"maindialog.h"
#include"MyBuffer.h"

LuxdomoClient::LuxdomoClient(QObject *parent) : QObject(parent)
{
    m_socketfd=-1;
    m_port=-1;
    m_addr=tr("");
}

void  LuxdomoClient::SendData(char *data, int  len)
{
    ::send(m_socketfd,data,len,0);
    QByteArray Array;
    Array.resize(len);
    memcpy(Array.data(),data,len);
    MyMsgLog<<"SendToClient"<<m_addr<<Array.toHex();
}
