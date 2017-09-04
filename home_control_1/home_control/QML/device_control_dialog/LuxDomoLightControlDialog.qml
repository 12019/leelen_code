import    QtQuick 1.0
import   LuxDomoLightControl  1.0
import  "../content"
Rectangle
{
    id:var_lightControl
    objectName: "var_lightControl"
    width: 800
    height: 480
    color: "#00000000"

    property  string   lightName: ""
    property  string   deivceUid: ""
    property  int        lightValue: 0
    property bool      lightCondition: false
    property string    brightnessValue: "#00000000"

    function  init(roomUid,deviceUid){  pLuxDomoLightControlItem.init(roomUid,deviceUid) }

    onLightValueChanged:
    {
        brightness.progressValue=lightValue
        brightness.changData()
    }

    LuxDomoLightControlItem
    {
        id:pLuxDomoLightControlItem
        objectName: "pLuxDomoLightControlItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {  pLuxDomoLightControlItem.close()  }
    }

    Item
    {
        width: parent.width*3/5
        height: parent.height*13/16
        anchors.horizontalCenter: parent.horizontalCenter
        y:parent.height*5/32
        MouseArea{  anchors.fill: parent  }
        Image
        {
            anchors.fill: parent
            source: "../device_control/var_background.png"
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
                    textName: lightName+"控制"
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
                        width: parent.width*7/8
                        height: parent.width*7/8
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: lightCondition?"../device_control/var_back.png":"../device_control/var_close.png"
                    }
                    Rectangle
                    {
                        width: parent.width*3/4
                        height: parent.width*3/4
                        visible: lightCondition
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 100
                        color: brightnessValue
                    }
                    Image
                    {
                        width: parent.width*3/5
                        height: parent.width*3/5
                        visible: lightCondition
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: "../device_control/var.png"
                    }
                }
                Item
                {
                    width: parent.width/4
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
                            if(lightCondition)
                            {
                                lightCondition=false
                                pLuxDomoLightControlItem.lightPowerClicked(false)
                            }
                            else
                            {
                                lightCondition=true
                                pLuxDomoLightControlItem.lightPowerClicked(true)
                            }
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
            }
            Item
            {
                width: parent.width
                height: parent.height/6
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:0
                            image:"../device_control/var_brightness_small.png"
                            imagepressed: image
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        Item
                       {
                           id:value
                           width: parent.width
                           height: parent.height/2
                           y:-parent.height*7/12
                           anchors.horizontalCenter: parent.horizontalCenter
                           LuxDomoLabel
                           {
                               width:10
                               height: parent.height
                               x:lightValue/100*value.width
                               textName: lightValue+"%"
                               textSize: 15
                           }
                       }
                        LuxDomoProgressBar
                        {
                            id:brightness
                            width: parent.width*6/5
                            height: parent.height/2
                            y:0
                            anchors.horizontalCenter: parent.horizontalCenter
                            progressBarImage1: "../device_control/progressImage_yellow.png"
                            progressBarImage2: "../device_control/progressImage.png"
                            progressButton: "../device_control/progressButton.png"
                            progressValue: lightValue
                            enabled:lightCondition
                            onPositionchanged:
                            {
                                lightValue=progressValue
                                pLuxDomoLightControlItem.lightBrightnessClicked(progressValue)
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height*2/3
                            height: parent.height*2/3
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-parent.height/12
                            image:"../device_control/var_brightness_big.png"
                            imagepressed: image
                        }
                    }
                }
            }
        }
    }
}
