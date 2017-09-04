import    QtQuick 1.0
import    LuxDomoTVControl  1.0
import    "../content"
Rectangle
{
    id:tvControl
    objectName: "tvControl"
    width:800
    height:480
    color: "#00000000"

    property   string     deviceUid: ""
    property   bool       tvCondition: false
    property  string      tvName: ""
    property   string     tvMode: ""
    function   init(roomUid,deviceUid) {   pLuxDomoTVControlItem.init(roomUid,deviceUid)  }

    LuxDomoTVControlItem
    {
        id:pLuxDomoTVControlItem
        objectName: "pLuxDomoTVControlItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {  pLuxDomoTVControlItem.close()   }
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
            source: "../device_control/tv_background.png"
        }
        LuxDomoLabel6
        {
            width: parent.width
            height: parent.height/7
            textName: tvName+"控制"
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
                               pLuxDomoTVControlItem.tvPowerClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "开关"
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
                            enabled: tvCondition
                            image:"../device_control/tv_volumeUp_off.png"
                            imagepressed: "../device_control/tv_volumeUp_on.png"
                            onClicked:
                            {
                                pLuxDomoTVControlItem.tvVolumelUpClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "音量+"
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
                            enabled: tvCondition
                            image:"../device_control/tv_volumeDown_off.png"
                            imagepressed: "../device_control/tv_volumeDown_on.png"
                            onClicked:
                            {
                                pLuxDomoTVControlItem.tvVolumeDownClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "音量-"
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
                    source: tvCondition?"../device_control/tv_open.png":"../device_control/tv_close.png"
                }
                LuxDomoLabel
                {
                    width: deviceImage.width
                    height: deviceImage.height/9
                    anchors.top: deviceImage.top
                    anchors.topMargin: deviceImage.height*9/64
                    anchors.horizontalCenter: parent.horizontalCenter
                    textName: tvMode+" 模式"
                    visible: tvCondition
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
                            enabled: tvCondition
                            image:"../device_control/tv_mode_off.png"
                            imagepressed: "../device_control/tv_mode_on.png"
                            onClicked:
                            {
                                pLuxDomoTVControlItem.tvModeClicked()
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
                            enabled: tvCondition
                            image:"../device_control/tv_channelUp_off.png"
                            imagepressed: "../device_control/tv_channelUp_on.png"
                            onClicked:
                            {
                                pLuxDomoTVControlItem.tvChannelUpClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "频道+"
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
                            enabled: tvCondition
                            image:"../device_control/tv_channelDown_off.png"
                            imagepressed:"../device_control/tv_channelDown_on.png"
                            onClicked:
                            {
                                pLuxDomoTVControlItem.tvChannelDownClicked()
                            }
                        }
                        LuxDomoLabel
                        {
                            width: parent.width
                            height: parent.height/4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: parent.height/4
                            textName: "频道-"
                        }
                    }
                }
            }
        }
    }
}
