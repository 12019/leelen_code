import   QtQuick 1.0
import   LuxDomoCloudSetting 1.0
import   "../content"
Rectangle
{
    id:cloudSetting
    objectName: "cloudSetting"
    width: 450
    height: 480
    color: "#00000000"

    property string   iP: "117.29.160.158"
    property string   iD: ""
    property string   condition: ""

    function   setInformation(data){  pLuxDomoCloudSettingItem.setInformation(data)  }

    LuxDomoCloudSettingItem
    {
        id:pLuxDomoCloudSettingItem
        objectName: "pLuxDomoCloudSettingItem"
    }

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
            textName: "云服务代理"
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
                    textName: "请输入云连接ID"
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
                    textName: iD
                    textSize: 20
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {  pLuxDomoCloudSettingItem.editInformation("CloudID",iD)  }
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
                    textName: "云连接IP"
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
                    textName: iP
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
                    textName: "云连接状态"
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
                    textName: condition
                    textSize: 20
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
                    onClicked: { pLuxDomoCloudSettingItem.saveInformation()  }
                }
            }
        }
    }
}
