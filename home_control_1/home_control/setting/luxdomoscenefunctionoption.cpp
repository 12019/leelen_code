#include "luxdomoscenefunctionoption.h"
#include"maindialog.h"
#include"MyBuffer.h"
extern   MainDialog *g_pMainDlg;
extern   LMap_6410_Msg mLMap_6410_Msg;
extern   EditDataType     dataType;
LuxDomoSceneFunctionOption::LuxDomoSceneFunctionOption(QDeclarativeItem *parent) : QDeclarativeItem(parent)
{
       connect(this,SIGNAL(sigDeleteHomeScene(QString)),g_pMainDlg->m_pHomeControlDlg->m_pHomeScene,SLOT(slot_deleteHomeScene(QString)));
}

LuxDomoSceneFunctionOption::~LuxDomoSceneFunctionOption()
{


}

void   LuxDomoSceneFunctionOption::componentComplete()
{
           MyMsgLog<<parentItem()->objectName()<<objectName();
}

void   LuxDomoSceneFunctionOption::deleteScene()
{
          QMetaObject::invokeMethod(parentItem()->parentItem(),"deleteHomeScene",Qt::DirectConnection,Q_ARG(QVariant,parentItem()->property("index").toInt()));
          emit   sigDeleteHomeScene(parentItem()->property("sceneFile").toString());
          parentItem()->deleteLater();
}

void  LuxDomoSceneFunctionOption::editSceneName()
{
          dataType=SceneName;
          g_pMainDlg->m_pHomeControlDlg->setHCLineText(parentItem()->property("sceneName").toString());
}
