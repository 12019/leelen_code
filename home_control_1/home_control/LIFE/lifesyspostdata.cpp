#include "lifesyspostdata.h"

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

LifeSysPostData::LifeSysPostData(QObject *parent) :
    QObject(parent)
{
    m_prequest=new QNetworkRequest;
    m_pmanager=new QNetworkAccessManager;
    connect(m_pmanager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
}

LifeSysPostData::~LifeSysPostData()
{
    delete m_pmanager;
    delete m_preply;
    delete m_prequest;
}

void LifeSysPostData::postData(QByteArray strData)   /***生命检测数据发送***/
{
    strData=getBase64Code(QString(strData)).toLatin1();   //base64加密
    m_pmanager->post(*m_prequest,strData);
}

void LifeSysPostData::replyFinished(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"the statuscode is"<<status_code.toInt();

    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"postData success!";
    }
    else
    {
        qDebug()<<"postData faile";
    }
    delete reply;
}

void LifeSysPostData::postDataInit()
{
    QString url="http://rlms.ttdc.com.tw/DeviceData/handlePayload/";
    QList<QNetworkInterface>list=QNetworkInterface::allInterfaces();
    QString useragent="TTDC-"+list.at(1).hardwareAddress();
    m_prequest->setUrl(QUrl(url));
    m_prequest->setRawHeader("HOST","RLMS.TTDC.COM.TW");
    m_prequest->setRawHeader("User-Agent",useragent.toStdString().c_str());
    m_prequest->setRawHeader("Accept","text/plain");
    m_prequest->setRawHeader("Accept-Language","en-US");
    m_prequest->setRawHeader("Accept-Encoding","identity");
    m_prequest->setRawHeader("Accept-Charset","ASCII");
    m_prequest->setRawHeader("Connection","Keep-Alive");
    m_prequest->setRawHeader("Cache-control","no-cache");
    m_prequest->setRawHeader("Content-Length","1000");
    m_prequest->setRawHeader("Content-Type","Application/Octet-stream");
    m_prequest->setRawHeader("Content-Encoding","BASE64");

    qDebug()<<useragent.toStdString().c_str();
}

char *LifeSysPostData::base64_Encode(unsigned char *data, int data_len) //base64加密
{
    int prepare = 0;
    int ret_len;
    int temp = 0;
    char *ret = NULL;
    char *f = NULL;
    int tmp = 0;
    char changed[4];
    int i = 0;
    ret_len = data_len / 3;
    temp = data_len % 3;
    if (temp > 0)
    {
        ret_len += 1;
    }
    ret_len = ret_len*4 + 1;//每三个字符转换为四个字符
    ret = (char *)malloc(ret_len);
    if ( ret == NULL)
    {
        printf("No enough memory.\n");
        exit(0);
    }
    memset(ret, 0, ret_len);
    f = ret;
    while (tmp < data_len)
    {
        temp = 0;
        prepare = 0;
        memset(changed,'\0', 4); //清零
        while (temp < 3)
        {
            if (tmp >= data_len)
            {
                break;
            }
            prepare = ((prepare << 8) | (data[tmp] & 0xFF));
            tmp++;
            temp++;
        }
        prepare = (prepare<<((3-temp)*8));
        for (i = 0; i < 4 ;i++ )
        {
            if (temp < i)
            {
                changed[i] = 0x40;
            }
            else
            {
                changed[i] = (prepare>>((3-i)*6)) & 0x3F;
            }
            *f = base[changed[i]];
            f++;
        }
    }
    *f = '\0';
    qDebug()<<"EnpostData:"<<ret;
    return ret;
}

QString LifeSysPostData::getBase64Code(QString str)
{
    QString Enstr=str.replace(" ","");//去掉空格
    int count=Enstr.count()/2;
    unsigned char Hex[count];
    for(int i=0;i<count;i++)
    {
        Hex[i]=Enstr.left(2).toInt(0,16);
        Enstr.remove(0,2);
    }//把Qstring转成十六进制
    qDebug()<<"postdata:"<<str;
    return base64_Encode(Hex,count);
}














