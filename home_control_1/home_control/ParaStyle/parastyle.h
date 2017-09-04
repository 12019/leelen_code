#ifndef PARASTYLE_H
#define PARASTYLE_H

#include <QObject>
#include<MyBuffer.h>

class ParaStyle : public QObject
{
    Q_OBJECT
public:
    explicit ParaStyle(QObject *parent = 0);
    Q_INVOKABLE QString getLabelText(QString type);
    Q_INVOKABLE void setDeveloperPwdText(QString arg);

    //Q_PROPERTY(QString developerPwd READ getDeveloperPwd WRITE setDeveloperPwd NOTIFY sig_DeveloperPwd)      //开发者选项密码
    Q_PROPERTY(QString userCode READ userCode WRITE setUserCode NOTIFY sig_UserCode)
    Q_PROPERTY(QString devName READ devName WRITE setDevName NOTIFY sig_DevName)
    Q_PROPERTY(QString devAddr READ devAddr WRITE setDevAddr NOTIFY sig_DevAddr)






    /*******************************************************************
     函数名：userCode
     描述：显示开发者密码，把值传回ｑｍｌ文件里
     参数：
            _userCode:要传回的值
            userCode:qml中直接调用
     函数名：setUserCode
     描述：调用输入法后把值传到ｄｅｖｅｌｏｐｅｒ
     * ******************************************************************/
    QString userCode() const
    {
        return _userCode;
    }
    void setUserCode(const QString &name)
    {
        _userCode = name;
        emit sig_UserCode();
    }

    /*******************************************************************
     函数名：devName
     描述：显示设备名称
     参数：
            _DevName:要传回的值
            DevName:qml中直接调用
     函数名：setDevName
     描述：调用输入法后把值传到ｄｅｖｅｌｏｐｅｒ
     * ******************************************************************/
    QString devName() const
    {
        return _DevName;
    }
    void setDevName(const QString &name)
    {
        _DevName = name;
        emit sig_DevName();
    }

    /*******************************************************************
     函数名：devAddr
     描述：显示设备地址
     参数：
            _DevAddr:要传回的值
            DevAddr:qml中直接调用
     函数名：setDevAddr
     描述：调用输入法后把值传到ｄｅｖｅｌｏｐｅｒ
     * ******************************************************************/
    QString devAddr() const
    {
        return _DevAddr;
    }
    void setDevAddr(const QString &name)
    {
        MyMsgLog<<"TTTTTTTTTTTTTTTTTTTTTTTTT";
        _DevAddr = name;
        emit sig_DevAddr();
    }

signals:
    void sig_UserCode(void);
    void sig_DevName(void);
    void sig_DevAddr(void);
public slots:
private:
    QString _userCode;
    QString _DevName;
    QString _DevAddr;
};

#endif // PARASTYLE_H
