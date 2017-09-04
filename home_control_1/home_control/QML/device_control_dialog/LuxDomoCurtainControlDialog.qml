import   QtQuick 1.0
import    LuxDomoCurtainControl   1.0
import    "../content"
Rectangle
{
    id:curtainControl
    objectName: "curtainControl"
    width: 800
    height: 480
    color: "#00000000"

    property   string    deviceUid: ""
    property   string    curtainName: ""
    property   string    curtainImage: ""
    property   string    curtainCondition:""
    function   init(roomUid,deviceUid){  pLuxDomoCurtainControlItem.init(roomUid,deviceUid)   }

    LuxDomoCurtainControlItem
    {
          id:pLuxDomoCurtainControlItem
          objectName: "pLuxDomoCurtainControlItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: { pLuxDomoCurtainControlItem.close()  }
    }

    Item
    {
        width: parent.width/2
        height: parent.height*13/16
        anchors.horizontalCenter: parent.horizontalCenter
        y:parent.height*5/32
        MouseArea{  anchors.fill: parent  }
        Image
        {
            anchors.fill: parent
            source: "../device_control/curtain_background.png"
        }
        Column
        {
            anchors.fill: parent
            Item
            {
                width: parent.width
                height: parent.height/9
                LuxDomoLabel6
                {
                    anchors.fill: parent
                    textName: curtainName+"控制"
                    textSize: 22
                }
            }
            Item
            {
                width: parent.width
                height: parent.height*17/24
                Item
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Image
                    {
                        width: parent.width
                        height: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: curtainImage
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/6
                Row
                {
                    width: parent.width*9/10
                    height: parent.height*4/3
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            image:curtainCondition=="on"?"../device_control/curtain_open_on.png":"../device_control/curtain_open_off.png"
                            imagepressed: image
                            onClicked:
                            {
                                curtainCondition="on"
                                curtainImage="../device_control/curtain_open.png"
                                pLuxDomoCurtainControlItem.curtainOpenClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "打开"
                        }
                    }
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            image:curtainCondition=="stop"?"../device_control/curtain_stop_on.png":"../device_control/curtain_stop_off.png"
                            imagepressed: image
                            onClicked:
                            {
                                curtainCondition="stop"
                                curtainImage="../device_control/curtain_stop.png"
                                pLuxDomoCurtainControlItem.curtainPauseClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "暂停"
                        }
                    }
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            image: curtainCondition=="off"?"../device_control/curtain_close_on.png":"../device_control/curtain_close_off.png"
                            imagepressed: image
                            onClicked:
                            {
                                curtainCondition="off"
                                curtainImage="../device_control/curtain_close.png"
                                pLuxDomoCurtainControlItem.curtainCloseClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "关闭"
                        }
                    }
                }
            }
        }
    }
}
