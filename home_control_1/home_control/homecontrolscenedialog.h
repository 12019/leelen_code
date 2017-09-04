#ifndef HOMECONTROLSCENEDIALOG_H
#define HOMECONTROLSCENEDIALOG_H

#include <QObject>
#include<QtDeclarative/QDeclarativeView>
#include<QDeclarativeEngine>
#include<QDeclarativeComponent>
#include<QDeclarativeItem>
#include<QDeclarativeView>
#include<QDeclarativeContext>
#include<homecontrolsceneedit.h>


class HomeControlSceneDialog : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString s1text READ s1text WRITE setS1text NOTIFY sig_S1text)   //场景1名称
    Q_PROPERTY(QString s2text READ s2text WRITE setS2text NOTIFY sig_S2text)   //场景2名称
    Q_PROPERTY(QString s3text READ s3text WRITE setS3text NOTIFY sig_S3text)   //场景3名称
    Q_PROPERTY(QString s4text READ s4text WRITE setS4text NOTIFY sig_S4text)   //场景4名称
    Q_PROPERTY(QString s5text READ s5text WRITE setS5text NOTIFY sig_S5text)   //场景5名称
    Q_PROPERTY(QString s6text READ s6text WRITE setS6text NOTIFY sig_S6text)   //场景6名称
    Q_PROPERTY(QString s7text READ s7text WRITE setS7text NOTIFY sig_S7text)   //场景7名称
    Q_PROPERTY(QString s8text READ s8text WRITE setS8text NOTIFY sig_S8text)   //场景8名称
    Q_PROPERTY(QString s9text READ s9text WRITE setS9text NOTIFY sig_S9text)   //场景9名称

public:
    explicit HomeControlSceneDialog(QObject *parent = 0);
    ~HomeControlSceneDialog();

    void setS1text(const QString& s1text)
    {
        m_Scene1Text = s1text;
        emit sig_S1text();
    }
    QString s1text() const {return m_Scene1Text;}    //场景1名称

    void setS2text(const QString& s2text)
    {
        m_Scene2Text = s2text;
        emit sig_S2text();
    }
    QString s2text() const {return m_Scene2Text;}    //场景2名称

    void setS3text(const QString& s3text)
    {
        m_Scene3Text = s3text;
        emit sig_S3text();
    }
    QString s3text() const {return m_Scene3Text;}    //场景3名称

    void setS4text(const QString& s4text)
    {
        m_Scene4Text = s4text;
        emit sig_S4text();
    }
    QString s4text() const {return m_Scene4Text;}    //场景4名称

    void setS5text(const QString& s5text)
    {
        m_Scene5Text = s5text;
        emit sig_S5text();
    }
    QString s5text() const {return m_Scene5Text;}    //场景5名称

    void setS6text(const QString& s6text)
    {
        m_Scene6Text = s6text;
        emit sig_S6text();
    }
    QString s6text() const {return m_Scene6Text;}    //场景6名称

    void setS7text(const QString& s7text)
    {
        m_Scene7Text = s7text;
        emit sig_S7text();
    }
    QString s7text() const {return m_Scene7Text;}    //场景7名称

    void setS8text(const QString& s8text)
    {
        m_Scene8Text = s8text;
        emit sig_S8text();
    }
    QString s8text() const {return m_Scene8Text;}    //场景8名称

    void setS9text(const QString& s9text)
    {
        m_Scene9Text = s9text;
        emit sig_S9text();
    }
    QString s9text() const {return m_Scene9Text;}    //场景9名称


    Q_INVOKABLE void pushButtonScene7Pressed();
    Q_INVOKABLE void pushButtonScene8Pressed();
    Q_INVOKABLE void pushButtonScene9Pressed();
    Q_INVOKABLE void pushButtonScenePressed(int sceneId);
    Q_INVOKABLE void pushBtnScene1Clicked(QString str);
    Q_INVOKABLE void pushBtnScene2Clicked(QString str);
    Q_INVOKABLE void pushBtnScene3Clicked(QString str);
    Q_INVOKABLE void pushBtnScene4Clicked(QString str);
    Q_INVOKABLE void pushBtnScene5Clicked(QString str);
    Q_INVOKABLE void pushBtnScene6Clicked(QString str);



    void sceneBtnNameIni();
    void setSceneName(int index,QString name,QString newname);
    void Translate();          //翻译

signals:
    void sig_S1text(void);
    void sig_S2text(void);
    void sig_S3text(void);
    void sig_S4text(void);
    void sig_S5text(void);
    void sig_S6text(void);
    void sig_S7text(void);
    void sig_S8text(void);
    void sig_S9text(void);
public slots:
private:
    QString m_Scene1Text;
    QString m_Scene2Text;
    QString m_Scene3Text;
    QString m_Scene4Text;
    QString m_Scene5Text;
    QString m_Scene6Text;
    QString m_Scene7Text;
    QString m_Scene8Text;
    QString m_Scene9Text;
};

#endif // HOMECONTROLSCENEDIALOG_H
