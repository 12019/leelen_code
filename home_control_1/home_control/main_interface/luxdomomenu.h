#ifndef LUXDOMOMENU_H
#define LUXDOMOMENU_H

#include<QDeclarativeItem>

class LuxDomoMenu : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit LuxDomoMenu(QDeclarativeItem *parent = 0);
     ~LuxDomoMenu();
     void   componentComplete();
     Q_INVOKABLE   void    showSettingDialog();
     Q_INVOKABLE   void    showHomeManageDialog();
     Q_INVOKABLE   void    showSecurityDialog();
     Q_INVOKABLE   void    showhomepageDialog();

     Q_INVOKABLE   void    inputPasswd();
     Q_INVOKABLE   void    checkPasswd(QString);
     Q_INVOKABLE   void    showInputPasswd();
signals:

public slots:
};

#endif // LUXDOMOMENU_H
