import  QtQuick 1.0
import   LuxDomoAcControl  1.0
import  "../content"
Rectangle
{
    id:acControl
    objectName: "acControl"
    width: 800
    height: 480
    color: "#00000000"

    function   init(roomUid,deviceUid){  pLuxDomoAcControlItem.init(roomUid,deviceUid)  }
    property   string    deviceUid: ""
    property   string    acName: ""
    property   bool     acCondition: false
    property   string    acMode: ""
    property   string    acTemp: ""
    property   string    acSpeed: ""
    property   string    acSwing: ""

    LuxDomoAcControlItem
    {
        id:pLuxDomoAcControlItem
        objectName: "pLuxDomoAcControlItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {  pLuxDomoAcControlItem.close() }
    }

    Item
    {
        width: parent.width*3/5
        height: parent.height*5/8
        y:parent.height/4
        anchors.horizontalCenter: parent.horizontalCenter
        MouseArea{  anchors.fill: parent  }
        Image
        {
            anchors.fill: parent
            source: "../device_control/ac_background.png"
        }
        LuxDomoLabel6
        {
            width: parent.width
            height: parent.height/7
            textName: acName+"控制"
            textSize: 22
        }
        Row
        {
            width: parent.width
            height: parent.height*6/7
            anchors.bottom: parent.bottom
            Item
            {
                width: parent.width/4
                height: parent.height
                Column
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            image:"../device_control/power_off.png"
                            imagepressed: "../device_control/power_on.png"
                            onClicked:
                            {
                                pLuxDomoAcControlItem.acPowerClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName:"开关"
                        }
                    }
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: acCondition
                            image:"../device_control/ac_mode_off.png"
                            imagepressed: "../device_control/ac_mode_on.png"
                            onClicked:
                            {
                                pLuxDomoAcControlItem.acModeClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "模式"
                        }
                    }
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: acCondition
                            image:"../device_control/ac_swing_off.png"
                            imagepressed: "../device_control/ac_swing_on.png"
                            onClicked:
                            {
                                pLuxDomoAcControlItem.acWindClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "风向"
                        }
                    }
                }
            }
            Item
            {
                width: parent.width/2
                height: parent.height
                Image
                {
                    id:deviceImage
                    width: parent.width*7/8
                    height: parent.width*7/8
                    anchors.horizontalCenter:parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    source: acCondition?"../device_control/ac_open.png":"../device_control/ac_close.png"
                }      
                LuxDomoLabel
                {
                    width: deviceImage.width
                    height: deviceImage.height/10
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:deviceImage.height*5/9
                    visible: acCondition
                    textName: acTemp
                    textColor: "#ffffff"
                    textSize: 20
                }
                Item
                {
                    width: deviceImage.width
                    height: deviceImage.height/5
                    anchors.top: deviceImage.top
                    anchors.topMargin: deviceImage.height*9/64
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: acCondition
                    Column
                    {
                        anchors.fill: parent
                       Item
                       {
                           width: parent.width
                           height: parent.height/2
                           LuxDomoImageLabel
                           {
                               width: parent.width
                               height: parent.height
                               anchors.horizontalCenter:parent.horizontalCenter
                               anchors.verticalCenter: parent.verticalCenter
                               textName: acMode
                               textSize: 15
                               imageStr: "../device_control/ac_mode.png"
                           }
                       }
                       Item
                       {
                           width: parent.width
                           height: parent.height/2
                           LuxDomoImageLabel
                           {
                               width: parent.width*2/5
                               height: parent.height
                               anchors.right: parent.right
                               anchors.rightMargin: parent.width/2
                               textName: acSwing
                               textSize: 15
                               imageStr: "../device_control/ac_swing.png"
                           }
                           LuxDomoImageLabel
                           {
                               width: parent.width/3
                               height: parent.height
                               x:parent.width/2
                               textName: acSpeed
                               textSize: 15
                               imageStr: "../device_control/ac_speed.png"
                           }
                       }
                    }
                }
            }
            Item
            {
                width: parent.width/4
                height: parent.height
                Column
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: acCondition
                            image:"../device_control/ac_speed_off.png"
                            imagepressed: "../device_control/ac_speed_on.png"
                            onClicked:
                            {
                               pLuxDomoAcControlItem.acSpeedClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "风速"
                        }
                    }
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: acCondition
                            image:"../device_control/ac_tempUp_off.png"
                            imagepressed: "../device_control/ac_tempUp_on.png"
                            onClicked:
                            {
                                pLuxDomoAcControlItem.acTempUpClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "温度+"
                        }
                    }
                    Item
                    {
                        width: parent.width
                        height: parent.height/3
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: acCondition
                            image:"../device_control/ac_tempDown_off.png"
                            imagepressed: "../device_control/ac_tempDown_on.png"
                            onClicked:
                            {
                                pLuxDomoAcControlItem.acTempDownClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "温度-"
                        }
                    }
                }
            }
        }
    }
}
