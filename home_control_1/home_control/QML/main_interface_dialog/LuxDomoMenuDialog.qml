import QtQuick 1.0
import  "../content"
import   LuxDomoMenu 1.0
Rectangle
{
    id:menu
    objectName: "menu"
     width: 100
     height: 480
     color: "#3c3636"

     property  bool    showPasswd: false
     property  string  menuCondition: "homepage"
     property  int       passwdDialogW: 896

     LuxDomoMenuItem
     {
         id:pLuxDomoMenuItem
         objectName: "pLuxDomoMenu"
     }

     MouseArea{  anchors.fill: parent  }
     Image
     {
         anchors.fill: parent
         source: "../menu/menu_background.png"
     }

     Column
     {
         anchors.fill: parent
         Item
         {
             width: parent.width
             height: parent.height/4
             anchors.horizontalCenter: parent.horizontalCenter
            LuxDomoButton
            {
                id:homepageButton
                width: parent.height/2
                height: parent.height/2
                y:parent.height/8
                anchors.horizontalCenter: parent.horizontalCenter
                image:menuCondition=="homepage"?"../menu/homepage_on.png":"../menu/homepage_off.png"
                imagepressed: homepageButton.image
            }
            LuxDomoLabel
            {
                width: parent.width
                height:parent.height*3/8
                anchors.top: homepageButton.bottom
                textName: "首页"
                textSize: 22
                textColor: "#cccccc"
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    menuCondition="homepage"
                    pLuxDomoMenuItem.showhomepageDialog()
                }
            }
         }
         Item
         {
             width: parent.width
             height: parent.height/4
             anchors.horizontalCenter: parent.horizontalCenter
            LuxDomoButton
            {
                id:manageButton
                width: parent.height/2
                height: parent.height/2
                y:parent.height/8
                anchors.horizontalCenter: parent.horizontalCenter
                image:menuCondition=="homemanage"?"../menu/homemanage_on.png":"../menu/homemanage_off.png"
                imagepressed: manageButton.image
            }
            LuxDomoLabel
            {
                width: parent.width
                height:parent.height*3/8
                anchors.top: manageButton.bottom
                textName: "家居管理"
                textSize: 22
                textColor: "#cccccc"
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    menuCondition="homemanage"
                    inputPasswd.passwd=""
                    inputPasswd.passwd1=""
                    showPasswd=true
                    pLuxDomoMenuItem.showInputPasswd()
                }
            }
         }
         Item
         {
             width: parent.width
             height: parent.height/4
             anchors.horizontalCenter: parent.horizontalCenter
             LuxDomoButton
             {
                 id:securityButton
                 width: parent.height/2
                 height: parent.height/2
                 y:parent.height/8
                 anchors.horizontalCenter: parent.horizontalCenter
                 image:menuCondition=="security"?"../menu/security_on.png":"../menu/security_off.png"
                 imagepressed: securityButton.image
             }
             LuxDomoLabel
             {
                 width: parent.width
                 height:parent.height*3/8
                 anchors.top: securityButton.bottom
                 textName: "安防布置"
                 textSize: 22
                 textColor: "#cccccc"
             }
             MouseArea
             {
                 anchors.fill: parent
                 onClicked:
                 {
                     menuCondition="security"
                     pLuxDomoMenuItem.showSecurityDialog()
                 }
             }
         }
         Item
         {
             width: parent.width
             height: parent.height/4
             anchors.horizontalCenter: parent.horizontalCenter
             LuxDomoButton
             {
                 id:setttingButton
                 width: parent.height/2
                 height: parent.height/2
                 y:parent.height/8
                 anchors.horizontalCenter: parent.horizontalCenter
                 image:menuCondition=="setting"?"../menu/setting_on.png":"../menu/setting_off.png"
                 imagepressed: setttingButton.image
             }
             LuxDomoLabel
             {
                 width: parent.width
                 height:parent.height*3/8
                 anchors.top: setttingButton.bottom
                 textName: "设置选项"
                 textSize: 22
                 textColor: "#cccccc"
             }
             MouseArea
             {
                 anchors.fill: parent
                 onClicked:
                 {
                     menuCondition="setting"
                     pLuxDomoMenuItem.showSettingDialog()
                 }
             }
         }
     }

     LuxDomoInputPasswd
     {
         id:inputPasswd
         width: passwdDialogW
         height: parent.height
         anchors.left: parent.right
         visible: showPasswd
         onCheckPasswd:
         {
             pLuxDomoMenuItem.checkPasswd(passwd)
             passwd=""
             passwd1=""
         }
     }
}

