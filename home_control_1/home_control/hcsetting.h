#ifndef HCSETTING_H
#define HCSETTING_H

#include <QObject>

class HCSetting : public QObject
{
    Q_OBJECT
public:
    explicit HCSetting(QObject *parent = 0);

    Q_INVOKABLE void setCloundIpText(QString strIP);    //服务器IP设置
    Q_INVOKABLE void setCloundIDText(QString strID);   //服务器ID设置
    Q_INVOKABLE void setCloundAccountText(QString strAcc);   //服务器账号设置
    Q_INVOKABLE void setCloundPWDText(QString strPWD);   //服务器密码设置
    Q_INVOKABLE void setTerAccountText(QString strAcc);
    Q_INVOKABLE void setTerPwdText(QString strPwd);
    Q_INVOKABLE void setMainHide();
    Q_INVOKABLE void serialdetect();
    Q_INVOKABLE QString serialInfo();
    Q_INVOKABLE QString getSettingNameText();
    Q_INVOKABLE QString getTerAccountText();
    Q_INVOKABLE QString getTerPwdText();

    Q_INVOKABLE QString getSvrIPText();
    Q_INVOKABLE QString getSvrAccText();
    Q_INVOKABLE QString getSvrPwdText();
    Q_INVOKABLE QString getSvrIDText();

    Q_INVOKABLE QString getSvrSetLabelText();
    Q_INVOKABLE QString getTerSetLabelText();
    Q_INVOKABLE QString getSerialLabelText();
    Q_INVOKABLE QString getHomeLabelText();

     Q_INVOKABLE QString getHomeVersionText();
     Q_INVOKABLE QString getNewVersionText();
     Q_INVOKABLE QString getUpdateVersionText();
     Q_INVOKABLE bool      getIsUpdate();
     Q_INVOKABLE void      downloadNewVersion();

    Q_INVOKABLE QString getCloseLabelText();
    Q_INVOKABLE QString getFinishLabelText();

    Q_PROPERTY(QString lin_CloudIP READ getlin_CloudIP WRITE setlin_CloudIP NOTIFY sig_lin_CloudIP)                    //服务器IP
    Q_PROPERTY(QString lin_LocalID READ getlin_LocalID WRITE setlin_LocalID NOTIFY sig_lin_LocalID)                    //服务器ID
    Q_PROPERTY(QString lin_CloudAcc READ getlin_CloudAcc WRITE setlin_CloudAcc NOTIFY sig_lin_CloudAcc)                    //服务器账号
    Q_PROPERTY(QString lin_CloudPwd READ getlin_CloudPwd WRITE setlin_CloudPwd NOTIFY sig_lin_CloudPwd)                    //服务器密码
    Q_PROPERTY(QString lin_LocalID READ getlin_LocalID WRITE setlin_LocalID NOTIFY sig_lin_LocalID)                     //服务器密码
    Q_PROPERTY(QString terAccount READ getTerAccount WRITE setTerAccount NOTIFY sig_TerAccount)     //终端机远程账号
    Q_PROPERTY(QString terPwd READ getTerPwd WRITE setTerPwd NOTIFY sig_TerPwd)      //终端机密码



    QString getlin_CloudIP(void) const;
    void setlin_CloudIP(const QString& stat);

    QString getlin_CloudAcc(void) const;
    void setlin_CloudAcc(const QString& stat);

    QString getlin_CloudPwd(void) const;
    void setlin_CloudPwd(const QString& stat);

    QString getlin_LocalID(void) const;
    void setlin_LocalID(const QString& stat);

    QString getTerAccount(void) const;
    void setTerAccount(const QString& stat);

    QString getTerPwd(void) const;
    void setTerPwd(const QString& stat);

    void initInfo();
    QString showInputMethodDlg(QString str);

signals:
    void sig_lin_CloudIP(void);
    void sig_lin_CloudAcc(void);
    void sig_lin_CloudPwd(void);
    void sig_lin_LocalID(void);
    void sig_TerAccount(void);
    void sig_TerPwd(void);
public slots:
private:
    QString mLIN_CLOUD_IP;
    QString mLIN_CLOUD_ID;
    QString mLIN_CLOUD_Account;
    QString mLIN_CLOUD_Pwd;

    QString TER_ACCOUNT;
    QString TER_PWD;
};

#endif // HCSETTING_H
