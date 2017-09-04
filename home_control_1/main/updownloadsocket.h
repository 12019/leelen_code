#ifndef UPDOWNLOADSOCKET_H
#define UPDOWNLOADSOCKET_H

#include <Qt/qtcpsocket.h>
#include <Qt/qtcpserver.h>
#include <stdio.h>
#include <QTextCodec>

class UpDownloadClientSocket : public QTcpSocket
{
	Q_OBJECT
public:
	UpDownloadClientSocket(QObject *parent = 0);
	~UpDownloadClientSocket();

	char m_cmd;
	short m_folderNameLen;
	char *m_sFolderName;
	short m_fileNameLen;
	char *m_sFileName;
	int m_fileDataLen;
	char *m_sFileData;
	short m_newFileNameLen;
	char *m_sNewFileName;

	FILE *m_fp;

	char m_ret;

	QTextCodec *m_gkCodec;

private slots:
	void readClient();
};

class UpDownloadServer : public QTcpServer
{
	Q_OBJECT
public:
	UpDownloadServer(QObject *parent = 0);

private:
	void incomingConnection(int socketId);
};

#endif // UPDOWNLOADSOCKET_H
