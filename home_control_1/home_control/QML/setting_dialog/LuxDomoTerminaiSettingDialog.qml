import  QtQuick 1.0
import  LuxDomoTerminaiSetting 1.0
import  "../content"

Rectangle
{
    id:terminalSetting
    objectName: "terminalSetting"
    width: 450
    height: 480
    color:"#efeff4"

    property string  terminaiIP: "192.168.80.109"
    property string  terminaiAccountNumber: "555555"
    property string  terminaiPasswd: "******"

    function   setInformation(data){  pLuxDomoTerminaiSettingItem.setInformation(data) }

    LuxDomoTerminaiSettingItem
    {
           id:pLuxDomoTerminaiSettingItem
           objectName: "pLuxDomoTerminaiSettingItem"
    }

    MouseArea{  anchors.fill: parent  }

    Rectangle
    {
        width: parent.width
        height: parent.height/8
        color:"#f6f6f7"
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
           color: "#bfbfbf"
        }
        LuxDomoLabel
        {
            anchors.fill: parent
            textName: "远程设置"
            textSize: 25
        }
    }

    Item
    {
        width: parent.width
        height: parent.height*7/8
        anchors.bottom: parent.bottom
        Column
        {
            width: parent.width*7/8
            height: parent.height*19/20
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Item
            {
                width: parent.width
                height: parent.height/8
                LuxDomoLabel1
                {
                    width: parent.width*2/5
                    height: parent.height
                    textName: "终端机IP"
                    textSize: 22
                    textColor: "#676767"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/8
                Image
                {
                    anchors.fill: parent
                    source: "../setting/version_number.png"
                }
                LuxDomoLabel1
                {
                    anchors.fill: parent
                    textName: terminaiIP
                    textSize: 20
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/8
                LuxDomoLabel1
                {
                    width: parent.width*2/5
                    height: parent.height
                    textName: "请输入登录账号"
                    textSize: 22
                    textColor: "#676767"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/8
                Image
                {
                    anchors.fill: parent
                    source: "../setting/version_number.png"
                }
                LuxDomoLabel1
                {
                    anchors.fill: parent
                    textName: terminaiAccountNumber
                    textSize: 20
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {  pLuxDomoTerminaiSettingItem.editInformation("TerminaiAccountNumber",terminaiAccountNumber)  }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/8
                LuxDomoLabel1
                {
                    width: parent.width*2/5
                    height: parent.height
                    textName: "请输入登录密码"
                    textSize: 22
                    textColor: "#676767"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/8
                Image
                {
                    anchors.fill: parent
                    source: "../setting/version_number.png"
                }
                LuxDomoLabel1
                {
                    anchors.fill: parent
                    textName: terminaiPasswd
                    textSize: 22
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {  pLuxDomoTerminaiSettingItem.editInformation("TerminaiPasswd",terminaiPasswd)  }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height*2/8
                LuxDomoButton
                {
                    width: parent.width/2
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    name:"保存"
                    fontsize: 22
                    onClicked: {  pLuxDomoTerminaiSettingItem.saveInformation()    }
                }
            }
        }
    }
}

