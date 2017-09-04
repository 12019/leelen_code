#include "updownloadsocket.h"
#include <QDir>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define min(x, y) ((x) <= (y) ? (x) : (y))

UpDownloadClientSocket::UpDownloadClientSocket(QObject *parent)
	: QTcpSocket(parent)
{
	m_cmd = -1;
	m_folderNameLen = -1;
	m_sFolderName = NULL;
	m_fileNameLen = -1;
	m_sFileName = NULL;
	m_fileDataLen = -1;
	m_sNewFileName = NULL;
	m_newFileNameLen = -1;
	m_sFileData = NULL;
	m_fp = NULL;

	m_ret = 0;

	m_gkCodec = QTextCodec::codecForName("GBK");

	connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

UpDownloadClientSocket::~UpDownloadClientSocket()
{

}


void UpDownloadClientSocket::readClient()
{
	FILE *fp;
	//int ret = 0;
	int bytes_read;

	if (m_cmd == (char)-1)
	{
		if (bytesAvailable() < 1) return;
		read(&m_cmd, 1);
		//printf("lg m_cmd: %d\n", m_cmd);
	}

	if (m_cmd == 1)
	{
		//参数设置
	}
	else if (m_cmd == 2)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen:%d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName:%s\n", m_sFolderName);
		}

		if (m_fileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_fileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sFileName == NULL)
		{
			if (bytesAvailable() < m_fileNameLen) return;

			char sBuf1[256];
			char sBuf2[256];
			int len;

			read(sBuf1, m_fileNameLen);
			sBuf1[m_fileNameLen] = '\0';

			strcpy(sBuf2, (const char *)m_gkCodec->toUnicode(sBuf1).toUtf8().data());

			len = strlen(sBuf2);
			m_sFileName = new char[len + 1];
			memcpy(m_sFileName, sBuf2, len);
			m_sFileName[len] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		if (m_fileDataLen == -1)
		{
			if (bytesAvailable() < 4) return;
			read((char *)&m_fileDataLen, 4);
			//printf("lg m_fileDataLen:%d\n", m_fileDataLen);
		}

		if (m_sFileData == NULL)
		{
			char filePath[512];

			sprintf(filePath, "%s", m_sFolderName);

			mkdir(filePath, S_IRWXU | S_IRWXG | S_IRWXO);

			sprintf(filePath, "%s/%s", m_sFolderName, m_sFileName);

			//printf("lg filePath:%s\n", filePath);

			m_fp = fopen(filePath, "wb");

			m_sFileData = new char[1024];
		}

		while ((m_fileDataLen > 0) && (bytesAvailable() > 0))
		{
			if (m_fileDataLen >= 1024)
			{
				bytes_read = read(m_sFileData, min(1024, bytesAvailable()));
			}
			else
			{
				bytes_read = read(m_sFileData, min(m_fileDataLen, bytesAvailable()));
			}

			m_fileDataLen -= bytes_read;

			if (m_fp)
			{
				int tmp;
				tmp = fwrite(m_sFileData, 1, bytes_read, m_fp);

				if (tmp < bytes_read) m_ret = -1;

				//printf("lg 2 %d tmp:%d bytes_read:%d\n", m_fileDataLen, tmp, bytes_read);
			}
		}

		//printf("lg %d\n", m_fileDataLen);


		if (m_fileDataLen == 0)
		{
			//printf("lg 1: m_fp:%p\n", m_fp);
			if (m_fp == NULL) m_ret = -1;

			write(&m_ret, 1);

			//printf("lg 2:ret:%d\n", m_ret);
			if (m_fp) fclose(m_fp);

			char filePath[512];
			sprintf(filePath, "%s/%s", m_sFolderName, m_sFileName);
			chmod(filePath, S_IRWXU | S_IRWXG | S_IRWXO);

			delete m_sFolderName;
			delete m_sFileName;
			delete m_sFileData;

			//printf("lg 3\n");

			m_cmd = -1;
			m_folderNameLen = -1;
			m_sFolderName = NULL;
			m_fileNameLen = -1;
			m_sFileName = NULL;
			m_fileDataLen = -1;
			m_sFileData = NULL;
			m_newFileNameLen = -1;
			m_sNewFileName = NULL;
			m_fp = NULL;

			m_ret = 0;
		}
	}
	else if (m_cmd == 3)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen:%d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName:%s\n", m_sFolderName);
		}

		if (m_fileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_fileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sFileName == NULL)
		{
			if (bytesAvailable() < m_fileNameLen) return;

			char sBuf1[256];
			char sBuf2[256];
			int len;

			read(sBuf1, m_fileNameLen);
			sBuf1[m_fileNameLen] = '\0';

			strcpy(sBuf2, (const char *)m_gkCodec->toUnicode(sBuf1).toUtf8().data());

			len = strlen(sBuf2);
			m_sFileName = new char[len + 1];
			memcpy(m_sFileName, sBuf2, len);
			m_sFileName[len] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		char filePath[512];

		sprintf(filePath, "%s/%s", m_sFolderName, m_sFileName);

		//printf("lg filePath:%s\n", filePath);

		fp = fopen(filePath, "rb");

		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			m_fileDataLen = ftell(fp);

			write((char *)&m_fileDataLen, 4);
			//printf("lg m_fileDataLen:%d\n", m_fileDataLen);

			m_sFileData = new char[1024];

			fseek(fp, 0, SEEK_SET);

			while (m_fileDataLen > 0)
			{
				int tmp;
				tmp = fread(m_sFileData, 1, 1024, fp);
				write(m_sFileData, tmp);
				m_fileDataLen -= tmp;
			}

			fclose(fp);
		}
		else
		{
			m_fileDataLen = -1;

			write((char *)&m_fileDataLen, 4);
		}

		delete m_sFolderName;
		delete m_sFileName;
		delete m_sFileData;

		m_cmd = -1;
		m_folderNameLen = -1;
		m_sFolderName = NULL;
		m_fileNameLen = -1;
		m_sFileName = NULL;
		m_fileDataLen = -1;
		m_sFileData = NULL;
		m_newFileNameLen = -1;
		m_sNewFileName = NULL;
		m_fp = NULL;

		m_ret = 0;
	}
	else if (m_cmd == 4)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen:%d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName:%s\n", m_sFolderName);
		}

		if (m_fileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_fileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sFileName == NULL)
		{
			if (bytesAvailable() < m_fileNameLen) return;

			char sBuf1[256];
			char sBuf2[256];
			int len;

			read(sBuf1, m_fileNameLen);
			sBuf1[m_fileNameLen] = '\0';

			strcpy(sBuf2, (const char *)m_gkCodec->toUnicode(sBuf1).toUtf8().data());

			len = strlen(sBuf2);
			m_sFileName = new char[len + 1];
			memcpy(m_sFileName, sBuf2, len);
			m_sFileName[len] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		char filePath[512];
		char cmd[512];
		sprintf(filePath, "%s/%s", m_sFolderName, m_sFileName);

		sprintf(cmd, "/bin/rm -rf %s", filePath);
		system(cmd);

		if (access(filePath, F_OK) == 0)
		{
			m_ret = -1;
		}
		else
		{
			m_ret = 0;
		}

		write(&m_ret, 1);

		delete m_sFolderName;
		delete m_sFileName;

		m_cmd = -1;
		m_folderNameLen = -1;
		m_sFolderName = NULL;
		m_fileNameLen = -1;
		m_sFileName = NULL;
		m_fileDataLen = -1;
		m_sFileData = NULL;
		m_newFileNameLen = -1;
		m_sNewFileName = NULL;
		m_fp = NULL;

		m_ret = 0;
	}
	else if (m_cmd == 5)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen:%d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName:%s\n", m_sFolderName);
		}

		if (m_fileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_fileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sFileName == NULL)
		{
			if (bytesAvailable() < m_fileNameLen) return;

			m_sFileName = new char[m_fileNameLen + 1];
			read(m_sFileName, m_fileNameLen);
			m_sFileName[m_fileNameLen] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		char filePath[512];
		sprintf(filePath, "%s/%s", m_sFolderName, m_sFileName);

		if (mkdir(filePath, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
		{
			m_ret = 0;
		}
		else
		{
			m_ret = -1;
		}

		write(&m_ret, 1);

		delete m_sFolderName;
		delete m_sFileName;

		m_cmd = -1;
		m_folderNameLen = -1;
		m_sFolderName = NULL;
		m_fileNameLen = -1;
		m_sFileName = NULL;
		m_fileDataLen = -1;
		m_sFileData = NULL;
		m_newFileNameLen = -1;
		m_sNewFileName = NULL;
		m_fp = NULL;

		m_ret = 0;
	}
	else if (m_cmd == 6)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen:%d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName:%s\n", m_sFolderName);
		}

		if (m_fileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_fileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sFileName == NULL)
		{
			if (bytesAvailable() < m_fileNameLen) return;

			m_sFileName = new char[m_fileNameLen + 1];
			read(m_sFileName, m_fileNameLen);
			m_sFileName[m_fileNameLen] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		if (m_newFileNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_newFileNameLen, 2);
			//printf("lg m_fileNameLen:%d\n", m_fileNameLen);
		}

		if (m_sNewFileName == NULL)
		{
			if (bytesAvailable() < m_newFileNameLen) return;

			m_sNewFileName = new char[m_newFileNameLen + 1];
			read(m_sNewFileName, m_newFileNameLen);
			m_sNewFileName[m_newFileNameLen] = '\0';
			//printf("lg m_sFileName:%s\n", m_sFileName);
		}

		char filePathOld[512];
		char filePathNew[512];
		sprintf(filePathOld, "%s/%s", m_sFolderName, m_sFileName);
		sprintf(filePathNew, "%s/%s", m_sFolderName, m_sNewFileName);

		if (rename(filePathOld, filePathNew) == 0)
		{
			m_ret = 0;
		}
		else
		{
			m_ret = -1;
		}

		write(&m_ret, 1);

		delete m_sFolderName;
		delete m_sFileName;
		delete m_sNewFileName;

		m_cmd = -1;
		m_folderNameLen = -1;
		m_sFolderName = NULL;
		m_fileNameLen = -1;
		m_sFileName = NULL;
		m_fileDataLen = -1;
		m_sFileData = NULL;
		m_newFileNameLen = -1;
		m_sNewFileName = NULL;
		m_fp = NULL;

		m_ret = 0;
	}
	else if (m_cmd == 7)
	{
		if (m_folderNameLen == -1)
		{
			if (bytesAvailable() < 2) return;
			read((char *)&m_folderNameLen, 2);
			//printf("lg m_folderNameLen: %d\n", m_folderNameLen);
		}

		if (m_sFolderName == NULL)
		{
			if (bytesAvailable() < m_folderNameLen) return;

			m_sFolderName = new char[m_folderNameLen + 1];
			read(m_sFolderName, m_folderNameLen);
			m_sFolderName[m_folderNameLen] = '\0';
			//printf("lg m_sFolderName: %s\n", m_sFolderName);
		}

		DIR *dp;
		struct dirent *dirp;
		char path[512];
		short fileCnt = 0;
		short fileCnt2 = 0;
		sprintf(path, "%s", m_sFolderName);

		if ((dp = opendir(path)) == NULL)
		{
			write((char *)&fileCnt, 2);
			goto error_jump;
		}

		while ((dirp = readdir(dp)) != NULL)
		{
			fileCnt++;
		}
		closedir(dp);

		//printf("lg fileCnt: %d\n", fileCnt);

		if ((dp = opendir(path)) == NULL)
		{
			fileCnt = 0;
			write((char *)&fileCnt, 2);
			goto error_jump;
		}

		write((char *)&fileCnt, 2);

		while ((dirp = readdir(dp)) != NULL)
		{
			short file_attr = 0;

			//if (dirp->d_type & DT_DIR)
			//{
			//	file_attr |= 1;
			//}
			char sBuf[256];
			QString str = QString(dirp->d_name);

			//printf("lg %s %d\n", str.toUtf8().data(), str.toUtf8().length());
			strcpy(sBuf, m_gkCodec->fromUnicode(str).data());

			m_fileNameLen = strlen(sBuf);
			write((char *)&m_fileNameLen, 2);
			write(sBuf, m_fileNameLen);

			sprintf(path, "%s/%s", m_sFolderName, dirp->d_name);

			struct stat buf;
			int file_len = 0;
			if (stat(path, &buf) >= 0)
			{
				if (!S_ISDIR(buf.st_mode))
				{
					file_len = buf.st_size;
				}
				else
				{
					file_attr |= 1;
				}
			}

			//printf("lg %s %08X\n", dirp->d_name, (int)buf.st_mode);

			write((char *)&file_attr, 2);
			write((char *)&file_len, 4);

			if (++fileCnt2 >= fileCnt) break;
		}
		closedir(dp);

		for (; fileCnt2 < fileCnt; fileCnt2++)
		{
			char sBuf[2];
			m_fileNameLen = 0;
			write((char *)&m_fileNameLen, 2);
			write(sBuf, m_fileNameLen);

			short file_attr = 0;
			int file_len = 0;

			write((char *)&file_attr, 2);
			write((char *)&file_len, 4);
		}

error_jump:
		delete m_sFolderName;

		m_cmd = -1;
		m_folderNameLen = -1;
		m_sFolderName = NULL;
		m_fileNameLen = -1;
		m_sFileName = NULL;
		m_fileDataLen = -1;
		m_sFileData = NULL;
		m_newFileNameLen = -1;
		m_sNewFileName = NULL;
		m_fp = NULL;

		m_ret = 0;
	}
}

UpDownloadServer::UpDownloadServer(QObject *parent)
	: QTcpServer(parent)
{

}

void UpDownloadServer::incomingConnection(int socketId)
{
	UpDownloadClientSocket *socket = new UpDownloadClientSocket(this);
	//printf("lg ParamSetServer::incomingConnection\n");
	socket->setSocketDescriptor(socketId);
}
